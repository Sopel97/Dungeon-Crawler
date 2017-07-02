#pragma once

#include <memory>
#include <vector>

#include "tiles/TileAttribute.h"

class TileAttributeSet;

class TileAttributeArray
{
private:
    using ValueType = TileAttribute::ValueType;
    static constexpr int numAttributes = TileAttributeIdHelper::numAttributes();

    std::vector<ValueType> m_values;
public:
    TileAttributeArray();
    explicit TileAttributeArray(const TileAttributeSet& set);
    TileAttributeArray(const TileAttributeSet& set, const std::vector<TileAttributeId>& mask);
    TileAttributeArray(const TileAttributeArray&) = default;
    TileAttributeArray(TileAttributeArray&&) = default;
    TileAttributeArray& operator=(const TileAttributeArray&) = default;
    TileAttributeArray& operator=(TileAttributeArray&&) = default;

    TileAttributeArray& operator+=(const TileAttributeArray& rhs);
    TileAttributeArray& operator-=(const TileAttributeArray& rhs);
    TileAttributeArray operator+(const TileAttributeArray& rhs) const;
    TileAttributeArray operator-(const TileAttributeArray& rhs) const;

    TileAttributeArray& operator+=(const TileAttribute& rhs);
    TileAttributeArray& operator-=(const TileAttribute& rhs);
    TileAttributeArray operator+(const TileAttribute& rhs) const;
    TileAttributeArray operator-(const TileAttribute& rhs) const;

    ValueType operator[](TileAttributeId id) const;

    void clear();
};