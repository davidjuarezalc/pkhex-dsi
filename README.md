# PKHeX DSi (nds-pkhex)

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Nintendo%20DS%20%7C%20DSi%20%7C%203DS-red.svg)](https://github.com/DS-Homebrew/TWiLightMenu)
[![Toolchain](https://img.shields.io/badge/Built%20with-BlocksDS-green.svg)](https://github.com/blocksds/sdk)

**PKHeX DSi** is a standalone, on-console Pokémon save editor and box manager running natively as a `.nds` homebrew executable. Designed specifically for **Nintendo DSi** (with TWiLight Menu++) and Nintendo DS / DS Lite flashcards (R4, Acekard, DSTWO).

> 🌐 **[Leer esta guía en Español](README.es.md)**

---

## 💡 About this Project

*PKHeX DSi* brings the convenience of on-the-go Pokémon save editing directly to your Nintendo DS/DSi hardware, without needing to extract your SD card or boot a computer.

* **Inspired by:** [PKHeX](https://github.com/kwsch/PKHeX) by Kaphotics.
* **Built upon:** The solid foundation of [pkmn-chest](https://github.com/Universal-Team/pkmn-chest) and [PKSM-Core](https://github.com/Flagbrew/PKSM-Core) by Universal-Team and BernardoGiordano.
* **License:** GNU General Public License v3.0 (GPLv3).

---

## 🎮 Supported Games

| Generation | Platform | Games | Save File Format |
| :--- | :--- | :--- | :--- |
| **Generation 3** | GBA | Ruby, Sapphire, Emerald, FireRed, LeafGreen | 128 KB `.sav` (via GBARunner2, GBARunner3, or Slot-2) |
| **Generation 4** | NDS | Diamond, Pearl, Platinum, HeartGold, SoulSilver | 512 KB `.sav` |
| **Generation 5** | NDS | Black, White, Black 2, White 2 | 512 KB `.sav` |

---

## ✨ Improvements & Features over Upstream

### 🛡️ 1. Dual Timestamped Backups & Atomic File Writes
* **Never lose a save**: Every time you save changes, PKHeX DSi automatically creates chronological timestamped backups in two independent locations:
  1. Global backup folder: `sd:/_nds/pkmn-chest/backups/<game>/YYYYMMDD-HHMMSS.sav.bak`
  2. Local directory next to the save: `<save_folder>/backups/<game>_YYYYMMDD_HHMMSS.sav.bak`
* **Safe Streaming Atomic Writes**: Writes are buffered in safe 16 KB chunks to a temporary file before atomic replacement, protecting your data against battery cutoffs or flashcard buffer errors.
* **Party Validation**: Enforces `fixParty()` checks to guarantee valid party count and checksums.

### 🧬 2. Complete Visual Ability Selection
* The original ability toggle (which only flipped between 0 and 1) has been completely redesigned.
* Now presents an explicit, categorized selection:
  * `[1]` Primary standard ability.
  * `[2]` Secondary standard ability.
  * `[H]` Hidden Ability (Gen 5 / Dream World).
  * Full alphabetical search of all valid generation abilities.

### 🎒 3. Dynamic TM/HM Move Names in Bag
* In-game TM/HM items now dynamically resolve and display the move they teach based on the current game and selected language (e.g., `TM01 Focus Punch` / `MT01 Puño Certero` instead of just `TM01`).

### 🔍 4. Accent-Insensitive & Case-Insensitive Search
* Full Unicode / Latin normalization (`normalizeForSort`) across moves, items, abilities, and species. Searching for `puño`, `puno`, `epee`, or `flamme` works smoothly in Spanish, French, German, and Italian.

### 🚀 5. ARM9 Memory Footprint Optimization (NDS 4 MB RAM)
* The original Nintendo DS has only 4 MB of RAM (16 MB on DSi).
* Converted dynamic heap `std::vector` UI allocations into static `constexpr` tables across all menus, eliminating memory fragmentation.
* Pruned unused Gen 6, 7, and 8 location lookup tables from the embedded core, saving dozens of kilobytes of RAM.

### 🐛 6. Critical Bug Fixes
* **Speed Sort Inversion Fixed**: Fixed an upstream bug where sorting by Speed was accidentally comparing Special Defense against Speed.
* **Touchscreen Hitbox Desync Fixed**: Resolved coordinate offset bugs in Filter and Stats menus where row taps calculated hitboxes based on the selected item rather than the tapped row.
* **Safe Number Input**: Replaced unchecked `std::stoi` with exception-safe, bounds-clamped `std::stoul` parsing to prevent crashes when inputting stats or trainer money.
* **Hidden Power Out-of-Bounds Fix**: Prevented underflow crashes when inspecting or setting Hidden Power type.
* **Slot 1 DMA Alignment**: Fixed header read alignment for physical DS game cartridges.
* **Memory & File Descriptor Leaks**: Fixed unclosed `FILE*` handles and leaked line buffers in the favorites browser.

---

## 📥 Installation

1. Download the latest `pkhex_dsi.nds` from the **[Releases](https://github.com/david/pkhex-dsi/releases)** page.
2. Insert your Nintendo DSi / DS SD card into your computer.
3. Copy `pkhex_dsi.nds` to your SD card (e.g., in `sd:/roms/nds/` or the root `sd:/`).
4. *(Recommended)* Ensure the following folder exists for fast initialization:
   ```text
   sd:/_nds/pkmn-chest/
   ```
5. Insert the SD card back into your Nintendo DSi and launch **TWiLight Menu++** (or your flashcard kernel).
6. Run `pkhex_dsi.nds`.
7. Browse your SD card to select any GBA or NDS Pokémon save file (`.sav`).

---

## 🛠️ Building from Source

### Option A: 1-Click Docker Build (Recommended)
Requires [Docker](https://www.docker.com/) installed and running.

```bash
# Clone the repository
git clone https://github.com/david/pkhex-dsi.git
cd pkhex-dsi

# Run the build script
./docker/build.sh
```
The compiled binary will be placed in `bin/pkhex_dsi.nds`.

### Option B: Local BlocksDS Toolchain
If you have [BlocksDS](https://github.com/blocksds/sdk) installed locally:

```bash
make -j$(nproc)
```

---

## 📜 Credits & Acknowledgments

* **[Universal-Team](https://github.com/Universal-Team)**: Creators of [pkmn-chest](https://github.com/Universal-Team/pkmn-chest), providing the original DS UI and framework.
* **[BernardoGiordano](https://github.com/BernardoGiordano)** & **Flagbrew**: Creators of [PKSM](https://github.com/BernardoGiordano/PKSM) and [PKSM-Core](https://github.com/Flagbrew/PKSM-Core).
* **[Kaphotics](https://github.com/kwsch)** & the PKHeX development team: For inspiring the entire Pokémon save editing community.
* **[AntonioND](https://github.com/AntonioND)** and the BlocksDS team: For the modern, open-source C/C++ Nintendo DS development SDK.

---

## 📄 License

This project is licensed under the **GNU General Public License v3.0 (GPLv3)**. See the [LICENSE](LICENSE) file for complete details.
