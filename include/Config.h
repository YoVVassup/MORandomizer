#pragma once

#include <string>
#include <vector>

// Глобальные настройки
extern bool enableBeeps;
extern bool enableDebugLog;
extern std::string logFilePath;

extern bool requireCtrl;
extern bool requireShift;
extern bool requireAlt;

extern int vkRandomWeapons;
extern int vkChaosMode;
extern int vkSelectedRandom;
extern int vkToggleChaosSelect;
extern int vkRestore;
extern int vkReloadConfig;

extern bool randInfantry;
extern bool randUnits;
extern bool randAircraft;
extern bool randBuildings;
extern bool includeBldInChaos;

extern bool isChaosToggleOn;
extern bool allowMultiplayer;

extern std::vector<std::string> WhiteList;
extern std::vector<std::string> SuperWeaponBlacklist;

void LoadConfiguration();
void ReloadConfiguration();