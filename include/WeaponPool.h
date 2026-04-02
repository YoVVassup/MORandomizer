#pragma once

#include <vector>
#include <YRpp.h>

struct WeaponPair {
    WeaponTypeClass* Normal;
    WeaponTypeClass* Elite;
};

struct WeaponSet {
    WeaponTypeClass* Pri;   WeaponTypeClass* Sec;
    WeaponTypeClass* EPri;  WeaponTypeClass* ESec;
    WeaponTypeClass* Occ;   WeaponTypeClass* EOcc;
};

// Global pools
extern std::vector<WeaponPair> SafeInfWeapons;
extern std::vector<WeaponPair> SafeUnitWeapons;
extern std::vector<WeaponPair> SafeAircraftWeapons;
extern std::vector<WeaponPair> SafeBuildingWeapons;
extern std::vector<WeaponPair> SafeOccupyWeapons;
extern std::vector<WeaponPair> GlobalChaosPool;

// Backups of original weapons
extern std::vector<WeaponSet> BackupInfWeapons;
extern std::vector<WeaponSet> BackupUnitWeapons;
extern std::vector<WeaponSet> BackupAircraftWeapons;
extern std::vector<WeaponSet> BackupBuildingWeapons;
extern bool isBackedUp;

// Functions
bool IsLegalWeapon(WeaponTypeClass* pWeapon);
bool IsWhitelisted(const char* id);
void TryAddPair(std::vector<WeaponPair>& pool, WeaponTypeClass* norm, WeaponTypeClass* elite);
void BuildBackupsAndPools();