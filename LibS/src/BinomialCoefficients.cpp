#include "../include/BinomialCoefficients.h"

namespace ls
{
    template <class T, int N>
    BinomialCoefficients<T, N>::BinomialCoefficients()
    {
        m_coefficients[0][0] = T(1);
        for(int y = 1; y < N; ++y)
        {
            for(int x = 0; x <= y; ++x)
            {
                const T& left = x > 0 ? m_coefficients[y - 1][x - 1] : 0;
                const T& right = x < y ? m_coefficients[y - 1][x] : 0;
                m_coefficients[y][x] = left + right;
            }
        }
    }

    template <class T, int N>
    const T& BinomialCoefficients<T, N>::value(T n, T k)
    {
        return instance().m_coefficients[n][k];
    }

    template <class T, int N>
    BinomialCoefficients<T, N>& BinomialCoefficients<T, N>::instance()
    {
        static BinomialCoefficients<T, N> inst{};
        return inst;
    }
}



