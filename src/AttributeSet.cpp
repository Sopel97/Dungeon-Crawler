#include "AttributeSet.h"

#include <algorithm>

bool AttributeSet::operator==(const AttributeSet& rhs) const
{
    return std::equal(m_attributes.begin(), m_attributes.end(), rhs.m_attributes.begin(), rhs.m_attributes.end());
}
bool AttributeSet::operator!=(const AttributeSet& rhs) const
{
    return !operator==(rhs);
}
AttributeSet& AttributeSet::operator+=(const AttributeSet& rhs)
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
AttributeSet& AttributeSet::operator-=(const AttributeSet& rhs)
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
            lhsCurrent = m_attributes.insert(std::next(lhsCurrent), Attribute{ rhsCurrent->id, -rhsCurrent->value });
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
AttributeSet AttributeSet::operator+(const AttributeSet& rhs) const
{
    AttributeSet sum = *this;
    sum += rhs;
    return sum;
}
AttributeSet AttributeSet::operator-(const AttributeSet& rhs) const
{
    AttributeSet diff = *this;
    diff -= rhs;
    return diff;
}

AttributeSet& AttributeSet::operator+=(const Attribute& rhs)
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), rhs, [](const Attribute& lhs, const Attribute& rhs)->bool {return lhs.id < rhs.id; });

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
AttributeSet& AttributeSet::operator-=(const Attribute& rhs)
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), rhs.id, [](const Attribute& lhs, const AttributeId& rhs)->bool {return lhs.id < rhs; });

    if (iter == cend() || iter->id != rhs.id) // not found
    {
        m_attributes.insert(iter, Attribute{ rhs.id, -rhs.value });
    }
    else
    {
        iter->value -= rhs.value;
    }

    return *this;
}
AttributeSet AttributeSet::operator+(const Attribute& rhs) const
{
    AttributeSet sum = *this;
    sum += rhs;
    return sum;
}
AttributeSet AttributeSet::operator-(const Attribute& rhs) const
{
    AttributeSet diff = *this;
    diff -= rhs;
    return diff;
}

bool AttributeSet::isEmpty() const
{
    return m_attributes.empty();
}
bool AttributeSet::isPresent(AttributeId id) const
{
    return find(id) != cend();
}
int AttributeSet::value(AttributeId id) const
{
    auto iter = find(id);
    if(iter != cend()) return iter->value;
    else return 0;
}
AttributeSet::ConstIterator AttributeSet::find(AttributeId id) const
{
    auto iter = std::lower_bound(m_attributes.begin(), m_attributes.end(), id, [](const Attribute& lhs, const AttributeId& rhs)->bool {return lhs.id < rhs; });
    
    if (iter == cend()) return iter;
    if (iter->id != id) return cend();

    return iter;
}

AttributeSet::ConstIterator AttributeSet::cbegin() const
{
    return m_attributes.cbegin();
}
AttributeSet::ConstIterator AttributeSet::begin() const
{
    return m_attributes.begin();
}
AttributeSet::Iterator AttributeSet::begin()
{
    return m_attributes.begin();
}
AttributeSet::ConstIterator AttributeSet::cend() const
{
    return m_attributes.cend();
}
AttributeSet::ConstIterator AttributeSet::end() const
{
    return m_attributes.end();
}
AttributeSet::Iterator AttributeSet::end()
{
    return m_attributes.end();
}

void AttributeSet::clear()
{
    m_attributes.clear();
}