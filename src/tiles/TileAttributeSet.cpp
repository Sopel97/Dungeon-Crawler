#include "tiles/TileAttributeSet.h"

#include <algorithm>

TileAttributeSet& TileAttributeSet::operator+=(const TileAttributeSet& rhs)
{
    auto lhsCurrent = begin();
    auto rhsCurrent = rhs.begin();
    auto rhsEnd = rhs.end();

    while (lhsCurrent != end() && rhsCurrent != rhsEnd)
    {
        if (lhsCurrent->id < rhsCurrent->id)
        {
            ++lhsCurrent;
        }
        else if (lhsCurrent->id == rhsCurrent->id)
        {
            lhsCurrent->value += rhsCurrent->value;
            ++lhsCurrent;
            ++rhsCurrent;
        }
        else
        {
            lhsCurrent = m_attributes.insert(std::next(lhsCurrent), *rhsCurrent);
            ++lhsCurrent;
            ++rhsCurrent;
        }
    }
    while (rhsCurrent != rhsEnd)
    {
        m_attributes.push_back(*rhsCurrent);
    }

    return *this;
}
TileAttributeSet& TileAttributeSet::operator-=(const TileAttributeSet& rhs)
{
    auto lhsCurrent = begin();
    auto rhsCurrent = rhs.begin();
    auto rhsEnd = rhs.end();

    while (lhsCurrent != end() && rhsCurrent != rhsEnd)
    {
        if (lhsCurrent->id < rhsCurrent->id)
        {
            ++lhsCurrent;
        }
        else if (lhsCurrent->id == rhsCurrent->id)
        {
            lhsCurrent->value -= rhsCurrent->value;
            ++lhsCurrent;
            ++rhsCurrent;
        }
        else
        {
            lhsCurrent = m_attributes.insert(std::next(lhsCurrent), TileAttribute{ rhsCurrent->id, -rhsCurrent->value });
            ++lhsCurrent;
            ++rhsCurrent;
        }
    }
    while (rhsCurrent != rhsEnd)
    {
        m_attributes.push_back(*rhsCurrent);
    }

    return *this;
}
TileAttributeSet TileAttributeSet::operator+(const TileAttributeSet& rhs) const
{
    TileAttributeSet sum = *this;
    sum += rhs;
    return sum;
}
TileAttributeSet TileAttributeSet::operator-(const TileAttributeSet& rhs) const
{
    TileAttributeSet diff = *this;
    diff -= rhs;
    return diff;
}

TileAttributeSet& TileAttributeSet::operator+=(const TileAttribute& rhs)
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), rhs, [](const TileAttribute& lhs, const TileAttribute& rhs)->bool {return lhs.id < rhs.id; });

    if (iter == cend() || iter->id != rhs.id) // not found
    {
        m_attributes.insert(iter, rhs);
    }
    else
    {
        iter->value += rhs.value;
    }

    return *this;
}
TileAttributeSet& TileAttributeSet::operator-=(const TileAttribute& rhs)
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), rhs.id, [](const TileAttribute& lhs, const TileAttributeId& rhs)->bool {return lhs.id < rhs; });

    if (iter == cend() || iter->id != rhs.id) // not found
    {
        m_attributes.insert(iter, TileAttribute{ rhs.id, -rhs.value });
    }
    else
    {
        iter->value -= rhs.value;
    }

    return *this;
}
TileAttributeSet TileAttributeSet::operator+(const TileAttribute& rhs) const
{
    TileAttributeSet sum = *this;
    sum += rhs;
    return sum;
}
TileAttributeSet TileAttributeSet::operator-(const TileAttribute& rhs) const
{
    TileAttributeSet diff = *this;
    diff -= rhs;
    return diff;
}

bool TileAttributeSet::isEmpty() const
{
    return m_attributes.empty();
}
bool TileAttributeSet::isPresent(TileAttributeId id) const
{
    return find(id) != cend();
}
int TileAttributeSet::value(TileAttributeId id) const
{
    auto iter = find(id);
    if(iter != cend()) return iter->value;
    else return 0;
}
TileAttributeSet::ConstIterator TileAttributeSet::find(TileAttributeId id) const
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), id, [](const TileAttribute& lhs, const TileAttributeId& rhs)->bool {return lhs.id < rhs; });
    
    if (iter == cend()) return iter;
    if (iter->id != id) return cend();

    return iter;
}

TileAttributeSet::ConstIterator TileAttributeSet::cbegin() const
{
    return m_attributes.cbegin();
}
TileAttributeSet::ConstIterator TileAttributeSet::begin() const
{
    return m_attributes.begin();
}
TileAttributeSet::Iterator TileAttributeSet::begin()
{
    return m_attributes.begin();
}
TileAttributeSet::ConstIterator TileAttributeSet::cend() const
{
    return m_attributes.cend();
}
TileAttributeSet::ConstIterator TileAttributeSet::end() const
{
    return m_attributes.end();
}
TileAttributeSet::Iterator TileAttributeSet::end()
{
    return m_attributes.end();
}

void TileAttributeSet::clear()
{
    m_attributes.clear();
}