#pragma once

#include <map>

template <class GroupType, class FromType, class IntType>
class DynamicEnum
{
private:
    IntType m_id;

    static IntType nextId()
    {
        static IntType m_nextId = 0;
        return m_nextId++;
    }
    static IntType hash(const FromType& from)
    {
        static std::map<FromType, IntType> m_hash;
        
        auto it = m_hash.find(from);
        if (it != m_hash.end())
        {
            return it->second;
        }
        else
        {
            return m_hash.insert(std::pair<FromType, IntType>(from, nextId())).first->second;
        }
    }

    static constexpr IntType m_invalidId = -1;

public:
    DynamicEnum() : m_id(m_invalidId) {}
    DynamicEnum(const DynamicEnum&) = default;
    DynamicEnum(DynamicEnum&&) = default;
    DynamicEnum(const FromType& from) : m_id(hash(from)) {}
    DynamicEnum& operator=(const DynamicEnum&) = default;
    DynamicEnum& operator=(DynamicEnum&&) = default;
    DynamicEnum& operator=(const FromType& from)
    {
        m_id = hash(from);
        return *this;
    }

    IntType id() const
    {
        return m_id;
    }

    operator IntType() const
    {
        return m_id;
    }
};

template <class GroupType>
using RuntimeIdentifier = DynamicEnum<GroupType, std::string, int>;