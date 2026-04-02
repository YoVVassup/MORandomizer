#include <windows.h>
#include <Syringe.h>
#include <YRpp.h>
#include <InfantryTypeClass.h>
#include <HouseClass.h>
#include "Offsets.h"
#include "Config.h"
#include "WeaponPool.h"
#include "Randomization.h"
#include "Utilities.h"
#include "LogManager.h"

void(__cdecl* g_DebugLog)(const char* pFormat, ...) = (void(__cdecl*)(const char*, ...))DEBUG_LOG;

// Check: the game is in multiplayer and whether there are real players (not bots)
static bool IsMultiplayerWithRealPlayers() {
    int humanPlayers = 0;
    for (int i = 0; i < HouseClass::Array.Count; ++i) {
        HouseClass* pHouse = HouseClass::Array.Items[i];
        if (pHouse && pHouse->IsHumanPlayer) humanPlayers++;
    }
    // If there are at least two real players, we consider it multiplayer with people
    return (humanPlayers >= 2);
}

DWORD WINAPI GodHandThread(LPVOID lpParam) {
    while (InfantryTypeClass::Array.Count == 0) {
        SleepMs(100);
    }

    LoadConfiguration();

    // Checking multiplayer at startup
    if (IsMultiplayerWithRealPlayers() && !allowMultiplayer) {
        LogDebug("Multiplayer with real players detected and randomization is disabled (AllowMultiplayer=no).");
        return 0;
    }

    while (true) {
        bool multiplayerBlock = IsMultiplayerWithRealPlayers() && !allowMultiplayer;

        bool modCtrl = requireCtrl ? (GetAsyncKeyState(VK_CONTROL) & 0x8000) : true;
        bool modShift = requireShift ? (GetAsyncKeyState(VK_SHIFT) & 0x8000) : true;
        bool modAlt = requireAlt ? (GetAsyncKeyState(VK_MENU) & 0x8000) : true;
        bool modsOk = modCtrl && modShift && modAlt;

        bool reloadPressed = modsOk && (GetAsyncKeyState(vkReloadConfig) & 0x8000);
        if (reloadPressed && !multiplayerBlock) {
            ReloadConfiguration();
            SleepMs(500);
        }

        if (multiplayerBlock) {
            SleepMs(100);
            continue;
        }

        bool pressW = modsOk && (GetAsyncKeyState(vkRandomWeapons) & 0x8000);
        bool pressC = modsOk && (GetAsyncKeyState(vkChaosMode) & 0x8000);
        bool pressS = modsOk && (GetAsyncKeyState(vkSelectedRandom) & 0x8000);
        bool pressE = modsOk && (GetAsyncKeyState(vkToggleChaosSelect) & 0x8000);
        bool pressR = modsOk && (GetAsyncKeyState(vkRestore) & 0x8000);

        if (!isBackedUp && (pressW || pressC || pressS || pressE || pressR)) {
            BuildBackupsAndPools();
        }

        if (isBackedUp) {
            if (pressE) {
                isChaosToggleOn = !isChaosToggleOn;
                if (isChaosToggleOn) {
                    PlayBeep(2000, 100); PlayBeep(2500, 100);
                    LogDebug(">>> E toggled ON: S-key uses cross-class pool <<<");
                } else {
                    PlayBeep(2500, 100); PlayBeep(2000, 100);
                    LogDebug(">>> E toggled OFF: S-key uses class-specific pool <<<");
                }
                SleepMs(300);
            }

            if (pressS) RandomizeSelected();
            if (pressW) RandomizeGlobalSameClass();
            if (pressC) RandomizeGlobalChaos();
            if (pressR) RestoreOriginal();
        }
        SleepMs(30);
    }
    return 0;
}

DEFINE_HOOK(EXE_RUN, MORandomizer_ExeRun, 0x9) {
    static bool threadStarted = false;
    if (!threadStarted) {
        threadStarted = true;
        g_DebugLog("[MORandomizer] Starting GodHandThread...\n");
        CreateThread(nullptr, 0, GodHandThread, nullptr, 0, nullptr);
    }
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        // Nothing
    } else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
        LogManager::Instance().Close();
    }
    return TRUE;
}