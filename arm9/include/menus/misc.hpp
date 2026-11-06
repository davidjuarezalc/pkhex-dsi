#ifndef MENU_MISC
#define MENU_MISC

#include "Nature.hpp"
#include "Sav.hpp"
#include "Type.hpp"
#include "i18n_ext.hpp"
#include <algorithm>

// Shows the form selection menu
int selectForm(const pksm::PKX &pkm);

// Shows a list of items in a vector to select from
std::string selectItem(int current, const std::vector<std::string> &strings);

// Shows a list of items in a vector to select from
template <typename T>
T selectItem(T current, const std::set<T> &validItems, const std::vector<std::string>& strings) {
	std::vector<std::string> availableItems;
	std::string currentStr = "";
	if((size_t)current < strings.size()) {
		currentStr = strings[(size_t)current];
	}

	for(unsigned int i=0;i<strings.size();i++) {
		if(validItems.count(T(i)) != 0) {
			availableItems.push_back(strings[i]);
		}
	}

	// Sort available items alphabetically, keeping "None" (index 0) at top if present
	if(availableItems.size() > 1) {
		auto startIt = availableItems.begin();
		if(!availableItems.empty() && (availableItems[0].empty() || availableItems[0] == "-----" || availableItems[0] == "None" || availableItems[0] == "---" || availableItems[0] == "Ninguno")) {
			startIt++;
		}
		std::sort(startIt, availableItems.end(), i18n::compareAlphabetical);
	}

	int currentIndex = 0;
	for(size_t i = 0; i < availableItems.size(); i++) {
		if(availableItems[i] == currentStr) {
			currentIndex = i;
			break;
		}
	}

	std::string selection = selectItem(currentIndex, availableItems);

	for(unsigned int i=0;i<strings.size();i++) {
		if(strings[i] == selection)	return T(i);
	}
	return current;
}

// Shows a list of items in a vector to select from
template <typename T>
T selectItem(T current, int start, int max, const std::vector<std::string> &strings) {
	std::string currentStr = "";
	if((int)current >= start && (int)current < max && (size_t)current < strings.size()) {
		currentStr = strings[(size_t)current];
	}

	std::vector<std::string> availableItems(&strings[start], &strings[max]);

	// Sort available items alphabetically, keeping first item if it is None/placeholder
	if(availableItems.size() > 1) {
		auto startIt = availableItems.begin();
		if(!availableItems.empty() && (availableItems[0].empty() || availableItems[0] == "-----" || availableItems[0] == "None" || availableItems[0] == "---" || availableItems[0] == "Ninguno")) {
			startIt++;
		}
		std::sort(startIt, availableItems.end(), i18n::compareAlphabetical);
	}

	int currentIndex = 0;
	for(size_t i = 0; i < availableItems.size(); i++) {
		if(availableItems[i] == currentStr) {
			currentIndex = i;
			break;
		}
	}

	std::string selection = selectItem(currentIndex, availableItems);

	for(unsigned int i=0;i<strings.size();i++) {
		if(strings[i] == selection)	return T(i);
	}

	return current;
}

// Shows the move editing menu
void selectMoves(pksm::PKX &pkm);

// Shows the nature selection menu
pksm::Nature selectNature(pksm::Nature currentNature);

// Shows the Pokéball selection menu
pksm::Ball selectPokeball(pksm::Ball currentBall);

// Shows the Wallpaper selection menu
int selectWallpaper(int currentWallpaper);

// Shows the box selection menu
int selectBox(int currentBox);

// Shows the origin editing menu
void selectOrigin(pksm::PKX &pkm);

// Shows the stat editing menu
void selectStats(pksm::PKX &pkm);

// Shows the type selection menu
pksm::Type selectHPType(pksm::Type type);

#endif
