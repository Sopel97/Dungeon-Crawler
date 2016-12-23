#pragma once

#include <type_traits>
#include <cstdint>

namespace ls
{
    template <class T, int N>
    class BinomialCoefficients
    {
        using ValueType = T;
        static constexpr int size = N;

        static_assert(std::is_integral<T>::value, "T must be an integral type");
    public:
        static const T& value(T n, T k);
    protected:
        BinomialCoefficients();
        static BinomialCoefficients<T, N>& instance();

        T m_coefficients[N][N];
    };

    using BinomialCoefficients64 = BinomialCoefficients<uint64_t, 64>; //the limit of 64 bits is at around 67 choose 33
}

#include "../src/BinomialCoefficients.cpp"
