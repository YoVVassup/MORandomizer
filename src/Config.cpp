#include "Config.h"
#include "Utilities.h"
#include "LogManager.h"
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

bool randInfantry = true;
bool randUnits = true;
bool randAircraft = true;
bool randBuildings = true;
bool includeBldInChaos = false;

bool isChaosToggleOn = false;

std::vector<std::string> WhiteList;
std::vector<std::string> SuperWeaponBlacklist;

void LoadConfiguration() {
    char iniPath[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, iniPath);
    strcat(iniPath, "\\MORandomizer.ini");

    logFilePath = std::string(iniPath);
    size_t pos = logFilePath.find_last_of("\\/");
    logFilePath = logFilePath.substr(0, pos) + "\\MORandomizer_Debug.log";

    char buf[32];
    GetPrivateProfileStringA("Settings", "EnableBeep", "yes", buf, sizeof(buf), iniPath);
    enableBeeps = (_stricmp(Trim(buf).c_str(), "no") != 0);
    GetPrivateProfileStringA("Settings", "EnableDebugLog", "no", buf, sizeof(buf), iniPath);
    enableDebugLog = (_stricmp(Trim(buf).c_str(), "yes") == 0);

    GetPrivateProfileStringA("Settings", "RequireCtrl", "yes", buf, sizeof(buf), iniPath);
    requireCtrl = (_stricmp(Trim(buf).c_str(), "yes") == 0);
    GetPrivateProfileStringA("Settings", "RequireShift", "no", buf, sizeof(buf), iniPath);
    requireShift = (_stricmp(Trim(buf).c_str(), "yes") == 0);
    GetPrivateProfileStringA("Settings", "RequireAlt", "no", buf, sizeof(buf), iniPath);
    requireAlt = (_stricmp(Trim(buf).c_str(), "yes") == 0);

    GetPrivateProfileStringA("Settings", "KeyRandomWeapons", "W", buf, sizeof(buf), iniPath);
    if (!Trim(buf).empty()) vkRandomWeapons = std::toupper(static_cast<unsigned char>(Trim(buf)[0]));
    GetPrivateProfileStringA("Settings", "KeyChaosMode", "C", buf, sizeof(buf), iniPath);
    if (!Trim(buf).empty()) vkChaosMode = std::toupper(static_cast<unsigned char>(Trim(buf)[0]));
    GetPrivateProfileStringA("Settings", "KeySelectedRandom", "S", buf, sizeof(buf), iniPath);
    if (!Trim(buf).empty()) vkSelectedRandom = std::toupper(static_cast<unsigned char>(Trim(buf)[0]));
    GetPrivateProfileStringA("Settings", "KeyToggleChaos", "E", buf, sizeof(buf), iniPath);
    if (!Trim(buf).empty()) vkToggleChaosSelect = std::toupper(static_cast<unsigned char>(Trim(buf)[0]));
    GetPrivateProfileStringA("Settings", "KeyRestore", "R", buf, sizeof(buf), iniPath);
    if (!Trim(buf).empty()) vkRestore = std::toupper(static_cast<unsigned char>(Trim(buf)[0]));

    GetPrivateProfileStringA("Settings", "RandomizeInfantry", "yes", buf, sizeof(buf), iniPath);
    randInfantry = (_stricmp(Trim(buf).c_str(), "no") != 0);
    GetPrivateProfileStringA("Settings", "RandomizeUnits", "yes", buf, sizeof(buf), iniPath);
    randUnits = (_stricmp(Trim(buf).c_str(), "no") != 0);
    GetPrivateProfileStringA("Settings", "RandomizeAircraft", "yes", buf, sizeof(buf), iniPath);
    randAircraft = (_stricmp(Trim(buf).c_str(), "no") != 0);
    GetPrivateProfileStringA("Settings", "RandomizeBuildings", "yes", buf, sizeof(buf), iniPath);
    randBuildings = (_stricmp(Trim(buf).c_str(), "no") != 0);

    GetPrivateProfileStringA("Settings", "IncludeBuildingsInChaos", "no", buf, sizeof(buf), iniPath);
    includeBldInChaos = (_stricmp(Trim(buf).c_str(), "yes") == 0);

    if (enableDebugLog) {
        LogManager::Instance().Open(logFilePath);
        LogDebug("========== MORandomizer engine started ==========");
        LogDebug("Version: %s", MORANDOMIZER_VERSION_STRING);
    }

    char buffer[4096];
    memset(buffer, 0, sizeof(buffer));
    GetPrivateProfileSectionA("Whitelist", buffer, sizeof(buffer), iniPath);
    ParseListBuffer(buffer, WhiteList, "Unit Whitelist");

    memset(buffer, 0, sizeof(buffer));
    GetPrivateProfileSectionA("Blacklist", buffer, sizeof(buffer), iniPath);
    ParseListBuffer(buffer, SuperWeaponBlacklist, "Weapon Blacklist");
}