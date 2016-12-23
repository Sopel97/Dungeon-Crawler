#include "..\include\IndexN.h"

namespace ls
{

    template <class T>
    IndexNTemplate<T> IndexNTemplate<T>::ordered(const std::vector<T>& indices)
    {
        IndexNTemplate<T> poly(indices);
        poly.sort();
        return poly;
    }

    template <class T>
    IndexNTemplate<T> IndexNTemplate<T>::ordered(std::vector<T>&& indices) noexcept(std::is_nothrow_constructible<IndexNTemplate<T>, std::vector<T>&&>::value)
    {
        IndexNTemplate<T> poly(std::move(indices));
        poly.sort();
        return poly;
    }

    template <class T>
    IndexNTemplate<T>::IndexNTemplate(const std::vector<T>& _indices) :
        indices(_indices)
    {

    }
    template <class T>
    IndexNTemplate<T>::IndexNTemplate(std::vector<T>&& _indices) noexcept :
    indices(std::move(_indices))
    {

    }

    template <class T>
    void IndexNTemplate<T>::sort() &
    {
        std::sort(indices.begin(), indices.end());
    }

    template <class T>
    bool operator==(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return std::equal(lhs.indices.begin(), lhs.indices.end(), rhs.indices.begin(), rhs.indices.end());
    }
    template <class T>
    bool operator!=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    bool operator<(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return std::lexicographical_compare(lhs.indices.begin(), lhs.indices.end(), rhs.indices.begin(), rhs.indices.end());
    }
    template <class T>
    bool operator<=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    bool operator>(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    bool operator>=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs)
    {
        return !(lhs < rhs);
    }

}
