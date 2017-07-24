#include "AttributeArray.h"
#include "AttributeSet.h"

AttributeArray::AttributeArray() :
    m_values(numAttributes, 0)
{
}
AttributeArray::AttributeArray(const AttributeSet& set) :
    m_values(numAttributes, 0)
{
    for (const Attribute& attr : set)
    {
        m_values[static_cast<int>(attr.id)] = attr.value;
    }
}
AttributeArray::AttributeArray(const AttributeSet& set, const std::vector<AttributeId>& mask) :
    m_values(numAttributes, 0)
{
    constexpr ValueType maskToCopy = std::numeric_limits<ValueType>::max();

    for (AttributeId maskAttr : mask)
    {
        m_values[static_cast<int>(maskAttr)] = maskToCopy;
    }

    for (const Attribute& attr : set)
    {
        if (m_values[static_cast<int>(attr.id)] != maskToCopy) continue;
        m_values[static_cast<int>(attr.id)] = attr.value;
    }

    for (AttributeId maskAttr : mask)
    {
        if (m_values[static_cast<int>(maskAttr)] != maskToCopy) continue;
        m_values[static_cast<int>(maskAttr)] = 0;
    }
}
AttributeArray::AttributeArray(const AttributeArray& other, const std::vector<AttributeId>& mask) :
    m_values(numAttributes, 0)
{
    for (auto id : mask)
    {
        m_values[static_cast<int>(id)] = other.m_values[static_cast<int>(id)];
    }
}

bool AttributeArray::operator==(const AttributeArray& rhs) const
{
    return std::equal(m_values.begin(), m_values.end(), rhs.m_values.begin());
}
bool AttributeArray::operator!=(const AttributeArray& rhs) const
{
    return !operator==(rhs);
}

AttributeArray& AttributeArray::operator+=(const AttributeArray& rhs)
{
    for (int i = 0; i < numAttributes; ++i)
    {
        m_values[i] += rhs.m_values[i];
    }

    return *this;
}
AttributeArray& AttributeArray::operator-=(const AttributeArray& rhs)
{
    for (int i = 0; i < numAttributes; ++i)
    {
        m_values[i] -= rhs.m_values[i];
    }

    return *this;
}
AttributeArray AttributeArray::operator+(const AttributeArray& rhs) const
{
    AttributeArray result(*this);
    result += rhs;
    return result;
}
AttributeArray AttributeArray::operator-(const AttributeArray& rhs) const
{
    AttributeArray result(*this);
    result -= rhs;
    return result;
}

AttributeArray& AttributeArray::operator+=(const Attribute& rhs)
{
    m_values[static_cast<int>(rhs.id)] += rhs.value;

    return *this;
}
AttributeArray& AttributeArray::operator-=(const Attribute& rhs)
{
    m_values[static_cast<int>(rhs.id)] -= rhs.value;

    return *this;
}
AttributeArray AttributeArray::operator+(const Attribute& rhs) const
{
    AttributeArray result(*this);
    result += rhs;
    return result;
}
AttributeArray AttributeArray::operator-(const Attribute& rhs) const
{
    AttributeArray result(*this);
    result -= rhs;
    return result;
}

AttributeArray::ValueType AttributeArray::operator[](AttributeId id) const
{
    return m_values[static_cast<int>(id)];
}

void AttributeArray::clear()
{
    std::fill(m_values.begin(), m_values.end(), 0);
}
