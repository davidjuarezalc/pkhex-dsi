#include "loader.hpp"

#include "cardSaves.hpp"
#include "cartSaves.hpp"
#include "config.hpp"
#include "fileBrowse.hpp"
#include "flashcard.hpp"
#include "i18n_ext.hpp"

#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

static std::string saveFileName;

std::shared_ptr<pksm::Sav> save;

bool loadSave(std::string savePath) {
	save         = nullptr;
	saveFileName = savePath;
	FILE *in     = fopen(savePath.c_str(), "rb");
	if(in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);
		fclose(in);
		save = pksm::Sav::getSave(saveData, size);
	} else {
		saveFileName = "";
		return false;
	}
	if(!save) {
		saveFileName = "";
		return false;
	}
	i18n::updateTMHMNames(Config::getLang("lang"));
	return true;
}

void saveChanges(std::string savePath) {
	// Parse file name and parent directory safely
	std::string saveParentDir;
	std::string saveFile;

	size_t lastSlash = savePath.find_last_of('/');
	if(lastSlash != std::string::npos) {
		saveParentDir = savePath.substr(0, lastSlash);
		saveFile = savePath.substr(lastSlash + 1);
	} else {
		char cwd[PATH_MAX];
		getcwd(cwd, PATH_MAX);
		saveParentDir = cwd;
		saveFile = savePath;
	}

	while(!saveParentDir.empty() && saveParentDir.back() == '/') {
		saveParentDir.pop_back();
	}

	std::string saveStem = saveFile;
	size_t dotPos = saveFile.find_last_of('.');
	if(dotPos != std::string::npos) {
		saveStem = saveFile.substr(0, dotPos);
	}

	bool isSlotSave = (savePath == cardSave || savePath == cartSave);

	DateTime now = DateTime::now();

	// Strategy 1: Dedicated global backup folder /_nds/pkmn-chest/backups/<game>/
	char backupDir[PATH_MAX];
	snprintf(backupDir,
			 sizeof(backupDir),
			 "%s:/_nds/pkmn-chest/backups/%s",
			 mainDrive().c_str(),
			 saveStem.c_str());
	mkdir(backupDir, 0777);

	char backupPath[PATH_MAX];
	snprintf(backupPath,
			 sizeof(backupPath),
			 "%s/%.4ld%.2d%.2d-%.2d%.2d%.2d.sav.bak",
			 backupDir,
			 now.year(),
			 now.month(),
			 now.day(),
			 now.hour(),
			 now.minute(),
			 now.second());
	fcopy(savePath.c_str(), backupPath);

	// Strategy 2: Local backup directory directly next to the save file: <saveDir>/backups/
	char localBackupDir[PATH_MAX];
	char localBackupPath[PATH_MAX];
	if(!isSlotSave) {
		snprintf(localBackupDir, sizeof(localBackupDir), "%s/backups", saveParentDir.c_str());
		mkdir(localBackupDir, 0777);

		snprintf(localBackupPath,
				 sizeof(localBackupPath),
				 "%s/%s_%.4ld%.2d%.2d_%.2d%.2d%.2d.sav.bak",
				 localBackupDir,
				 saveStem.c_str(),
				 now.year(),
				 now.month(),
				 now.day(),
				 now.hour(),
				 now.minute(),
				 now.second());
		fcopy(savePath.c_str(), localBackupPath);
	}

	// Only clean up old backups if explicitly configured (> 0). Default is 0 (keep all old backups!).
	if(Config::getInt("backupAmount") > 0) { // 0 = unlimited, never delete old backups
		char savDir[PATH_MAX];
		getcwd(savDir, PATH_MAX);

		// Clean global backup folder
		chdir(backupDir);
		std::vector<DirEntry> dirContents;
		getDirectoryContents(dirContents, {"bak"});
		while((int)dirContents.size() > Config::getInt("backupAmount") && dirContents.size() > 1) {
			remove(dirContents[1].name.c_str()); // index 0 is '..'
			dirContents.erase(dirContents.begin() + 1);
		}

		// Clean local backup folder if applicable
		if(!isSlotSave) {
			chdir(localBackupDir);
			std::vector<DirEntry> localContents;
			getDirectoryContents(localContents, {"bak"});
			while((int)localContents.size() > Config::getInt("backupAmount") && localContents.size() > 1) {
				remove(localContents[1].name.c_str());
				localContents.erase(localContents.begin() + 1);
			}
		}

		chdir(savDir);
	}

	save->finishEditing();

	// Atomic save write: Write to temporary file first, then flush and overwrite original
	std::string tempSavePath = savePath + ".tmp";
	FILE *outTemp = fopen(tempSavePath.c_str(), "wb");
	if(outTemp) {
		fwrite(save->rawData().get(), 1, save->getLength(), outTemp);
		fflush(outTemp);
		fclose(outTemp);
		// Replace original save with the verified written temp file
		remove(savePath.c_str());
		rename(tempSavePath.c_str(), savePath.c_str());
	} else {
		// Fallback to in-place overwrite if tmp file creation fails
		FILE *out = fopen(savePath.c_str(), "rb+");
		if(out) {
			fwrite(save->rawData().get(), 1, save->getLength(), out);
			fflush(out);
			fclose(out);
		}
	}

	save->beginEditing();
}
