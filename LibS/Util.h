#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include "Geometry.h"

using namespace Geo;

class Util
{
public:
    template <class T>
    static inline int fastFloor(const T& value)
    {
        int i = (int)value; /* truncate */
        return i - (value < 0.0);   /* convert trunc to floor */
    }
    template <class T>
    static inline Vec2I fastFloor(const Vec2<T>& value)
    {
        return Vec2I(fastFloor(value.x), fastFloor(value.y));
    }
    template <class T>
    static inline int mod(const T& value, const T& div)
    {
        int r = value % div;
        if(r < 0) r += div;
        return r;
    }
    template <class T>
    static inline float mod(const float value, const T& div)
    {
        float r = fmod(value, div);
        if(r < 0) r += div;
        return r;
    }
    template <class T, std::size_t N>
    static inline float size(T(&)[N])
    {
        return N;
    }

    template <class T>
    class Range
    {
    public:
        T min, max;
        Range(const T& _min, const T& _max) : min(_min), max(_max)
        {

        }
        Range() : min(0), max(0)
        {

        }
    };
    typedef Range<float> RangeF;
    typedef Range<double> RangeD;
    typedef Range<int> RangeI;

    template <class T>
    class IterableRange
    {
    public:
        struct Iter
        {
            typedef Iter self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef std::random_access_iterator_tag iterator_category;
            typedef int difference_type;

            Iter(const T& v, const T& s) : m_ptr(v), m_step(s) {}

            self_type       operator++ (int)                  { self_type i = *this; m_ptr += m_step; return i;       }
            self_type       operator-- (int)                  { self_type i = *this; m_ptr -= m_step; return i;       }
            self_type&      operator++ ()                     { m_ptr += m_step; return *this;                        }
            self_type&      operator-- ()                     { m_ptr -= m_step; return *this;                        }

            self_type       operator+ (int n)                { self_type i = *this; i.m_ptr += m_step * n; return i; }
            self_type       operator- (int n)                { self_type i = *this; i.m_ptr -= m_step * n; return i; }
            self_type&      operator+= (int n)                { m_ptr += m_step * n; return *this;                    }
            self_type&      operator-= (int n)                { m_ptr -= m_step * n; return *this;                    }

            difference_type operator- (const self_type& n)   { return (m_ptr - n.m_ptr) / m_step;                    }

            reference       operator[](int n)                { return m_ptr + m_step * n;                            }
            reference       operator* ()                     { return m_ptr;                                         }

            bool            operator== (const self_type& rhs) { return m_ptr == rhs.m_ptr;                            }
            bool            operator< (const self_type& rhs) { return m_ptr <  rhs.m_ptr;                            }
            bool            operator> (const self_type& rhs) { return m_ptr >  rhs.m_ptr;                            }
            bool            operator<= (const self_type& rhs) { return m_ptr <= rhs.m_ptr;                            }
            bool            operator>= (const self_type& rhs) { return m_ptr >= rhs.m_ptr;                            }
            bool            operator!= (const self_type& rhs) { return m_ptr != rhs.m_ptr;                            }
        private:

            T m_ptr, m_step;
        };
        T min, max, step;
        IterableRange() : min(0), max(0), step(1) {}
        IterableRange(const T& _min, const T& _max, const T& _step = T(1)) : min(_min), max(_max), step(_step)
        {

        }
        const Iter begin() const
        {
            return Iter(min, step);
        }
        const Iter end() const
        {
            return Iter(max / step * step + step, step);
        }
    private:
    };
    typedef IterableRange<float> IterableRangeF;
    typedef IterableRange<double> IterableRangeD;
    typedef IterableRange<int> IterableRangeI;

    template <class It>
    class IteratorRange
    {
    public:
        IteratorRange(const It& begin, const It& end) : m_begin(begin), m_end(end)
        {

        }
        const It& begin() const
        {
            return m_begin;
        }
        const It& end() const
        {
            return m_end;
        }
        It begin()
        {
            return m_begin;
        }
        It end()
        {
            return m_end;
        }
    private:
        It m_begin;
        It m_end;
    };

protected:
private:
    Util() {}
    virtual ~Util() {}

};

#endif // UTIL_H
