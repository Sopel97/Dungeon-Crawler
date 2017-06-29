#pragma once

#include <map>

template <class GroupType, class FromType, class IntType>
class CompactIdentifier
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
    CompactIdentifier() : m_id(m_invalidId) {}
    CompactIdentifier(const CompactIdentifier&) = default;
    CompactIdentifier(CompactIdentifier&&) = default;
    CompactIdentifier(const FromType& from) : m_id(hash(from)) {}
    CompactIdentifier& operator=(const CompactIdentifier&) = default;
    CompactIdentifier& operator=(CompactIdentifier&&) = default;
    CompactIdentifier& operator=(const FromType& from)
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