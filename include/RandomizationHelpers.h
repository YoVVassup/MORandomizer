#pragma once

#include <vector>
#include <set>
#include <YRpp.h>
#include "Config.h"
#include "WeaponPool.h"
#include "Utilities.h"
#include <type_traits>

// Template for obtaining a type index from a pointer
template<typename T>
int GetTypeIndex(T* pType, const DynamicVectorClass<T*>& array) {
    for (int i = 0; i < array.Count; ++i) {
        if (array.Items[i] == pType) return i;
    }
    return -1;
}

// Template for randomizing one type of units (infantry, vehicles, aviation, buildings)
template<typename TTypeClass>
void RandomizeUnitType(
    const std::set<TTypeClass*>& selectedTypes,
    const std::vector<WeaponSet>& backups,
    std::vector<WeaponPair>& safePool,
    const std::vector<WeaponPair>& chaosPool,
    bool useChaosPool,
    bool isBuilding = false)
{
    if (selectedTypes.empty()) return;

    int safePoolSize = static_cast<int>(safePool.size());
    int chaosPoolSize = static_cast<int>(chaosPool.size());

    for (auto pType : selectedTypes) {
        int typeIdx = GetTypeIndex(pType, TTypeClass::Array);
        if (typeIdx == -1 || typeIdx >= static_cast<int>(backups.size())) continue;

        const auto& backup = backups[typeIdx];

        if (backup.Pri) {
            WeaponPair wp;
            if (useChaosPool && chaosPoolSize > 0) {
                wp = chaosPool[GetRandomInt(chaosPoolSize)];
            } else if (safePoolSize > 0) {
                wp = safePool[GetRandomInt(safePoolSize)];
            } else continue;
            pType->Weapon[0].WeaponType = wp.Normal;
            pType->EliteWeapon[0].WeaponType = wp.Elite;
            LogDebug("[Local] %s primary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        if (backup.Sec) {
            WeaponPair wp;
            if (useChaosPool && chaosPoolSize > 0) {
                wp = chaosPool[GetRandomInt(chaosPoolSize)];
            } else if (safePoolSize > 0) {
                wp = safePool[GetRandomInt(safePoolSize)];
            } else continue;
            pType->Weapon[1].WeaponType = wp.Normal;
            pType->EliteWeapon[1].WeaponType = wp.Elite;
            LogDebug("[Local] %s secondary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Special treatment for infantry (occupy weapon)
        if constexpr (std::is_same_v<TTypeClass, InfantryTypeClass>) {
            if (backup.Occ && SafeOccupyWeapons.size() > 0) {
                WeaponPair wp;
                if (useChaosPool && chaosPoolSize > 0) {
                    wp = chaosPool[GetRandomInt(chaosPoolSize)];
                } else {
                    int occPoolSize = static_cast<int>(SafeOccupyWeapons.size());
                    wp = SafeOccupyWeapons[GetRandomInt(occPoolSize)];
                }
                pType->OccupyWeapon.WeaponType = wp.Normal;
                pType->EliteOccupyWeapon.WeaponType = wp.Elite;
                LogDebug("[Local] Infantry %s occupant -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
            }
        }
    }
}