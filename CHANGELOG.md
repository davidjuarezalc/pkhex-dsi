# Changelog

All notable changes to the **PKHeX DSi** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] - 2026-11-06

### Added
* **Dual Timestamped Backups**: Automatically creates two independent backup copies every time a save is modified (global in `/_nds/pkmn-chest/backups/` and locally in `<save_dir>/backups/`).
* **Visual Ability Selector**: Added ability selection menu supporting primary, secondary, and Hidden Abilities (`[1]`, `[2]`, `[H]`), along with an alphabetical list of all abilities valid for the current generation.
* **Dynamic TM/HM Move Labels**: Items pouch now displays the move name taught by each TM/HM dynamically, matching the loaded generation and language.
* **Accent-Insensitive String Sorting**: Added `normalizeForSort` and `compareAlphabetical` for international search and sorting.
* **Self-Contained Docker Build System**: Pre-configured environment using BlocksDS v1.18.0 and `bmp2ds` for 1-command reproducible builds across Linux, macOS, and Windows.
* **Modern GitHub Actions CI/CD**: Automatic build on push/PR and release artifact publishing.

### Changed
* **Repository Architecture**: Flattened project tree into standard Nintendo DS homebrew structure.
* **ARM9 Memory Footprint Optimization**: Converted dynamic UI `std::vector` allocations to static `constexpr` arrays to prevent heap fragmentation.
* **Stripped Legacy Location Tables**: Removed unused Gen 6, 7, and 8 location text files from PKSM-Core, saving RAM on 4 MB NDS hardware.
* **Party Validation**: Calls `save->fixParty()` after party edits to maintain save data integrity.

### Fixed
* **Speed Sort Inversion**: Fixed an upstream issue where sorting by Speed compared Special Defense instead of Speed.
* **Touchscreen Coordinate Desync**: Fixed row touch calculation in Filter and Stats menus that referenced current cursor position rather than touched row index.
* **Integer Input Crash**: Replaced unchecked `std::stoi` with exception-handled `std::stoul` to prevent crashes when inputting large numbers.
* **Hidden Power Underflow**: Prevented out-of-bounds array access when checking or setting Hidden Power type.
* **Slot 1 Cartridge Header Alignment**: Added `alignas(4)` buffer for cartridge header reads.
* **Slot 2 Cartridge ID String**: Fixed null-termination bug on GBA cart identifier.
* **Resource Leaks**: Fixed missing `free()` and `fclose()` in favorites list parser.
