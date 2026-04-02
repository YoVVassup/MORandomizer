#include "Randomization.h"
#include "Config.h"
#include "WeaponPool.h"
#include "Utilities.h"
#include <YRpp.h>
#include <InfantryClass.h>
#include <UnitClass.h>
#include <AircraftClass.h>
#include <BuildingClass.h>
#include <set>

void RandomizeSelected() {
    LogDebug(">>> Local randomization started (S key) <<<");
    PlayBeep(1500, 100);

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

    // Infantry
    for (auto pInfType : selectedInfTypes) {
        int typeIdx = -1;
        for (int idx = 0; idx < InfantryTypeClass::Array.Count; ++idx) {
            if (InfantryTypeClass::Array.Items[idx] == pInfType) { typeIdx = idx; break; }
        }
        if (typeIdx != -1 && typeIdx < static_cast<int>(BackupInfWeapons.size())) {
            if (BackupInfWeapons[typeIdx].Pri) {
                WeaponPair wp1 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeInfWeapons[GetRandomInt(static_cast<int>(SafeInfWeapons.size()))];
                pInfType->Weapon[0].WeaponType = wp1.Normal;
                pInfType->EliteWeapon[0].WeaponType = wp1.Elite;
                LogDebug("[Local] Infantry %s primary -> %s | %s", pInfType->ID, wp1.Normal->ID, wp1.Elite->ID);
            }
            if (BackupInfWeapons[typeIdx].Sec) {
                WeaponPair wp2 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeInfWeapons[GetRandomInt(static_cast<int>(SafeInfWeapons.size()))];
                pInfType->Weapon[1].WeaponType = wp2.Normal;
                pInfType->EliteWeapon[1].WeaponType = wp2.Elite;
                LogDebug("[Local] Infantry %s secondary -> %s | %s", pInfType->ID, wp2.Normal->ID, wp2.Elite->ID);
            }
            if (BackupInfWeapons[typeIdx].Occ && SafeOccupyWeapons.size() > 0) {
                WeaponPair wp3 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeOccupyWeapons[GetRandomInt(static_cast<int>(SafeOccupyWeapons.size()))];
                pInfType->OccupyWeapon.WeaponType = wp3.Normal;
                pInfType->EliteOccupyWeapon.WeaponType = wp3.Elite;
                LogDebug("[Local] Infantry %s occupant -> %s | %s", pInfType->ID, wp3.Normal->ID, wp3.Elite->ID);
            }
        }
    }

    // Units
    for (auto pUnitType : selectedUnitTypes) {
        int typeIdx = -1;
        for (int idx = 0; idx < UnitTypeClass::Array.Count; ++idx) {
            if (UnitTypeClass::Array.Items[idx] == pUnitType) { typeIdx = idx; break; }
        }
        if (typeIdx != -1 && typeIdx < static_cast<int>(BackupUnitWeapons.size())) {
            if (BackupUnitWeapons[typeIdx].Pri) {
                WeaponPair wp1 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeUnitWeapons[GetRandomInt(static_cast<int>(SafeUnitWeapons.size()))];
                pUnitType->Weapon[0].WeaponType = wp1.Normal;
                pUnitType->EliteWeapon[0].WeaponType = wp1.Elite;
                LogDebug("[Local] Unit %s primary -> %s | %s", pUnitType->ID, wp1.Normal->ID, wp1.Elite->ID);
            }
            if (BackupUnitWeapons[typeIdx].Sec) {
                WeaponPair wp2 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeUnitWeapons[GetRandomInt(static_cast<int>(SafeUnitWeapons.size()))];
                pUnitType->Weapon[1].WeaponType = wp2.Normal;
                pUnitType->EliteWeapon[1].WeaponType = wp2.Elite;
                LogDebug("[Local] Unit %s secondary -> %s | %s", pUnitType->ID, wp2.Normal->ID, wp2.Elite->ID);
            }
        }
    }

    // Aircraft
    for (auto pAirType : selectedAirTypes) {
        int typeIdx = -1;
        for (int idx = 0; idx < AircraftTypeClass::Array.Count; ++idx) {
            if (AircraftTypeClass::Array.Items[idx] == pAirType) { typeIdx = idx; break; }
        }
        if (typeIdx != -1 && typeIdx < static_cast<int>(BackupAircraftWeapons.size())) {
            if (BackupAircraftWeapons[typeIdx].Pri) {
                WeaponPair wp1 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeAircraftWeapons[GetRandomInt(static_cast<int>(SafeAircraftWeapons.size()))];
                pAirType->Weapon[0].WeaponType = wp1.Normal;
                pAirType->EliteWeapon[0].WeaponType = wp1.Elite;
                LogDebug("[Local] Aircraft %s primary -> %s | %s", pAirType->ID, wp1.Normal->ID, wp1.Elite->ID);
            }
            if (BackupAircraftWeapons[typeIdx].Sec) {
                WeaponPair wp2 = isChaosToggleOn ? GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))] : SafeAircraftWeapons[GetRandomInt(static_cast<int>(SafeAircraftWeapons.size()))];
                pAirType->Weapon[1].WeaponType = wp2.Normal;
                pAirType->EliteWeapon[1].WeaponType = wp2.Elite;
                LogDebug("[Local] Aircraft %s secondary -> %s | %s", pAirType->ID, wp2.Normal->ID, wp2.Elite->ID);
            }
        }
    }

    // Buildings
    for (auto pBldType : selectedBldTypes) {
        int typeIdx = -1;
        for (int idx = 0; idx < BuildingTypeClass::Array.Count; ++idx) {
            if (BuildingTypeClass::Array.Items[idx] == pBldType) { typeIdx = idx; break; }
        }
        if (typeIdx != -1 && typeIdx < static_cast<int>(BackupBuildingWeapons.size())) {
            if (BackupBuildingWeapons[typeIdx].Pri) {
                WeaponPair wp1;
                if (isChaosToggleOn && includeBldInChaos)
                    wp1 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                else
                    wp1 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                pBldType->Weapon[0].WeaponType = wp1.Normal;
                pBldType->EliteWeapon[0].WeaponType = wp1.Elite;
                LogDebug("[Local] Building %s primary -> %s | %s", pBldType->ID, wp1.Normal->ID, wp1.Elite->ID);
            }
            if (BackupBuildingWeapons[typeIdx].Sec) {
                WeaponPair wp2;
                if (isChaosToggleOn && includeBldInChaos)
                    wp2 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                else
                    wp2 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                pBldType->Weapon[1].WeaponType = wp2.Normal;
                pBldType->EliteWeapon[1].WeaponType = wp2.Elite;
                LogDebug("[Local] Building %s secondary -> %s | %s", pBldType->ID, wp2.Normal->ID, wp2.Elite->ID);
            }
        }
    }

    LogDebug("<<< Local randomization done <<<");
    Sleep(300);
}

void RandomizeGlobalSameClass() {
    LogDebug(">>> Global same-class shuffle started (W key) <<<");
    PlayBeep(2000, 100);

    if (randInfantry && SafeInfWeapons.size() > 0) {
        for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
            InfantryTypeClass* pInf = InfantryTypeClass::Array.Items[i];
            if (pInf && !IsWhitelisted(pInf->ID) && i < static_cast<int>(BackupInfWeapons.size())) {
                if (BackupInfWeapons[i].Pri) {
                    WeaponPair wp1 = SafeInfWeapons[GetRandomInt(static_cast<int>(SafeInfWeapons.size()))];
                    pInf->Weapon[0].WeaponType = wp1.Normal;
                    pInf->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Global] Infantry %s primary -> %s | %s", pInf->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupInfWeapons[i].Sec) {
                    WeaponPair wp2 = SafeInfWeapons[GetRandomInt(static_cast<int>(SafeInfWeapons.size()))];
                    pInf->Weapon[1].WeaponType = wp2.Normal;
                    pInf->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Global] Infantry %s secondary -> %s | %s", pInf->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
                if (BackupInfWeapons[i].Occ && SafeOccupyWeapons.size() > 0) {
                    WeaponPair wp3 = SafeOccupyWeapons[GetRandomInt(static_cast<int>(SafeOccupyWeapons.size()))];
                    pInf->OccupyWeapon.WeaponType = wp3.Normal;
                    pInf->EliteOccupyWeapon.WeaponType = wp3.Elite;
                    LogDebug("[Global] Infantry %s occupant -> %s | %s", pInf->ID, wp3.Normal->ID, wp3.Elite->ID);
                }
            }
        }
    }

    if (randUnits && SafeUnitWeapons.size() > 0) {
        for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
            UnitTypeClass* pUnit = UnitTypeClass::Array.Items[i];
            if (pUnit && !IsWhitelisted(pUnit->ID) && i < static_cast<int>(BackupUnitWeapons.size())) {
                if (BackupUnitWeapons[i].Pri) {
                    WeaponPair wp1 = SafeUnitWeapons[GetRandomInt(static_cast<int>(SafeUnitWeapons.size()))];
                    pUnit->Weapon[0].WeaponType = wp1.Normal;
                    pUnit->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Global] Unit %s primary -> %s | %s", pUnit->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupUnitWeapons[i].Sec) {
                    WeaponPair wp2 = SafeUnitWeapons[GetRandomInt(static_cast<int>(SafeUnitWeapons.size()))];
                    pUnit->Weapon[1].WeaponType = wp2.Normal;
                    pUnit->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Global] Unit %s secondary -> %s | %s", pUnit->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    if (randAircraft && SafeAircraftWeapons.size() > 0) {
        for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
            AircraftTypeClass* pAir = AircraftTypeClass::Array.Items[i];
            if (pAir && !IsWhitelisted(pAir->ID) && i < static_cast<int>(BackupAircraftWeapons.size())) {
                if (BackupAircraftWeapons[i].Pri) {
                    WeaponPair wp1 = SafeAircraftWeapons[GetRandomInt(static_cast<int>(SafeAircraftWeapons.size()))];
                    pAir->Weapon[0].WeaponType = wp1.Normal;
                    pAir->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Global] Aircraft %s primary -> %s | %s", pAir->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupAircraftWeapons[i].Sec) {
                    WeaponPair wp2 = SafeAircraftWeapons[GetRandomInt(static_cast<int>(SafeAircraftWeapons.size()))];
                    pAir->Weapon[1].WeaponType = wp2.Normal;
                    pAir->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Global] Aircraft %s secondary -> %s | %s", pAir->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    if (randBuildings && SafeBuildingWeapons.size() > 0) {
        for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
            BuildingTypeClass* pBld = BuildingTypeClass::Array.Items[i];
            if (pBld && !IsWhitelisted(pBld->ID) && i < static_cast<int>(BackupBuildingWeapons.size())) {
                if (BackupBuildingWeapons[i].Pri) {
                    WeaponPair wp1 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                    pBld->Weapon[0].WeaponType = wp1.Normal;
                    pBld->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Global] Building %s primary -> %s | %s", pBld->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupBuildingWeapons[i].Sec) {
                    WeaponPair wp2 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                    pBld->Weapon[1].WeaponType = wp2.Normal;
                    pBld->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Global] Building %s secondary -> %s | %s", pBld->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    LogDebug("<<< Global same-class shuffle done <<<");
    Sleep(300);
}

void RandomizeGlobalChaos() {
    LogDebug(">>> Global cross-class chaos started (C key) <<<");
    PlayBeep(2500, 150);

    if (GlobalChaosPool.empty()) return;

    if (randInfantry) {
        for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
            InfantryTypeClass* pInf = InfantryTypeClass::Array.Items[i];
            if (pInf && !IsWhitelisted(pInf->ID) && i < static_cast<int>(BackupInfWeapons.size())) {
                if (BackupInfWeapons[i].Pri) {
                    WeaponPair wp1 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pInf->Weapon[0].WeaponType = wp1.Normal;
                    pInf->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Chaos] Infantry %s primary -> %s | %s", pInf->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupInfWeapons[i].Sec) {
                    WeaponPair wp2 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pInf->Weapon[1].WeaponType = wp2.Normal;
                    pInf->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Chaos] Infantry %s secondary -> %s | %s", pInf->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
                if (BackupInfWeapons[i].Occ) {
                    WeaponPair wp3 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pInf->OccupyWeapon.WeaponType = wp3.Normal;
                    pInf->EliteOccupyWeapon.WeaponType = wp3.Elite;
                    LogDebug("[Chaos] Infantry %s occupant -> %s | %s", pInf->ID, wp3.Normal->ID, wp3.Elite->ID);
                }
            }
        }
    }

    if (randUnits) {
        for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
            UnitTypeClass* pUnit = UnitTypeClass::Array.Items[i];
            if (pUnit && !IsWhitelisted(pUnit->ID) && i < static_cast<int>(BackupUnitWeapons.size())) {
                if (BackupUnitWeapons[i].Pri) {
                    WeaponPair wp1 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pUnit->Weapon[0].WeaponType = wp1.Normal;
                    pUnit->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Chaos] Unit %s primary -> %s | %s", pUnit->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupUnitWeapons[i].Sec) {
                    WeaponPair wp2 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pUnit->Weapon[1].WeaponType = wp2.Normal;
                    pUnit->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Chaos] Unit %s secondary -> %s | %s", pUnit->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    if (randAircraft) {
        for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
            AircraftTypeClass* pAir = AircraftTypeClass::Array.Items[i];
            if (pAir && !IsWhitelisted(pAir->ID) && i < static_cast<int>(BackupAircraftWeapons.size())) {
                if (BackupAircraftWeapons[i].Pri) {
                    WeaponPair wp1 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pAir->Weapon[0].WeaponType = wp1.Normal;
                    pAir->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Chaos] Aircraft %s primary -> %s | %s", pAir->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupAircraftWeapons[i].Sec) {
                    WeaponPair wp2 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    pAir->Weapon[1].WeaponType = wp2.Normal;
                    pAir->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Chaos] Aircraft %s secondary -> %s | %s", pAir->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    if (randBuildings) {
        for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
            BuildingTypeClass* pBld = BuildingTypeClass::Array.Items[i];
            if (pBld && !IsWhitelisted(pBld->ID) && i < static_cast<int>(BackupBuildingWeapons.size())) {
                if (BackupBuildingWeapons[i].Pri) {
                    WeaponPair wp1;
                    if (includeBldInChaos)
                        wp1 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    else
                        wp1 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                    pBld->Weapon[0].WeaponType = wp1.Normal;
                    pBld->EliteWeapon[0].WeaponType = wp1.Elite;
                    LogDebug("[Chaos] Building %s primary -> %s | %s", pBld->ID, wp1.Normal->ID, wp1.Elite->ID);
                }
                if (BackupBuildingWeapons[i].Sec) {
                    WeaponPair wp2;
                    if (includeBldInChaos)
                        wp2 = GlobalChaosPool[GetRandomInt(static_cast<int>(GlobalChaosPool.size()))];
                    else
                        wp2 = SafeBuildingWeapons[GetRandomInt(static_cast<int>(SafeBuildingWeapons.size()))];
                    pBld->Weapon[1].WeaponType = wp2.Normal;
                    pBld->EliteWeapon[1].WeaponType = wp2.Elite;
                    LogDebug("[Chaos] Building %s secondary -> %s | %s", pBld->ID, wp2.Normal->ID, wp2.Elite->ID);
                }
            }
        }
    }

    LogDebug("<<< Global cross-class chaos done <<<");
    Sleep(300);
}

void RestoreOriginal() {
    LogDebug(">>> Restoring original weapons (R key) <<<");
    PlayBeep(1000, 200);

    for (int i = 0; i < InfantryTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupInfWeapons.size()) && InfantryTypeClass::Array.Items[i]) {
            InfantryTypeClass::Array.Items[i]->Weapon[0].WeaponType = BackupInfWeapons[i].Pri;
            InfantryTypeClass::Array.Items[i]->Weapon[1].WeaponType = BackupInfWeapons[i].Sec;
            InfantryTypeClass::Array.Items[i]->EliteWeapon[0].WeaponType = BackupInfWeapons[i].EPri;
            InfantryTypeClass::Array.Items[i]->EliteWeapon[1].WeaponType = BackupInfWeapons[i].ESec;
            InfantryTypeClass::Array.Items[i]->OccupyWeapon.WeaponType = BackupInfWeapons[i].Occ;
            InfantryTypeClass::Array.Items[i]->EliteOccupyWeapon.WeaponType = BackupInfWeapons[i].EOcc;
        }
    }
    for (int i = 0; i < UnitTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupUnitWeapons.size()) && UnitTypeClass::Array.Items[i]) {
            UnitTypeClass::Array.Items[i]->Weapon[0].WeaponType = BackupUnitWeapons[i].Pri;
            UnitTypeClass::Array.Items[i]->Weapon[1].WeaponType = BackupUnitWeapons[i].Sec;
            UnitTypeClass::Array.Items[i]->EliteWeapon[0].WeaponType = BackupUnitWeapons[i].EPri;
            UnitTypeClass::Array.Items[i]->EliteWeapon[1].WeaponType = BackupUnitWeapons[i].ESec;
        }
    }
    for (int i = 0; i < AircraftTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupAircraftWeapons.size()) && AircraftTypeClass::Array.Items[i]) {
            AircraftTypeClass::Array.Items[i]->Weapon[0].WeaponType = BackupAircraftWeapons[i].Pri;
            AircraftTypeClass::Array.Items[i]->Weapon[1].WeaponType = BackupAircraftWeapons[i].Sec;
            AircraftTypeClass::Array.Items[i]->EliteWeapon[0].WeaponType = BackupAircraftWeapons[i].EPri;
            AircraftTypeClass::Array.Items[i]->EliteWeapon[1].WeaponType = BackupAircraftWeapons[i].ESec;
        }
    }
    for (int i = 0; i < BuildingTypeClass::Array.Count; i++) {
        if (i < static_cast<int>(BackupBuildingWeapons.size()) && BuildingTypeClass::Array.Items[i]) {
            BuildingTypeClass::Array.Items[i]->Weapon[0].WeaponType = BackupBuildingWeapons[i].Pri;
            BuildingTypeClass::Array.Items[i]->Weapon[1].WeaponType = BackupBuildingWeapons[i].Sec;
            BuildingTypeClass::Array.Items[i]->EliteWeapon[0].WeaponType = BackupBuildingWeapons[i].EPri;
            BuildingTypeClass::Array.Items[i]->EliteWeapon[1].WeaponType = BackupBuildingWeapons[i].ESec;
        }
    }

    LogDebug("<<< Restore completed <<<");
    Sleep(500);
}