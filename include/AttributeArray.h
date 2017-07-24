#pragma once

#include <memory>
#include <vector>

#include "Attribute.h"

class AttributeSet;

class AttributeArray
{
private:
    using ValueType = Attribute::ValueType;
    static constexpr int numAttributes = AttributeIdHelper::numAttributes();

    std::vector<ValueType> m_values;
public:
    AttributeArray();
    explicit AttributeArray(const AttributeSet& set);
    AttributeArray(const AttributeSet& set, const std::vector<AttributeId>& mask);
    AttributeArray(const AttributeArray& other, const std::vector<AttributeId>& mask);
    AttributeArray(const AttributeArray&) = default;
    AttributeArray(AttributeArray&&) = default;
    AttributeArray& operator=(const AttributeArray&) = default;
    AttributeArray& operator=(AttributeArray&&) = default;

    bool operator==(const AttributeArray& rhs) const;
    bool operator!=(const AttributeArray& rhs) const;

    AttributeArray& operator+=(const AttributeArray& rhs);
    AttributeArray& operator-=(const AttributeArray& rhs);
    AttributeArray operator+(const AttributeArray& rhs) const;
    AttributeArray operator-(const AttributeArray& rhs) const;

    AttributeArray& operator+=(const Attribute& rhs);
    AttributeArray& operator-=(const Attribute& rhs);
    AttributeArray operator+(const Attribute& rhs) const;
    AttributeArray operator-(const Attribute& rhs) const;

    ValueType operator[](AttributeId id) const;

    void clear();
};