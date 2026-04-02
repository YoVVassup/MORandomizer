#include <windows.h>
#include <Syringe.h>
#include <YRpp.h>
#include <InfantryTypeClass.h>
#include "Config.h"
#include "WeaponPool.h"
#include "Randomization.h"
#include "Utilities.h"

void(__cdecl* g_DebugLog)(const char* pFormat, ...) = (void(__cdecl*)(const char*, ...))0x4068E0;

DWORD WINAPI GodHandThread(LPVOID lpParam) {
    while (InfantryTypeClass::Array.Count == 0) {
        Sleep(100);
    }

    LoadConfiguration();

    while (true) {
        bool modCtrl = requireCtrl ? (GetAsyncKeyState(VK_CONTROL) & 0x8000) : true;
        bool modShift = requireShift ? (GetAsyncKeyState(VK_SHIFT) & 0x8000) : true;
        bool modAlt = requireAlt ? (GetAsyncKeyState(VK_MENU) & 0x8000) : true;
        bool modsOk = modCtrl && modShift && modAlt;

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
                Sleep(300);
            }

            if (pressS) RandomizeSelected();
            if (pressW) RandomizeGlobalSameClass();
            if (pressC) RandomizeGlobalChaos();
            if (pressR) RestoreOriginal();
        }
        Sleep(30);
    }
    return 0;
}

DEFINE_HOOK(0x7CD810, MORandomizer_ExeRun, 0x9) {
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
    }
    return TRUE;
}