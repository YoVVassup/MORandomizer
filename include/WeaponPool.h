#pragma once

#include <vector>
#include <array>
#include <YRpp.h>

enum WeaponSlot : size_t {
    SLOT_PRIMARY = 0,
    SLOT_SECONDARY = 1,
    SLOT_ELITE_PRIMARY = 2,
    SLOT_ELITE_SECONDARY = 3,
    SLOT_OCCUPY = 4,
    SLOT_ELITE_OCCUPY = 5
};

struct WeaponPair {
    WeaponTypeClass* Normal;
    WeaponTypeClass* Elite;
};

struct WeaponSet {
    std::array<WeaponTypeClass*, 6> weapons = {nullptr};
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