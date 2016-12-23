#include "../include/Index2.h"

namespace ls
{

    template <class T>
    Index2Template<T> Index2Template<T>::ordered(T i_, T j_)
    {
        Index2Template<T> indices(i_, j_);
        indices.sort();
        return indices;
    }

    template <class T>
    Index2Template<T>::Index2Template(T i_, T j_) :
        i(i_),
        j(j_)
    {

    }

    template <class T>
    void Index2Template<T>::sort() &
    {
        if(i > j) std::swap(i, j);
    }

    template <class T>
    bool operator==(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return std::tie(lhs.i, lhs.j) == std::tie(rhs.i, rhs.j);
    }
    template <class T>
    bool operator!=(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return std::tie(lhs.i, lhs.j) < std::tie(rhs.i, rhs.j);
    }
    template <class T>
    bool operator>(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator<=(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>=(const Index2Template<T>& lhs, const Index2Template<T>& rhs)
    {
        return !(lhs < rhs);
    }

}