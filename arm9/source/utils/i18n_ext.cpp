#include "i18n_ext.hpp"

#include "../core/source/i18n/i18n_internal.hpp"
#include "i18n.hpp"

namespace i18n {
	extern std::unordered_map<pksm::Language, std::vector<std::string>> items;
	extern std::unordered_map<pksm::Language, std::vector<std::string>> items3;
}

const std::string &i18n::pouch(pksm::Language lang, pksm::Sav::Pouch pouch) {
	switch(pouch) {
		case pksm::Sav::Pouch::NormalItem:
			return localize(lang, "ITEMS");
			break;
		case pksm::Sav::Pouch::KeyItem:
			return localize(lang, "KEY_ITEMS");
			break;
		case pksm::Sav::Pouch::TM:
			return localize(lang, "TMHM");
			break;
		case pksm::Sav::Pouch::Mail:
			return localize(lang, "MAIL");
			break;
		case pksm::Sav::Pouch::Medicine:
			return localize(lang, "MEDICINE");
			break;
		case pksm::Sav::Pouch::Berry:
			return localize(lang, "BERRIES");
			break;
		case pksm::Sav::Pouch::Ball:
			return localize(lang, "BALLS");
			break;
		case pksm::Sav::Pouch::Battle:
			return localize(lang, "BATTLE_ITEMS");
			break;
		case pksm::Sav::Pouch::Candy:
			return localize(lang, "CANDIES");
			break;
		case pksm::Sav::Pouch::ZCrystals:
			return localize(lang, "ZCRYSTALS");
			break;
		case pksm::Sav::Pouch::Treasure:
			return localize(lang, "TREASURES");
			break;
		case pksm::Sav::Pouch::Ingredient:
			return localize(lang, "INGREDIENTS");
			break;
		case pksm::Sav::Pouch::PCItem:
			return localize(lang, "PC_ITEMS");
			break;
		case pksm::Sav::Pouch::RotomPower:
			return localize(lang, "ROTOM_POWERS");
			break;
		case pksm::Sav::Pouch::CatchingItem:
			return localize(lang, "CATCHING_ITEMS");
			break;
		default:
			return emptyString;
			break;
	}
}

std::unordered_map<pksm::Language, nlohmann::json> gui;

// load function for json
void i18n::load(pksm::Language lang, const std::string &name, nlohmann::json &json) {
	std::string path = io::exists(_PKSMCORE_LANG_FOLDER + i18n::folder(lang) + name)
		? _PKSMCORE_LANG_FOLDER + i18n::folder(lang) + name
		: _PKSMCORE_LANG_FOLDER + i18n::folder(pksm::Language::ENG) + name;

	FILE *values = fopen(path.c_str(), "rt");
	if(values) {
		json = nlohmann::json::parse(values, nullptr, false);
		fclose(values);
	}
}

void i18n::initGui(pksm::Language lang) {
	nlohmann::json j;
	load(lang, "/gui.json", j);

	gui.insert_or_assign(lang, std::move(j));
}

void i18n::exitGui(pksm::Language lang) { gui.erase(lang); }

const std::string &i18n::localize(pksm::Language lang, const std::string &v) {
	if(v.empty()) {
		return emptyString;
	}
	checkInitialized(lang);
	auto it = gui.find(lang);
	if(it != gui.end()) {
		if(!it->second.contains(v)) {
			it->second[v] = "MISSING: " + v;
		}
		return it->second[v].get_ref<const std::string &>();
	}
	return emptyString;
}

const std::string &i18n::localize(pksm::Language lang, const char *v) {
	if(!v || v[0] == '\0') {
		return emptyString;
	}
	checkInitialized(lang);
	auto it = gui.find(lang);
	if(it != gui.end()) {
		if(!it->second.contains(v)) {
			it->second[v] = std::string("MISSING: ") + v;
		}
		return it->second[v].get_ref<const std::string &>();
	}
	return emptyString;
}

std::string i18n::normalizeForSort(const std::string &s) {
	std::string result;
	result.reserve(s.length());
	size_t i = 0;
	while (i < s.length()) {
		unsigned char c = static_cast<unsigned char>(s[i]);
		if (c < 0x80) {
			result.push_back(std::tolower(c));
			i++;
		} else if (c == 0xC3 && i + 1 < s.length()) {
			unsigned char c2 = static_cast<unsigned char>(s[i + 1]);
			// A with accents: À-Å (0x80-0x85), à-å (0xA0-0xA5)
			if ((c2 >= 0x80 && c2 <= 0x85) || (c2 >= 0xA0 && c2 <= 0xA5)) {
				result.push_back('a');
			}
			// E with accents: È-Ë (0x88-0x8B), è-ë (0xA8-0xAB)
			else if ((c2 >= 0x88 && c2 <= 0x8B) || (c2 >= 0xA8 && c2 <= 0xAB)) {
				result.push_back('e');
			}
			// I with accents: Ì-Ï (0x8C-0x8F), ì-ï (0xAC-0xAF)
			else if ((c2 >= 0x8C && c2 <= 0x8F) || (c2 >= 0xAC && c2 <= 0xAF)) {
				result.push_back('i');
			}
			// O with accents: Ò-Ö, Ø (0x92-0x96, 0x98), ò-ö, ø (0xB2-0xB6, 0xB8)
			else if ((c2 >= 0x92 && c2 <= 0x96) || c2 == 0x98 || (c2 >= 0xB2 && c2 <= 0xB6) || c2 == 0xB8) {
				result.push_back('o');
			}
			// U with accents: Ù-Ü (0x99-0x9C), ù-ü (0xB9-0xBC)
			else if ((c2 >= 0x99 && c2 <= 0x9C) || (c2 >= 0xB9 && c2 <= 0xBC)) {
				result.push_back('u');
			}
			// C with cedilla: Ç (0x87), ç (0xA7)
			else if (c2 == 0x87 || c2 == 0xA7) {
				result.push_back('c');
			}
			// N with tilde: Ñ (0x91), ñ (0xB1) -> sort right after 'n'
			else if (c2 == 0x91 || c2 == 0xB1) {
				result.push_back('n');
				result.push_back('~');
			} else {
				result.push_back(static_cast<char>(c));
				result.push_back(static_cast<char>(c2));
			}
			i += 2;
		} else {
			result.push_back(static_cast<char>(c));
			i++;
		}
	}
	return result;
}

bool i18n::compareAlphabetical(const std::string &a, const std::string &b) {
	std::string normA = normalizeForSort(a);
	std::string normB = normalizeForSort(b);
	if (normA != normB) {
		return normA < normB;
	}
	return a < b;
}

extern std::shared_ptr<pksm::Sav> save;

void i18n::updateTMHMNames(pksm::Language lang) {
	checkInitialized(lang);

	// Gen 4 DP/Pt TM01-92 move IDs
	static const u16 gen4TMs[92] = {
		264, 337, 352, 347, 46,  92,  258, 339, 331, 237,
		241, 269, 58,  59,  63,  113, 182, 240, 202, 219,
		218, 76,  231, 85,  87,  89,  216, 91,  94,  247,
		280, 104, 115, 351, 53,  188, 201, 126, 317, 332,
		259, 263, 290, 156, 213, 168, 211, 285, 289, 315,
		355, 411, 412, 206, 362, 374, 451, 203, 406, 409,
		261, 318, 373, 153, 421, 371, 278, 416, 397, 148,
		444, 419, 86,  360, 14,  446, 244, 445, 399, 157,
		404, 214, 363, 398, 138, 447, 207, 365, 369, 164,
		430, 433
	};

	// Gen 5 BW/B2W2 TM01-95 move IDs
	static const u16 gen5TMs[95] = {
		468, 337, 473, 347, 46,  92,  258, 339, 474, 237,
		241, 269, 58,  59,  63,  113, 182, 240, 477, 219,
		218, 76,  479, 85,  87,  89,  216, 91,  94,  247,
		280, 104, 115, 482, 53,  188, 201, 126, 317, 332,
		259, 263, 488, 156, 213, 168, 490, 496, 497, 315,
		502, 411, 412, 206, 503, 374, 451, 507, 510, 511,
		261, 512, 373, 153, 421, 371, 514, 416, 397, 148,
		444, 521, 86,  360, 14,  522, 244, 523, 524, 157,
		404, 525, 526, 398, 138, 447, 207, 365, 369, 164,
		430, 433, 528, 249, 555
	};

	pksm::Generation gen = save ? save->generation() : pksm::Generation::FOUR;
	pksm::GameVersion ver = save ? save->version() : pksm::GameVersion::SS;

	// Update items[lang]
	if (items.count(lang) > 0) {
		std::vector<std::string> &itemVec = items[lang];

		if (gen == pksm::Generation::FIVE) {
			// Gen 5 TMs: 328..419 (TM01..TM92), 618..620 (TM93..TM95)
			for (int i = 0; i < 92; i++) {
				u16 itemIdx = 328 + i;
				if (itemIdx < itemVec.size()) {
					std::string prefix = itemVec[itemIdx];
					size_t spacePos = prefix.find(' ');
					if (spacePos != std::string::npos) {
						prefix = prefix.substr(0, spacePos);
					}
					itemVec[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen5TMs[i]));
				}
			}
			for (int i = 0; i < 3; i++) {
				u16 itemIdx = 618 + i;
				if (itemIdx < itemVec.size()) {
					std::string prefix = itemVec[itemIdx];
					size_t spacePos = prefix.find(' ');
					if (spacePos != std::string::npos) {
						prefix = prefix.substr(0, spacePos);
					}
					itemVec[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen5TMs[92 + i]));
				}
			}

			// Gen 5 HMs: 420..425 (HM01..HM06)
			// Cut (15), Fly (19), Surf (57), Strength (70), Waterfall (127), Dive (291)
			static const u16 gen5HMs[6] = {15, 19, 57, 70, 127, 291};
			for (int i = 0; i < 6; i++) {
				u16 itemIdx = 420 + i;
				if (itemIdx < itemVec.size()) {
					std::string prefix = itemVec[itemIdx];
					size_t spacePos = prefix.find(' ');
					if (spacePos != std::string::npos) {
						prefix = prefix.substr(0, spacePos);
					}
					itemVec[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen5HMs[i]));
				}
			}
		} else {
			// Gen 4 TMs: 328..419 (TM01..TM92)
			for (int i = 0; i < 92; i++) {
				u16 itemIdx = 328 + i;
				if (itemIdx < itemVec.size()) {
					std::string prefix = itemVec[itemIdx];
					size_t spacePos = prefix.find(' ');
					if (spacePos != std::string::npos) {
						prefix = prefix.substr(0, spacePos);
					}
					itemVec[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen4TMs[i]));
				}
			}

			// Gen 4 HMs: 420..427 (HM01..HM08)
			// Cut (15), Fly (19), Surf (57), Strength (70),
			// HM05: Defog (432) in DP/Pt, Whirlpool (250) in HGSS,
			// Rock Smash (249), Waterfall (127), Rock Climb (431)
			u16 hm05Move = (ver == pksm::GameVersion::HG || ver == pksm::GameVersion::SS) ? 250 : 432;
			u16 gen4HMs[8] = {15, 19, 57, 70, hm05Move, 249, 127, 431};
			for (int i = 0; i < 8; i++) {
				u16 itemIdx = 420 + i;
				if (itemIdx < itemVec.size()) {
					std::string prefix = itemVec[itemIdx];
					size_t spacePos = prefix.find(' ');
					if (spacePos != std::string::npos) {
						prefix = prefix.substr(0, spacePos);
					}
					itemVec[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen4HMs[i]));
				}
			}
		}
	}

	// Update items3[lang] (Gen 3)
	if (items3.count(lang) > 0) {
		std::vector<std::string> &itemVec3 = items3[lang];
		// Gen 3 TMs: 289..338 (TM01..TM50) matches gen4TMs[0..49]
		for (int i = 0; i < 50; i++) {
			u16 itemIdx = 289 + i;
			if (itemIdx < itemVec3.size()) {
				std::string prefix = itemVec3[itemIdx];
				size_t spacePos = prefix.find(' ');
				if (spacePos != std::string::npos) {
					prefix = prefix.substr(0, spacePos);
				}
				itemVec3[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen4TMs[i]));
			}
		}

		// Gen 3 HMs: 339..346 (HM01..HM08)
		// Cut (15), Fly (19), Surf (57), Strength (70), Flash (148), Rock Smash (249), Waterfall (127), Dive (291)
		static const u16 gen3HMs[8] = {15, 19, 57, 70, 148, 249, 127, 291};
		for (int i = 0; i < 8; i++) {
			u16 itemIdx = 339 + i;
			if (itemIdx < itemVec3.size()) {
				std::string prefix = itemVec3[itemIdx];
				size_t spacePos = prefix.find(' ');
				if (spacePos != std::string::npos) {
					prefix = prefix.substr(0, spacePos);
				}
				itemVec3[itemIdx] = prefix + " " + i18n::move(lang, pksm::Move(gen3HMs[i]));
			}
		}
	}
}
