# MORandomizer (Mental Omega / Red Alert 2 Weapon Randomizer)

🎲 **MORandomizer** is a C++ memory injection extension for *Command & Conquer: Red Alert 2 – Mental Omega* (and vanilla Yuri's Revenge). It dynamically shuffles unit weapons at runtime, bypassing the game engine's limitations.

**Ever wanted a Conscript to fire a prism laser? Or a tank to drop a Kirov bomb? Now everything is possible!**

## ✨ Features

- **🛡️ Engine‑level Snapshot**: Automatically backs up all weapon slots (infantry, vehicles, aircraft, buildings) on first hotkey press. One‑key restore to factory settings.
- **🎁 Veteran‑ready Pairing**: Normal and elite (veteran) weapons are paired together. Units never lose their firepower when ranking up.
- **🌪️ Four Randomization Modes**:
  - *Global same‑class* – infantry get infantry weapons, vehicles get vehicle weapons.
  - *Cross‑class chaos* – break the rules: infantry can carry AA guns, tanks can use sniper rifles.
  - *Local selection* – only affects currently selected units (reads the engine’s `IsSelected` flag).
- **🔒 Safety Filters**: Hard‑coded checks reject dummy weapons (no projectile, no warhead, range ≤2.5). External INI‑based blacklist blocks dangerous patterns (e.g., airstrike beacons, chrono weapons, spawners) that cause `C0000005` crashes.
- **📝 Detailed Debug Logging**: Every weapon assignment is written to `MORandomizer_Debug.log` for easy troubleshooting.

## 🎮 Installation & Usage

### For Players
1. Download the latest release from the [Releases](../../releases) page.
2. Place `MORandomizer.dll` and `MORandomizer.ini` into your *Mental Omega* game folder (where `gamemd.exe` and `Syringe.exe` reside).
3. Ensure the DLL is loaded by Syringe, or let Syringe auto‑detect it (the DLL exports a hook at `0x7CD810`).
4. Launch the game via Syringe (e.g., double‑click `Syringe.exe` or use the game launcher).
5. In a match, hold the modifier keys (default `Ctrl + Shift`) and press:
   - `W` – Global same‑class shuffle
   - `C` – Global cross‑class chaos
   - `S` – Local shuffle (only selected units)
   - `E` – Toggle the behaviour of `S` (same‑class ↔ cross‑class)
   - `R` – Restore all original weapons

### ⚙️ Configuration (`MORandomizer.ini`)
The plugin is highly customisable. Edit the INI file to:
- Change hotkeys and modifier key requirements.
- Enable/disable randomization for infantry, vehicles, aircraft, or buildings.
- Add units to the `[Whitelist]` (they will never be affected).
- Extend the `[Blacklist]` with weapon name patterns that should be excluded from the pool.

## 🛠️ Building from Source

### Dependencies
- **Visual Studio** (2019 or 2022, with **x86** toolchain)
- **[YRpp](https://github.com/Phobos-developers/YRpp)** – header‑only library for YR memory reflection
- **Syringe** headers (provided with Ares)

### Steps
1. Clone the repository:
   ```bash
   git clone --recursive https://github.com/your-username/MORandomizer.git
   ```
   (The `--recursive` flag automatically fetches the YRpp submodule.)
2. Open the folder in CLion with CMake or open the generated Visual Studio solution.
3. Build the project in **Release | Win32** (the game is 32‑bit).
4. The output `MORandomizer.dll` will be placed in the `Release/` folder, along with a copy of `MORandomizer.ini`.

## ⚠️ Important Notes
- **Mental Omega 3.3.6** has heavily modified game logic. Although the plugin includes extensive crash protection, very frequent or massive shuffles during heavy combat may still trigger rare internal errors (IE / `C0000005`).
- Enable `EnableDebugLog=yes` in the INI to generate `MORandomizer_Debug.log`. If a crash occurs, check the last logged weapon assignment and add that weapon name to the `[Blacklist]`.
- The DLL is designed to be loaded by **Syringe** (the same injector used by Ares). No external launcher is required.

## 📄 License & Credits
This project is released under the **[GPL‑3.0 License](LICENSE)**. You may freely use, modify, and redistribute it, provided you share your changes under the same license and retain the original author credit.

**Special Thanks:**
- [YRpp](https://github.com/Phobos-developers/YRpp) – essential YR memory reflection library.
- Ares / Syringe – for providing a clean DLL injection framework.
- The Mental Omega team – for creating an incredible mod.