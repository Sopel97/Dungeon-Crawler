#pragma once

#include <vector>

#include "TileAttribute.h"

class TileAttributeSet
{
public:
    using Iterator = typename std::vector<TileAttribute>::iterator;
    using ConstIterator = typename std::vector<TileAttribute>::const_iterator;
private:
    std::vector<TileAttribute> m_attributes;

public:
    TileAttributeSet() = default;
    TileAttributeSet(const TileAttributeSet&) = default;
    TileAttributeSet(TileAttributeSet&&) = default;
    TileAttributeSet& operator=(const TileAttributeSet&) = default;
    TileAttributeSet& operator=(TileAttributeSet&&) = default;

    TileAttributeSet& operator+=(const TileAttributeSet& rhs);
    TileAttributeSet& operator-=(const TileAttributeSet& rhs);
    TileAttributeSet operator+(const TileAttributeSet& rhs) const;
    TileAttributeSet operator-(const TileAttributeSet& rhs) const;

    TileAttributeSet& operator+=(const TileAttribute& rhs);
    TileAttributeSet& operator-=(const TileAttribute& rhs);
    TileAttributeSet operator+(const TileAttribute& rhs) const;
    TileAttributeSet operator-(const TileAttribute& rhs) const;

    bool isEmpty() const;
    bool isPresent(TileAttributeId id) const;
    int value(TileAttributeId id) const;
    ConstIterator find(TileAttributeId id) const;

    ConstIterator cbegin() const;
    ConstIterator begin() const;
    Iterator begin();
    ConstIterator cend() const;
    ConstIterator end() const;
    Iterator end();

    void clear();
};