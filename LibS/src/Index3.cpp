#include "../include/Index3.h"

namespace ls
{

    template <class T>
    Index3Template<T> Index3Template<T>::ordered(T i_, T j_, T k_) noexcept
    {
        Index3Template<T> indices(i_, j_, k_);
        indices.sort();
        return indices;
    }

    template <class T>
    Index3Template<T>::Index3Template(T i_, T j_, T k_) noexcept :
    i(i_),
        j(j_),
        k(k_)
    {

    }

    template <class T>
    void Index3Template<T>::sort() &
    {
        if(j > k) std::swap(j, k);
        if(i > k) std::swap(i, k);
        if(i > j) std::swap(i, j);
    }

    template <class T>
    bool operator==(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return std::tie(lhs.i, lhs.j, lhs.k) == std::tie(rhs.i, rhs.j, rhs.k);
    }
    template <class T>
    bool operator!=(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return std::tie(lhs.i, lhs.j, lhs.k) < std::tie(rhs.i, rhs.j, rhs.k);
    }
    template <class T>
    bool operator<=(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator>=(const Index3Template<T>& lhs, const Index3Template<T>& rhs)
    {
        return !(lhs < rhs);
    }
}