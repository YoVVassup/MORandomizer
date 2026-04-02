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

    // Используем шаблонную функцию для каждого типа
    RandomizeUnitType(selectedInfTypes, BackupInfWeapons, SafeInfWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedUnitTypes, BackupUnitWeapons, SafeUnitWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedAirTypes, BackupAircraftWeapons, SafeAircraftWeapons, GlobalChaosPool, isChaosToggleOn);
    RandomizeUnitType(selectedBldTypes, BackupBuildingWeapons, SafeBuildingWeapons, GlobalChaosPool, isChaosToggleOn && includeBldInChaos, true);

    LogDebug("<<< Local randomization done <<<");
    SleepMs(300);
}

void RandomizeGlobalSameClass() {
    LogDebug(">>> Global same-class shuffle started (W key) <<<");
    PlayBeep(2000, 100);
    SleepMs(100);

    int infPoolSize = static_cast<int>(SafeInfWeapons.size());
    int unitPoolSize = static_cast<int>(SafeUnitWeapons.size());
    int airPoolSize = static_cast<int>(SafeAircraftWeapons.size());
    int bldPoolSize = static_cast<int>(SafeBuildingWeapons.size());
    int occPoolSize = static_cast<int>(SafeOccupyWeapons.size());

    if (randInfantry && infPoolSize > 0) {
        for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
            InfantryTypeClass* pInf = InfantryTypeClass::Array.Items[i];
            if (pInf && !IsWhitelisted(pInf->ID) && i < static_cast<int>(BackupInfWeapons.size())) {
                const auto& backup = BackupInfWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = SafeInfWeapons[GetRandomInt(infPoolSize)];
                    pInf->Weapon[0].WeaponType = wp.Normal;
                    pInf->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Global] Infantry %s primary -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = SafeInfWeapons[GetRandomInt(infPoolSize)];
                    pInf->Weapon[1].WeaponType = wp.Normal;
                    pInf->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Global] Infantry %s secondary -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Occ && occPoolSize > 0) {
                    WeaponPair wp = SafeOccupyWeapons[GetRandomInt(occPoolSize)];
                    pInf->OccupyWeapon.WeaponType = wp.Normal;
                    pInf->EliteOccupyWeapon.WeaponType = wp.Elite;
                    LogDebug("[Global] Infantry %s occupant -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randUnits && unitPoolSize > 0) {
        for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
            UnitTypeClass* pUnit = UnitTypeClass::Array.Items[i];
            if (pUnit && !IsWhitelisted(pUnit->ID) && i < static_cast<int>(BackupUnitWeapons.size())) {
                const auto& backup = BackupUnitWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = SafeUnitWeapons[GetRandomInt(unitPoolSize)];
                    pUnit->Weapon[0].WeaponType = wp.Normal;
                    pUnit->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Global] Unit %s primary -> %s | %s", pUnit->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = SafeUnitWeapons[GetRandomInt(unitPoolSize)];
                    pUnit->Weapon[1].WeaponType = wp.Normal;
                    pUnit->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Global] Unit %s secondary -> %s | %s", pUnit->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randAircraft && airPoolSize > 0) {
        for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
            AircraftTypeClass* pAir = AircraftTypeClass::Array.Items[i];
            if (pAir && !IsWhitelisted(pAir->ID) && i < static_cast<int>(BackupAircraftWeapons.size())) {
                const auto& backup = BackupAircraftWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = SafeAircraftWeapons[GetRandomInt(airPoolSize)];
                    pAir->Weapon[0].WeaponType = wp.Normal;
                    pAir->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Global] Aircraft %s primary -> %s | %s", pAir->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = SafeAircraftWeapons[GetRandomInt(airPoolSize)];
                    pAir->Weapon[1].WeaponType = wp.Normal;
                    pAir->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Global] Aircraft %s secondary -> %s | %s", pAir->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randBuildings && bldPoolSize > 0) {
        for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
            BuildingTypeClass* pBld = BuildingTypeClass::Array.Items[i];
            if (pBld && !IsWhitelisted(pBld->ID) && i < static_cast<int>(BackupBuildingWeapons.size())) {
                const auto& backup = BackupBuildingWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = SafeBuildingWeapons[GetRandomInt(bldPoolSize)];
                    pBld->Weapon[0].WeaponType = wp.Normal;
                    pBld->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Global] Building %s primary -> %s | %s", pBld->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = SafeBuildingWeapons[GetRandomInt(bldPoolSize)];
                    pBld->Weapon[1].WeaponType = wp.Normal;
                    pBld->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Global] Building %s secondary -> %s | %s", pBld->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    LogDebug("<<< Global same-class shuffle done <<<");
    SleepMs(300);
}

void RandomizeGlobalChaos() {
    LogDebug(">>> Global cross-class chaos started (C key) <<<");
    PlayBeep(2500, 150);
    SleepMs(100);

    int chaosPoolSize = static_cast<int>(GlobalChaosPool.size());
    if (chaosPoolSize == 0) return;

    int bldSafePoolSize = static_cast<int>(SafeBuildingWeapons.size());

    if (randInfantry) {
        for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
            InfantryTypeClass* pInf = InfantryTypeClass::Array.Items[i];
            if (pInf && !IsWhitelisted(pInf->ID) && i < static_cast<int>(BackupInfWeapons.size())) {
                const auto& backup = BackupInfWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pInf->Weapon[0].WeaponType = wp.Normal;
                    pInf->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Infantry %s primary -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pInf->Weapon[1].WeaponType = wp.Normal;
                    pInf->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Infantry %s secondary -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Occ) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pInf->OccupyWeapon.WeaponType = wp.Normal;
                    pInf->EliteOccupyWeapon.WeaponType = wp.Elite;
                    LogDebug("[Chaos] Infantry %s occupant -> %s | %s", pInf->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randUnits) {
        for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
            UnitTypeClass* pUnit = UnitTypeClass::Array.Items[i];
            if (pUnit && !IsWhitelisted(pUnit->ID) && i < static_cast<int>(BackupUnitWeapons.size())) {
                const auto& backup = BackupUnitWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pUnit->Weapon[0].WeaponType = wp.Normal;
                    pUnit->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Unit %s primary -> %s | %s", pUnit->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pUnit->Weapon[1].WeaponType = wp.Normal;
                    pUnit->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Unit %s secondary -> %s | %s", pUnit->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randAircraft) {
        for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
            AircraftTypeClass* pAir = AircraftTypeClass::Array.Items[i];
            if (pAir && !IsWhitelisted(pAir->ID) && i < static_cast<int>(BackupAircraftWeapons.size())) {
                const auto& backup = BackupAircraftWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pAir->Weapon[0].WeaponType = wp.Normal;
                    pAir->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Aircraft %s primary -> %s | %s", pAir->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    pAir->Weapon[1].WeaponType = wp.Normal;
                    pAir->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Aircraft %s secondary -> %s | %s", pAir->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    if (randBuildings) {
        for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
            BuildingTypeClass* pBld = BuildingTypeClass::Array.Items[i];
            if (pBld && !IsWhitelisted(pBld->ID) && i < static_cast<int>(BackupBuildingWeapons.size())) {
                const auto& backup = BackupBuildingWeapons[i];
                if (backup.Pri) {
                    WeaponPair wp;
                    if (includeBldInChaos)
                        wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    else
                        wp = SafeBuildingWeapons[GetRandomInt(bldSafePoolSize)];
                    pBld->Weapon[0].WeaponType = wp.Normal;
                    pBld->EliteWeapon[0].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Building %s primary -> %s | %s", pBld->ID, wp.Normal->ID, wp.Elite->ID);
                }
                if (backup.Sec) {
                    WeaponPair wp;
                    if (includeBldInChaos)
                        wp = GlobalChaosPool[GetRandomInt(chaosPoolSize)];
                    else
                        wp = SafeBuildingWeapons[GetRandomInt(bldSafePoolSize)];
                    pBld->Weapon[1].WeaponType = wp.Normal;
                    pBld->EliteWeapon[1].WeaponType = wp.Elite;
                    LogDebug("[Chaos] Building %s secondary -> %s | %s", pBld->ID, wp.Normal->ID, wp.Elite->ID);
                }
            }
        }
    }

    LogDebug("<<< Global cross-class chaos done <<<");
    SleepMs(300);
}

void RestoreOriginal() {
    LogDebug(">>> Restoring original weapons (R key) <<<");
    PlayBeep(1000, 200);
    SleepMs(100);

    for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupInfWeapons.size()) && InfantryTypeClass::Array.Items[i]) {
            auto& p = InfantryTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupInfWeapons[i].Pri;
            p->Weapon[1].WeaponType = BackupInfWeapons[i].Sec;
            p->EliteWeapon[0].WeaponType = BackupInfWeapons[i].EPri;
            p->EliteWeapon[1].WeaponType = BackupInfWeapons[i].ESec;
            p->OccupyWeapon.WeaponType = BackupInfWeapons[i].Occ;
            p->EliteOccupyWeapon.WeaponType = BackupInfWeapons[i].EOcc;
        }
    }
    for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupUnitWeapons.size()) && UnitTypeClass::Array.Items[i]) {
            auto& p = UnitTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupUnitWeapons[i].Pri;
            p->Weapon[1].WeaponType = BackupUnitWeapons[i].Sec;
            p->EliteWeapon[0].WeaponType = BackupUnitWeapons[i].EPri;
            p->EliteWeapon[1].WeaponType = BackupUnitWeapons[i].ESec;
        }
    }
    for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupAircraftWeapons.size()) && AircraftTypeClass::Array.Items[i]) {
            auto& p = AircraftTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupAircraftWeapons[i].Pri;
            p->Weapon[1].WeaponType = BackupAircraftWeapons[i].Sec;
            p->EliteWeapon[0].WeaponType = BackupAircraftWeapons[i].EPri;
            p->EliteWeapon[1].WeaponType = BackupAircraftWeapons[i].ESec;
        }
    }
    for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupBuildingWeapons.size()) && BuildingTypeClass::Array.Items[i]) {
            auto& p = BuildingTypeClass::Array.Items[i];
            p->Weapon[0].WeaponType = BackupBuildingWeapons[i].Pri;
            p->Weapon[1].WeaponType = BackupBuildingWeapons[i].Sec;
            p->EliteWeapon[0].WeaponType = BackupBuildingWeapons[i].EPri;
            p->EliteWeapon[1].WeaponType = BackupBuildingWeapons[i].ESec;
        }
    }

    LogDebug("<<< Restore completed <<<");
    SleepMs(500);
}