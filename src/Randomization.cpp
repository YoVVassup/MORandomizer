#include "Randomization.h"
#include "Config.h"
#include "WeaponPool.h"
#include "Utilities.h"
#include "RandomizationHelpers.h"
#include <YRpp.h>
#include <InfantryClass.h>
#include <UnitClass.h>
#include <AircraftClass.h>
#include <BuildingClass.h>
#include <set>

// ============================================================
// Local randomization (selected units only)
// ============================================================
void RandomizeSelected() {
    LogDebug(">>> Local randomization started (S key) <<<");
    PlayBeep(1500, 100);
    SleepMs(100);

    std::set<InfantryTypeClass*> selectedInfTypes;
    std::set<UnitTypeClass*> selectedUnitTypes;
    std::set<AircraftTypeClass*> selectedAirTypes;
    std::set<BuildingTypeClass*> selectedBldTypes;

    if (randInfantry && SafeInfWeapons.size() > 0) {
        for (int i = 0; i < InfantryClass::Array.Count; i++) {
            InfantryClass* pObj = InfantryClass::Array.Items[i];
            if (pObj && pObj->IsSelected && pObj->Type && !IsWhitelisted(pObj->Type->ID))
                selectedInfTypes.insert(pObj->Type);
        }
    }
    if (randUnits && SafeUnitWeapons.size() > 0) {
        for (int i = 0; i < UnitClass::Array.Count; i++) {
            UnitClass* pObj = UnitClass::Array.Items[i];
            if (pObj && pObj->IsSelected && pObj->Type && !IsWhitelisted(pObj->Type->ID))
                selectedUnitTypes.insert(pObj->Type);
        }
    }
    if (randAircraft && SafeAircraftWeapons.size() > 0) {
        for (int i = 0; i < AircraftClass::Array.Count; i++) {
            AircraftClass* pObj = AircraftClass::Array.Items[i];
            if (pObj && pObj->IsSelected && pObj->Type && !IsWhitelisted(pObj->Type->ID))
                selectedAirTypes.insert(pObj->Type);
        }
    }
    if (randBuildings && SafeBuildingWeapons.size() > 0) {
        for (int i = 0; i < BuildingClass::Array.Count; i++) {
            BuildingClass* pObj = BuildingClass::Array.Items[i];
            if (pObj && pObj->IsSelected && pObj->Type && !IsWhitelisted(pObj->Type->ID))
                selectedBldTypes.insert(pObj->Type);
        }
    }

    // Apply randomization to each type
    RandomizeUnitType(selectedInfTypes, BackupInfWeapons, SafeInfWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedUnitTypes, BackupUnitWeapons, SafeUnitWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedAirTypes, BackupAircraftWeapons, SafeAircraftWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedBldTypes, BackupBuildingWeapons, SafeBuildingWeapons, GlobalChaosPool, isChaosToggleOn && includeBldInChaos);

    LogDebug("<<< Local randomization done <<<");
    SleepMs(300);
}

// ============================================================
// Global randomization within one class (W)
// ============================================================
void RandomizeGlobalSameClass() {
    LogDebug(">>> Global same-class shuffle started (W key) <<<");
    PlayBeep(2000, 100);
    SleepMs(100);

    if (randInfantry)
        RandomizeGlobalSameClassForType(InfantryTypeClass::Array, BackupInfWeapons, SafeInfWeapons, SafeOccupyWeapons);
    if (randUnits)
        RandomizeGlobalSameClassForType(UnitTypeClass::Array, BackupUnitWeapons, SafeUnitWeapons, SafeOccupyWeapons);
    if (randAircraft)
        RandomizeGlobalSameClassForType(AircraftTypeClass::Array, BackupAircraftWeapons, SafeAircraftWeapons, SafeOccupyWeapons);
    if (randBuildings)
        RandomizeGlobalSameClassForType(BuildingTypeClass::Array, BackupBuildingWeapons, SafeBuildingWeapons, SafeOccupyWeapons);

    LogDebug("<<< Global same-class shuffle done <<<");
    SleepMs(300);
}

// ============================================================
// Global chaotic randomization (C)
// ============================================================
void RandomizeGlobalChaos() {
    LogDebug(">>> Global cross-class chaos started (C key) <<<");
    PlayBeep(2500, 150);
    SleepMs(100);

    if (GlobalChaosPool.empty()) return;

    if (randInfantry)
        RandomizeGlobalChaosForType(InfantryTypeClass::Array, BackupInfWeapons, GlobalChaosPool, SafeBuildingWeapons, includeBldInChaos);
    if (randUnits)
        RandomizeGlobalChaosForType(UnitTypeClass::Array, BackupUnitWeapons, GlobalChaosPool, SafeBuildingWeapons, includeBldInChaos);
    if (randAircraft)
        RandomizeGlobalChaosForType(AircraftTypeClass::Array, BackupAircraftWeapons, GlobalChaosPool, SafeBuildingWeapons, includeBldInChaos);
    if (randBuildings)
        RandomizeGlobalChaosForType(BuildingTypeClass::Array, BackupBuildingWeapons, GlobalChaosPool, SafeBuildingWeapons, includeBldInChaos);

    LogDebug("<<< Global cross-class chaos done <<<");
    SleepMs(300);
}

// ============================================================
// Restoring original weapons (R)
// ============================================================
void RestoreOriginal() {
    LogDebug(">>> Restoring original weapons (R key) <<<");
    PlayBeep(1000, 200);
    SleepMs(100);

    // Infantry
    for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupInfWeapons.size()) && InfantryTypeClass::Array.Items[i]) {
            auto& p = InfantryTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupInfWeapons[i].weapons[SLOT_PRIMARY];
            p->Weapon[1].WeaponType = BackupInfWeapons[i].weapons[SLOT_SECONDARY];
            p->EliteWeapon[0].WeaponType = BackupInfWeapons[i].weapons[SLOT_ELITE_PRIMARY];
            p->EliteWeapon[1].WeaponType = BackupInfWeapons[i].weapons[SLOT_ELITE_SECONDARY];
            p->OccupyWeapon.WeaponType = BackupInfWeapons[i].weapons[SLOT_OCCUPY];
            p->EliteOccupyWeapon.WeaponType = BackupInfWeapons[i].weapons[SLOT_ELITE_OCCUPY];
        }
    }
    // Units
    for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupUnitWeapons.size()) && UnitTypeClass::Array.Items[i]) {
            auto& p = UnitTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupUnitWeapons[i].weapons[SLOT_PRIMARY];
            p->Weapon[1].WeaponType = BackupUnitWeapons[i].weapons[SLOT_SECONDARY];
            p->EliteWeapon[0].WeaponType = BackupUnitWeapons[i].weapons[SLOT_ELITE_PRIMARY];
            p->EliteWeapon[1].WeaponType = BackupUnitWeapons[i].weapons[SLOT_ELITE_SECONDARY];
        }
    }
    // Aircraft
    for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupAircraftWeapons.size()) && AircraftTypeClass::Array.Items[i]) {
            auto& p = AircraftTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupAircraftWeapons[i].weapons[SLOT_PRIMARY];
            p->Weapon[1].WeaponType = BackupAircraftWeapons[i].weapons[SLOT_SECONDARY];
            p->EliteWeapon[0].WeaponType = BackupAircraftWeapons[i].weapons[SLOT_ELITE_PRIMARY];
            p->EliteWeapon[1].WeaponType = BackupAircraftWeapons[i].weapons[SLOT_ELITE_SECONDARY];
        }
    }
    // Buildings
    for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupBuildingWeapons.size()) && BuildingTypeClass::Array.Items[i]) {
            auto& p = BuildingTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupBuildingWeapons[i].weapons[SLOT_PRIMARY];
            p->Weapon[1].WeaponType = BackupBuildingWeapons[i].weapons[SLOT_SECONDARY];
            p->EliteWeapon[0].WeaponType = BackupBuildingWeapons[i].weapons[SLOT_ELITE_PRIMARY];
            p->EliteWeapon[1].WeaponType = BackupBuildingWeapons[i].weapons[SLOT_ELITE_SECONDARY];
        }
    }

    LogDebug("<<< Restore completed <<<");
    SleepMs(500);
}