#pragma once

namespace ls
{
    template <class T>
    class Index2Template
    {
    public:
        T i, j;

        static Index2Template ordered(T _i, T _j);

        Index2Template(T _i, T _j);

        Index2Template(const Index2Template<T>& other) noexcept = default;
        Index2Template(Index2Template<T>&& other) noexcept = default;

        Index2Template<T>& operator=(const Index2Template<T>& other) & noexcept = default;
        Index2Template<T>& operator=(Index2Template<T>&& other) & noexcept = default;

        void sort() &;
    };

    template <class T>
    bool operator==(const Index2Template<T>& lhs, const Index2Template<T>& rhs);
    template <class T>
    bool operator!=(const Index2Template<T>& lhs, const Index2Template<T>& rhs);
    template <class T>
    bool operator<(const Index2Template<T>& lhs, const Index2Template<T>& rhs); //compares indices lexicographically
    template <class T>
    bool operator>(const Index2Template<T>& lhs, const Index2Template<T>& rhs); //compares indices lexicographically
    template <class T>
    bool operator<=(const Index2Template<T>& lhs, const Index2Template<T>& rhs); //compares indices lexicographically
    template <class T>
    bool operator>=(const Index2Template<T>& lhs, const Index2Template<T>& rhs); //compares indices lexicographically

    using Index2 = Index2Template<int>;

}

#include "../src/Index2.cpp"
