#pragma once

#include <vector>

namespace ls
{
    template <class T>
    class IndexNTemplate
    {
    public:
        std::vector<T> indices;

        static IndexNTemplate<T> ordered(const std::vector<T>& indices);
        static IndexNTemplate<T> ordered(std::vector<T>&& indices) noexcept(std::is_nothrow_constructible<IndexNTemplate<T>, std::vector<T>&&>::value);

        explicit IndexNTemplate(const std::vector<T>& _indices);
        explicit IndexNTemplate(std::vector<T>&& _indices) noexcept; //crashes optimiser in vc --- (std::is_nothrow_move_constructible<std::vector<int>>::value);

        IndexNTemplate(const IndexNTemplate<T>& other) = default;
        IndexNTemplate(IndexNTemplate<T>&& other) = default;

        IndexNTemplate<T>& operator=(const IndexNTemplate<T>& other) & = default;
        IndexNTemplate<T>& operator=(IndexNTemplate<T>&& other) & = default;

        void sort() &;
    };

    template <class T>
    bool operator==(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs);
    template <class T>
    bool operator!=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs);
    template <class T>
    bool operator<(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const IndexNTemplate<T>& lhs, const IndexNTemplate<T>& rhs); //compares pointers lexicographically
}

#include "..\src\IndexN.cpp"
