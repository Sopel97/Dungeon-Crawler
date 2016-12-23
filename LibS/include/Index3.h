#pragma once

namespace ls
{
    template <class T>
    class Index3Template
    {
    public:
        T i, j, k;

        static Index3Template<T> ordered(T i_, T j_, T k_) noexcept;

        Index3Template(T i_, T j_, T k_) noexcept;

        Index3Template(const Index3Template<T>& other) = default;
        Index3Template(Index3Template<T>&& other) = default;

        Index3Template<T>& operator=(const Index3Template<T>& other) & = default;
        Index3Template<T>& operator=(Index3Template<T>&& other) & = default;

        void sort() &;
    };

    template <class T>
    bool operator==(const Index3Template<T>& lhs, const Index3Template<T>& rhs);
    template <class T>
    bool operator!=(const Index3Template<T>& lhs, const Index3Template<T>& rhs);
    template <class T>
    bool operator<(const Index3Template<T>& lhs, const Index3Template<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const Index3Template<T>& lhs, const Index3Template<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const Index3Template<T>& lhs, const Index3Template<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const Index3Template<T>& lhs, const Index3Template<T>& rhs); //compares pointers lexicographically

    using Index3 = Index3Template<int>;
}

#include "../src/Index3.cpp"
