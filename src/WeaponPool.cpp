#include "WeaponPool.h"
#include "Config.h"
#include "Utilities.h"
#include <YRpp.h>
#include <InfantryTypeClass.h>
#include <UnitTypeClass.h>
#include <AircraftTypeClass.h>
#include <BuildingTypeClass.h>
#include <Offsets.h>

std::vector<WeaponPair> SafeInfWeapons;
std::vector<WeaponPair> SafeUnitWeapons;
std::vector<WeaponPair> SafeAircraftWeapons;
std::vector<WeaponPair> SafeBuildingWeapons;
std::vector<WeaponPair> SafeOccupyWeapons;
std::vector<WeaponPair> GlobalChaosPool;

std::vector<WeaponSet> BackupInfWeapons;
std::vector<WeaponSet> BackupUnitWeapons;
std::vector<WeaponSet> BackupAircraftWeapons;
std::vector<WeaponSet> BackupBuildingWeapons;
bool isBackedUp = false;

bool IsLegalWeapon(WeaponTypeClass* pWeapon) {
    if (!pWeapon || !pWeapon->ID) return false;

    if (!pWeapon->Projectile || !pWeapon->Warhead || pWeapon->Range <= Offsets::MinWeaponRange) return false;
    if (EndsWithCaseInsensitive(pWeapon->ID, "AI")) return false;

    std::string wUpper = pWeapon->ID;
    for (auto& c : wUpper) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

    if (wUpper.find("DUMMY") != std::string::npos || wUpper.find("FAKE") != std::string::npos ||
        wUpper.find("NOTA") != std::string::npos || wUpper.find("SUPPORT") != std::string::npos ||
        wUpper.find("START") != std::string::npos || wUpper.find("PATH") != std::string::npos ||
        wUpper.find("MINDCONTROL") != std::string::npos || wUpper.find("MINE") != std::string::npos ||
        wUpper.find("BOMB") != std::string::npos) return false;

    for (const auto& bw : SuperWeaponBlacklist) {
        std::string bwUpper = bw;
        for (auto& c : bwUpper) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        if (wUpper.find(bwUpper) != std::string::npos) return false;
    }
    return true;
}

bool IsWhitelisted(const char* id) {
    if (!id) return false;
    for (const auto& w : WhiteList) {
        if (_stricmp(id, w.c_str()) == 0) return true;
    }
    return false;
}

void TryAddPair(std::vector<WeaponPair>& pool, WeaponTypeClass* norm, WeaponTypeClass* elite) {
    if (IsLegalWeapon(norm)) {
        WeaponPair wp;
        wp.Normal = norm;
        wp.Elite = (elite && IsLegalWeapon(elite)) ? elite : norm;
        pool.push_back(wp);
    }
}

void BuildBackupsAndPools() {
    if (InfantryTypeClass::Array.Count == 0) return;

    // Reserve memory to reduce re-allocations
    BackupInfWeapons.reserve(InfantryTypeClass::Array.Count);
    BackupUnitWeapons.reserve(UnitTypeClass::Array.Count);
    BackupAircraftWeapons.reserve(AircraftTypeClass::Array.Count);
    BackupBuildingWeapons.reserve(BuildingTypeClass::Array.Count);

    SafeInfWeapons.reserve(InfantryTypeClass::Array.Count * 2);
    SafeUnitWeapons.reserve(UnitTypeClass::Array.Count * 2);
    SafeAircraftWeapons.reserve(AircraftTypeClass::Array.Count * 2);
    SafeBuildingWeapons.reserve(BuildingTypeClass::Array.Count * 2);
    SafeOccupyWeapons.reserve(InfantryTypeClass::Array.Count);
    GlobalChaosPool.reserve(SafeInfWeapons.capacity() + SafeUnitWeapons.capacity() +
                            SafeAircraftWeapons.capacity() + SafeBuildingWeapons.capacity());

    // Infantry
    for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
        InfantryTypeClass* pInf = InfantryTypeClass::Array.Items[i];
        WeaponSet ws;
        if (pInf) {
            ws.weapons[SLOT_PRIMARY] = pInf->Weapon[0].WeaponType;
            ws.weapons[SLOT_SECONDARY] = pInf->Weapon[1].WeaponType;
            ws.weapons[SLOT_ELITE_PRIMARY] = pInf->EliteWeapon[0].WeaponType;
            ws.weapons[SLOT_ELITE_SECONDARY] = pInf->EliteWeapon[1].WeaponType;
            ws.weapons[SLOT_OCCUPY] = pInf->OccupyWeapon.WeaponType;
            ws.weapons[SLOT_ELITE_OCCUPY] = pInf->EliteOccupyWeapon.WeaponType;
            TryAddPair(SafeInfWeapons, ws.weapons[SLOT_PRIMARY], ws.weapons[SLOT_ELITE_PRIMARY]);
            TryAddPair(SafeInfWeapons, ws.weapons[SLOT_SECONDARY], ws.weapons[SLOT_ELITE_SECONDARY]);
            TryAddPair(SafeOccupyWeapons, ws.weapons[SLOT_OCCUPY], ws.weapons[SLOT_ELITE_OCCUPY]);
        }
        BackupInfWeapons.push_back(ws);
    }

    // Units
    for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
        UnitTypeClass* pUnit = UnitTypeClass::Array.Items[i];
        WeaponSet ws;
        if (pUnit) {
            ws.weapons[SLOT_PRIMARY] = pUnit->Weapon[0].WeaponType;
            ws.weapons[SLOT_SECONDARY] = pUnit->Weapon[1].WeaponType;
            ws.weapons[SLOT_ELITE_PRIMARY] = pUnit->EliteWeapon[0].WeaponType;
            ws.weapons[SLOT_ELITE_SECONDARY] = pUnit->EliteWeapon[1].WeaponType;
            TryAddPair(SafeUnitWeapons, ws.weapons[SLOT_PRIMARY], ws.weapons[SLOT_ELITE_PRIMARY]);
            TryAddPair(SafeUnitWeapons, ws.weapons[SLOT_SECONDARY], ws.weapons[SLOT_ELITE_SECONDARY]);
        }
        BackupUnitWeapons.push_back(ws);
    }

    // Aircraft
    for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
        AircraftTypeClass* pAir = AircraftTypeClass::Array.Items[i];
        WeaponSet ws;
        if (pAir) {
            ws.weapons[SLOT_PRIMARY] = pAir->Weapon[0].WeaponType;
            ws.weapons[SLOT_SECONDARY] = pAir->Weapon[1].WeaponType;
            ws.weapons[SLOT_ELITE_PRIMARY] = pAir->EliteWeapon[0].WeaponType;
            ws.weapons[SLOT_ELITE_SECONDARY] = pAir->EliteWeapon[1].WeaponType;
            TryAddPair(SafeAircraftWeapons, ws.weapons[SLOT_PRIMARY], ws.weapons[SLOT_ELITE_PRIMARY]);
            TryAddPair(SafeAircraftWeapons, ws.weapons[SLOT_SECONDARY], ws.weapons[SLOT_ELITE_SECONDARY]);
        }
        BackupAircraftWeapons.push_back(ws);
    }

    // Buildings
    for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
        BuildingTypeClass* pBld = BuildingTypeClass::Array.Items[i];
        WeaponSet ws;
        if (pBld) {
            ws.weapons[SLOT_PRIMARY] = pBld->Weapon[0].WeaponType;
            ws.weapons[SLOT_SECONDARY] = pBld->Weapon[1].WeaponType;
            ws.weapons[SLOT_ELITE_PRIMARY] = pBld->EliteWeapon[0].WeaponType;
            ws.weapons[SLOT_ELITE_SECONDARY] = pBld->EliteWeapon[1].WeaponType;
            TryAddPair(SafeBuildingWeapons, ws.weapons[SLOT_PRIMARY], ws.weapons[SLOT_ELITE_PRIMARY]);
            TryAddPair(SafeBuildingWeapons, ws.weapons[SLOT_SECONDARY], ws.weapons[SLOT_ELITE_SECONDARY]);
        }
        BackupBuildingWeapons.push_back(ws);
    }

    // Global chaos pool
    GlobalChaosPool.insert(GlobalChaosPool.end(), SafeInfWeapons.begin(), SafeInfWeapons.end());
    GlobalChaosPool.insert(GlobalChaosPool.end(), SafeUnitWeapons.begin(), SafeUnitWeapons.end());
    GlobalChaosPool.insert(GlobalChaosPool.end(), SafeAircraftWeapons.begin(), SafeAircraftWeapons.end());
    GlobalChaosPool.insert(GlobalChaosPool.end(), SafeBuildingWeapons.begin(), SafeBuildingWeapons.end());

    isBackedUp = true;
    LogDebug("<<< Backup completed >>>");
    PlayBeep(800, 300);
    SleepMs(200);
}