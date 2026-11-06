#ifndef I18N_EXT_HPP
#define I18N_EXT_HPP

#include "json.hpp"
#include "Sav.hpp"

namespace i18n {
	const std::string &pouch(pksm::Language lang, pksm::Sav::Pouch);

	void load(pksm::Language lang, const std::string& name, nlohmann::json& json);

	void initGui(pksm::Language lang);
	void exitGui(pksm::Language lang);
	const std::string &localize(pksm::Language lang, const std::string &v);
	const std::string &localize(pksm::Language lang, const char *v);

	// Update TM/HM item names to include their moves
	void updateTMHMNames(pksm::Language lang);

	// Accent-insensitive and case-insensitive comparison for alphabetical sorting
	bool compareAlphabetical(const std::string &a, const std::string &b);
	std::string normalizeForSort(const std::string &s);
}

#endif
