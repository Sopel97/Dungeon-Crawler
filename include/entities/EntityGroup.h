#pragma once

#include <map>
#include <string>

enum class EntityGroupId
{
    Friendly,
    Unfriendly,
    NumGroups
};

class EntityGroupIdHelper
{
private:
    static const std::map<std::string, EntityGroupId> m_stringToEnum;

public:
    static EntityGroupId stringToEnum(const std::string& s)
    {
        return m_stringToEnum.at(s);
    }
};

class EntityGroupRelations
{
private:
    static constexpr int numGroups = static_cast<int>(EntityGroupId::NumGroups);
    static constexpr bool m_canCollide[numGroups][numGroups] = {
        /*                   Friendly    Unfriendly */
        /* Friendly */     { true,       true },
        /* Unfriendly */   { true,       true }
    };
    static constexpr bool m_canDamage[numGroups][numGroups] = {
        /*                   Friendly    Unfriendly */
        /* Friendly */     { false,      true },
        /* Unfriendly */   { true,       false }
    };

public:
    static constexpr bool canCollide(EntityGroupId first, EntityGroupId second)
    {
        return m_canCollide[static_cast<int>(first)][static_cast<int>(second)];
    }
    static constexpr bool canDamage(EntityGroupId first, EntityGroupId second)
    {
        return m_canDamage[static_cast<int>(first)][static_cast<int>(second)];
    }
};