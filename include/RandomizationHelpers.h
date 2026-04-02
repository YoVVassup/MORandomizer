#pragma once

#include <vector>
#include <set>
#include <type_traits>
#include <YRpp.h>
#include "Config.h"
#include "WeaponPool.h"
#include "Utilities.h"

// ============================================================
// Helper: get type index from pointer
// ============================================================
template<typename T>
int GetTypeIndex(T* pType, const DynamicVectorClass<T*>& array) {
    for (int i = 0; i < array.Count; ++i) {
        if (array.Items[i] == pType) return i;
    }
    return -1;
}

// ============================================================
// Local randomization (selected units only)
// ============================================================
template<typename TTypeClass>
void RandomizeUnitType(
    const std::set<TTypeClass*>& selectedTypes,
    const std::vector<WeaponSet>& backups,
    std::vector<WeaponPair>& safePool,
    const std::vector<WeaponPair>& chaosPool,
    bool useChaosPool)
{
    if (selectedTypes.empty()) return;

    int safePoolSize = static_cast<int>(safePool.size());
    int chaosPoolSize = static_cast<int>(chaosPool.size());

    for (auto pType : selectedTypes) {
        int typeIdx = GetTypeIndex(pType, TTypeClass::Array);
        if (typeIdx == -1 || typeIdx >= static_cast<int>(backups.size())) continue;

        const auto& backup = backups[typeIdx];

        // Primary weapon
        if (backup.weapons[SLOT_PRIMARY]) {
            WeaponPair wp;
            if (useChaosPool && chaosPoolSize > 0)
                wp = chaosPool[GetRandomInt(chaosPoolSize)];
            else if (safePoolSize > 0)
                wp = safePool[GetRandomInt(safePoolSize)];
            else continue;
            pType->Weapon[0].WeaponType = wp.Normal;
            pType->EliteWeapon[0].WeaponType = wp.Elite;
            LogDebug("[Local] %s primary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Secondary weapon
        if (backup.weapons[SLOT_SECONDARY]) {
            WeaponPair wp;
            if (useChaosPool && chaosPoolSize > 0)
                wp = chaosPool[GetRandomInt(chaosPoolSize)];
            else if (safePoolSize > 0)
                wp = safePool[GetRandomInt(safePoolSize)];
            else continue;
            pType->Weapon[1].WeaponType = wp.Normal;
            pType->EliteWeapon[1].WeaponType = wp.Elite;
            LogDebug("[Local] %s secondary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Occupy weapon (infantry only)
        if constexpr (std::is_same_v<TTypeClass, InfantryTypeClass>) {
            if (backup.weapons[SLOT_OCCUPY] && SafeOccupyWeapons.size() > 0) {
                WeaponPair wp;
                if (useChaosPool && chaosPoolSize > 0)
                    wp = chaosPool[GetRandomInt(chaosPoolSize)];
                else {
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

// ============================================================
// Global randomization (same type, all units)
// ============================================================
template<typename TTypeClass>
void RandomizeGlobalSameClassForType(
    DynamicVectorClass<TTypeClass*>& typeArray,
    const std::vector<WeaponSet>& backups,
    std::vector<WeaponPair>& safePool,
    std::vector<WeaponPair>& occupyPool)   // for infantry
{
    if (safePool.empty()) return;

    int safePoolSize = static_cast<int>(safePool.size());
    int occPoolSize = static_cast<int>(occupyPool.size());

    for (int i = 0; i < typeArray.Count; ++i) {
        TTypeClass* pType = typeArray.Items[i];
        if (!pType || IsWhitelisted(pType->ID)) continue;
        if (i >= static_cast<int>(backups.size())) continue;

        const auto& backup = backups[i];

        // Primary
        if (backup.weapons[SLOT_PRIMARY]) {
            WeaponPair wp = safePool[GetRandomInt(safePoolSize)];
            pType->Weapon[0].WeaponType = wp.Normal;
            pType->EliteWeapon[0].WeaponType = wp.Elite;
            LogDebug("[Global] %s primary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Secondary
        if (backup.weapons[SLOT_SECONDARY]) {
            WeaponPair wp = safePool[GetRandomInt(safePoolSize)];
            pType->Weapon[1].WeaponType = wp.Normal;
            pType->EliteWeapon[1].WeaponType = wp.Elite;
            LogDebug("[Global] %s secondary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Occupy weapon (infantry only)
        if constexpr (std::is_same_v<TTypeClass, InfantryTypeClass>) {
            if (backup.weapons[SLOT_OCCUPY] && occPoolSize > 0) {
                WeaponPair wp = occupyPool[GetRandomInt(occPoolSize)];
                pType->OccupyWeapon.WeaponType = wp.Normal;
                pType->EliteOccupyWeapon.WeaponType = wp.Elite;
                LogDebug("[Global] Infantry %s occupant -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
            }
        }
    }
}

// ============================================================
// Global randomization (shuffling between types)
// ============================================================
template<typename TTypeClass>
void RandomizeGlobalChaosForType(
    DynamicVectorClass<TTypeClass*>& typeArray,
    const std::vector<WeaponSet>& backups,
    std::vector<WeaponPair>& chaosPool,
    std::vector<WeaponPair>& buildingSafePool,   // for buildings, if chaos is not allowed
    bool useChaosForBuildings)
{
    if (chaosPool.empty()) return;

    for (int i = 0; i < typeArray.Count; ++i) {
        TTypeClass* pType = typeArray.Items[i];
        if (!pType || IsWhitelisted(pType->ID)) continue;
        if (i >= static_cast<int>(backups.size())) continue;

        const auto& backup = backups[i];

        // Selecting a pool for this type
        std::vector<WeaponPair>* sourcePool = &chaosPool;
        if constexpr (std::is_same_v<TTypeClass, BuildingTypeClass>) {
            if (!useChaosForBuildings) {
                sourcePool = &buildingSafePool;
                if (sourcePool->empty()) continue;
            }
        }
        int poolSize = static_cast<int>(sourcePool->size());

        // Primary
        if (backup.weapons[SLOT_PRIMARY]) {
            WeaponPair wp = (*sourcePool)[GetRandomInt(poolSize)];
            pType->Weapon[0].WeaponType = wp.Normal;
            pType->EliteWeapon[0].WeaponType = wp.Elite;
            LogDebug("[Chaos] %s primary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Secondary
        if (backup.weapons[SLOT_SECONDARY]) {
            WeaponPair wp = (*sourcePool)[GetRandomInt(poolSize)];
            pType->Weapon[1].WeaponType = wp.Normal;
            pType->EliteWeapon[1].WeaponType = wp.Elite;
            LogDebug("[Chaos] %s secondary -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
        }

        // Occupy weapon (infantry only)
        if constexpr (std::is_same_v<TTypeClass, InfantryTypeClass>) {
            if (backup.weapons[SLOT_OCCUPY]) {
                WeaponPair wp = (*sourcePool)[GetRandomInt(poolSize)];
                pType->OccupyWeapon.WeaponType = wp.Normal;
                pType->EliteOccupyWeapon.WeaponType = wp.Elite;
                LogDebug("[Chaos] Infantry %s occupant -> %s | %s", pType->ID, wp.Normal->ID, wp.Elite->ID);
            }
        }
    }
}