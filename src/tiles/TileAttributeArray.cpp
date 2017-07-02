#include "tiles/TileAttributeArray.h"
#include "tiles/TileAttributeSet.h"

TileAttributeArray::TileAttributeArray() :
    m_values(numAttributes, 0)
{
}
TileAttributeArray::TileAttributeArray(const TileAttributeSet& set) :
    m_values(numAttributes, 0)
{
    for (const TileAttribute& attr : set)
    {
        m_values[static_cast<int>(attr.id)] = attr.value;
    }
}
TileAttributeArray::TileAttributeArray(const TileAttributeSet& set, const std::vector<TileAttributeId>& mask) :
    m_values(numAttributes, 0)
{
    constexpr ValueType maskToCopy = std::numeric_limits<ValueType>::max();

    for (TileAttributeId maskAttr : mask)
    {
        m_values[static_cast<int>(maskAttr)] = maskToCopy;
    }

    for (const TileAttribute& attr : set)
    {
        if (m_values[static_cast<int>(attr.id)] != maskToCopy) continue;
        m_values[static_cast<int>(attr.id)] = attr.value;
    }

    for (TileAttributeId maskAttr : mask)
    {
        if (m_values[static_cast<int>(maskAttr)] != maskToCopy) continue;
        m_values[static_cast<int>(maskAttr)] = 0;
    }
}

TileAttributeArray& TileAttributeArray::operator+=(const TileAttributeArray& rhs)
{
    for (int i = 0; i < numAttributes; ++i)
    {
        m_values[i] += rhs.m_values[i];
    }

    return *this;
}
TileAttributeArray& TileAttributeArray::operator-=(const TileAttributeArray& rhs)
{
    for (int i = 0; i < numAttributes; ++i)
    {
        m_values[i] -= rhs.m_values[i];
    }

    return *this;
}
TileAttributeArray TileAttributeArray::operator+(const TileAttributeArray& rhs) const
{
    TileAttributeArray result(*this);
    result += rhs;
    return result;
}
TileAttributeArray TileAttributeArray::operator-(const TileAttributeArray& rhs) const
{
    TileAttributeArray result(*this);
    result -= rhs;
    return result;
}

TileAttributeArray& TileAttributeArray::operator+=(const TileAttribute& rhs)
{
    m_values[static_cast<int>(rhs.id)] += rhs.value;

    return *this;
}
TileAttributeArray& TileAttributeArray::operator-=(const TileAttribute& rhs)
{
    m_values[static_cast<int>(rhs.id)] -= rhs.value;

    return *this;
}
TileAttributeArray TileAttributeArray::operator+(const TileAttribute& rhs) const
{
    TileAttributeArray result(*this);
    result += rhs;
    return result;
}
TileAttributeArray TileAttributeArray::operator-(const TileAttribute& rhs) const
{
    TileAttributeArray result(*this);
    result -= rhs;
    return result;
}

TileAttributeArray::ValueType TileAttributeArray::operator[](TileAttributeId id) const
{
    return m_values[static_cast<int>(id)];
}

void TileAttributeArray::clear()
{
    std::fill(m_values.begin(), m_values.end(), 0);
}
