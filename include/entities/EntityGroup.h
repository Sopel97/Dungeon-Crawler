#pragma once

#include <map>
#include <string>

enum class EntityGroupId
{
    Friendly,
    Unfriendly,
    Neutral,
    Aggressive,
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
        /*                   Friendly    Unfriendly    Neutral    Aggressive */
        /* Friendly */     { true,       true,         true,      true },
        /* Unfriendly */   { true,       true,         true,      true },
        /* Neutral */      { true,       true,         true,      true },
        /* Aggressive */   { true,       true,         true,      true }
    };
    static constexpr bool m_canDamage[numGroups][numGroups] = {
        /*                   Friendly    Unfriendly    Neutral    Aggressive */
        /* Friendly */     { true,       true,         false,     true },
        /* Unfriendly */   { true,       true,         false,     true },
        /* Neutral */      { false,      false,        false,     false },
        /* Aggressive */   { true,       true,         false,     false }
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