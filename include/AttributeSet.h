#pragma once

#include <vector>

#include "Attribute.h"

class AttributeSet
{
public:
    using Iterator = typename std::vector<Attribute>::iterator;
    using ConstIterator = typename std::vector<Attribute>::const_iterator;
private:
    std::vector<Attribute> m_attributes;

public:
    AttributeSet() = default;
    AttributeSet(const AttributeSet&) = default;
    AttributeSet(AttributeSet&&) = default;
    AttributeSet& operator=(const AttributeSet&) = default;
    AttributeSet& operator=(AttributeSet&&) = default;

    bool operator==(const AttributeSet& rhs) const;
    bool operator!=(const AttributeSet& rhs) const;

    AttributeSet& operator+=(const AttributeSet& rhs);
    AttributeSet& operator-=(const AttributeSet& rhs);
    AttributeSet operator+(const AttributeSet& rhs) const;
    AttributeSet operator-(const AttributeSet& rhs) const;

    AttributeSet& operator+=(const Attribute& rhs);
    AttributeSet& operator-=(const Attribute& rhs);
    AttributeSet operator+(const Attribute& rhs) const;
    AttributeSet operator-(const Attribute& rhs) const;

    bool isEmpty() const;
    bool isPresent(AttributeId id) const;
    int value(AttributeId id) const;
    ConstIterator find(AttributeId id) const;

    ConstIterator cbegin() const;
    ConstIterator begin() const;
    Iterator begin();
    ConstIterator cend() const;
    ConstIterator end() const;
    Iterator end();

    void clear();
};