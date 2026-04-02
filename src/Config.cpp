#include "Config.h"
#include "Utilities.h"
#include "LogManager.h"
#include "WeaponPool.h"
#include <windows.h>
#include "Version.h"

bool enableBeeps = true;
bool enableDebugLog = false;
std::string logFilePath;

bool requireCtrl = true;
bool requireShift = false;
bool requireAlt = false;

int vkRandomWeapons = 'W';
int vkChaosMode = 'C';
int vkSelectedRandom = 'S';
int vkToggleChaosSelect = 'E';
int vkRestore = 'R';
int vkReloadConfig = VK_F5;

bool randInfantry = true;
bool randUnits = true;
bool randAircraft = true;
bool randBuildings = true;
bool includeBldInChaos = false;

bool isChaosToggleOn = false;
bool allowMultiplayer = false;

std::vector<std::string> WhiteList;
std::vector<std::string> SuperWeaponBlacklist;

// Helper function to read a string from INI
static std::string ReadIniString(const char* section, const char* key, const char* def, const char* iniPath) {
    char buf[256];
    GetPrivateProfileStringA(section, key, def, buf, sizeof(buf), iniPath);
    return Trim(buf);
}

void LoadConfiguration() {
    char iniPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, iniPath);
    strcat(iniPath, "\\MORandomizer.ini");

    logFilePath = std::string(iniPath);
    size_t pos = logFilePath.find_last_of("\\/");
    logFilePath = logFilePath.substr(0, pos) + "\\MORandomizer_Debug.log";

    // Reading settings
    enableBeeps        = (_stricmp(ReadIniString("Settings", "EnableBeep", "yes", iniPath).c_str(), "no") != 0);
    enableDebugLog     = (_stricmp(ReadIniString("Settings", "EnableDebugLog", "no", iniPath).c_str(), "yes") == 0);
    requireCtrl        = (_stricmp(ReadIniString("Settings", "RequireCtrl", "yes", iniPath).c_str(), "yes") == 0);
    requireShift       = (_stricmp(ReadIniString("Settings", "RequireShift", "no", iniPath).c_str(), "yes") == 0);
    requireAlt         = (_stricmp(ReadIniString("Settings", "RequireAlt", "no", iniPath).c_str(), "yes") == 0);
    allowMultiplayer   = (_stricmp(ReadIniString("Settings", "AllowMultiplayer", "no", iniPath).c_str(), "yes") == 0);

    std::string key = ReadIniString("Settings", "KeyRandomWeapons", "W", iniPath);
    if (!key.empty()) vkRandomWeapons = std::toupper(key[0]);
    key = ReadIniString("Settings", "KeyChaosMode", "C", iniPath);
    if (!key.empty()) vkChaosMode = std::toupper(key[0]);
    key = ReadIniString("Settings", "KeySelectedRandom", "S", iniPath);
    if (!key.empty()) vkSelectedRandom = std::toupper(key[0]);
    key = ReadIniString("Settings", "KeyToggleChaos", "E", iniPath);
    if (!key.empty()) vkToggleChaosSelect = std::toupper(key[0]);
    key = ReadIniString("Settings", "KeyRestore", "R", iniPath);
    if (!key.empty()) vkRestore = std::toupper(key[0]);
    key = ReadIniString("Settings", "KeyReloadConfig", "F5", iniPath);
    if (!key.empty() && key.length() == 1) {
        vkReloadConfig = std::toupper(key[0]);
    } else if (_stricmp(key.c_str(), "F5") == 0) {
        vkReloadConfig = VK_F5;
    }

    randInfantry       = (_stricmp(ReadIniString("Settings", "RandomizeInfantry", "yes", iniPath).c_str(), "no") != 0);
    randUnits          = (_stricmp(ReadIniString("Settings", "RandomizeUnits", "yes", iniPath).c_str(), "no") != 0);
    randAircraft       = (_stricmp(ReadIniString("Settings", "RandomizeAircraft", "yes", iniPath).c_str(), "no") != 0);
    randBuildings      = (_stricmp(ReadIniString("Settings", "RandomizeBuildings", "yes", iniPath).c_str(), "no") != 0);
    includeBldInChaos  = (_stricmp(ReadIniString("Settings", "IncludeBuildingsInChaos", "no", iniPath).c_str(), "yes") == 0);

    // White and black lists
    WhiteList.clear();
    SuperWeaponBlacklist.clear();
    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    GetPrivateProfileSectionA("Whitelist", buffer, sizeof(buffer), iniPath);
    ParseListBuffer(buffer, WhiteList, "Unit Whitelist");
    memset(buffer, 0, sizeof(buffer));
    GetPrivateProfileSectionA("Blacklist", buffer, sizeof(buffer), iniPath);
    ParseListBuffer(buffer, SuperWeaponBlacklist, "Weapon Blacklist");

    if (enableDebugLog) {
        LogManager::Instance().Open(logFilePath);
        LogDebug("========== MORandomizer engine started ==========");
        LogDebug("Version: %s", MORANDOMIZER_VERSION_STRING);
        LogDebug("Multiplayer allowed: %s", allowMultiplayer ? "yes" : "no");
    }
}

// Configuration reload function (without recreating backups)
void ReloadConfiguration() {
    LogDebug(">>> Reloading configuration... <<<");
    LoadConfiguration();
    isBackedUp = false;
    SafeInfWeapons.clear();
    SafeUnitWeapons.clear();
    SafeAircraftWeapons.clear();
    SafeBuildingWeapons.clear();
    SafeOccupyWeapons.clear();
    GlobalChaosPool.clear();
    BackupInfWeapons.clear();
    BackupUnitWeapons.clear();
    BackupAircraftWeapons.clear();
    BackupBuildingWeapons.clear();
    LogDebug("<<< Configuration reloaded. Pools will be rebuilt on next action. <<<");
    PlayBeep(1000, 200);
}