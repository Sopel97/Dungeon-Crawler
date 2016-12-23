#include "..\include\Matrix.h"

namespace ls
{
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::zero()
    {
        return Matrix<T, R, C>(T(0));
    }
    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    Matrix<T, R, C> Matrix<T, R, C>::identity()
    {
        Matrix<T, R, C> matrix(T(0));

        for(int i = 0; i < R; ++i)
            matrix.m_values[i][i] = T(1);

        return matrix;
    }


    template <class T, int R, int C>
    Matrix<T, R, C>::Matrix(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value)
    {
        fill(value);
    }
    template <class T, int R, int C>
    constexpr Matrix<T, R, C>::Matrix(const std::array<std::array<T, C>, R>& values) noexcept(std::is_nothrow_copy_constructible<std::array<std::array<T, C>, R>>::value) :
        m_values(values)
    {

    }
    template <class T, int R, int C>
    constexpr Matrix<T, R, C>::Matrix(std::array<std::array<T, C>, R>&& values) noexcept(std::is_nothrow_move_constructible<std::array<std::array<T, C>, R>>::value) :
        m_values(std::move(values))
    {

    }

    template <class T, int R, int C>
    constexpr const T& Matrix<T, R, C>::operator()(int row, int col) const
    {
        return m_values[row][col];
    }
    template <class T, int R, int C>
    T& Matrix<T, R, C>::operator()(int row, int col)
    {
        return m_values[row][col];
    }

    template <class T, int R, int C>
    bool Matrix<T, R, C>::operator ==(const Matrix<T, R, C>& rhs) const
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                if(m_values[r][c] != rhs.m_values[r][c]) return false;
            }
        }
        return true;
    }

    template <class T, int R, int C>
    bool Matrix<T, R, C>::operator !=(const Matrix<T, R, C>& rhs) const
    {
        return !(*this == rhs);
    }

    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator+(const Matrix<T, R, C>& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result += rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator+(const T& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result += rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator-(const Matrix<T, R, C>& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result -= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator-(const T& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result -= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator-() const
    {
        Matrix<T, R, C> result(*this);
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                result.m_values[r][c] = -result.m_values[r][c];
            }
        }
        return result;
    }
    template <class T, int R, int C>
    template <int C2>
    Matrix<T, R, C2> Matrix<T, R, C>::operator*(const Matrix<T, C, C2>& rhs) const
    {
        Matrix<T, R, C2> result;
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C2; ++c)
            {
                T resultElement = T(0);
                for(int i = 0; i < C; ++i)
                {
                    resultElement += m_values[r][i] * rhs.m_values[i][c];
                }
                result[r][c] = resultElement;
            }
        }
        return result;
    }
    template <class T, int R, int C>
    typename Matrix<T, R, C>::ColVecType Matrix<T, R, C>::operator*(const typename Matrix<T, R, C>::RowVecType& rhs) const
    {
        Matrix<T, R, C>::ColVecType result;
        for(int r = 0; r < R; ++r)
        {
            T resultElement = 0;
            for(int i = 0; i < C; ++i)
            {
                resultElement += m_values[r][i] * rhs[i];
            }
            result[r] = resultElement;
        }
        return result;

    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator*(const T& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result *= rhs);
    }
    template <class T, int R, int C>
    Matrix<T, R, C> Matrix<T, R, C>::operator/(const T& rhs) const
    {
        Matrix<T, R, C> result(*this);
        return (result /= rhs);
    }

    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const Matrix<T, R, C>& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] += rhs.m_values[r][c];
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator+=(const T& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] += rhs;
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const Matrix<T, R, C>& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] -= rhs.m_values[r][c];
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator-=(const T& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] -= rhs;
            }
        }
        return *this;
    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator*=(const T& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] *= rhs;
            }
        }
        return *this;

    }
    template <class T, int R, int C>
    Matrix<T, R, C>& Matrix<T, R, C>::operator/=(const T& rhs) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] /= rhs;
            }
        }
        return *this;
    }

    template <class T, int R, int C>
    T Matrix<T, R, C>::sum() const
    {
        T sumOfElements = 0;
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                sumOfElements += m_values[r][c];
            }
        }
        return sumOfElements;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::product() const
    {
        T productOfElements = 1;
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                productOfElements *= m_values[r][c];
            }
        }
        return productOfElements;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::mean() const
    {
        return sum() / (R * C);
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::min() const
    {
        T minElement = m_values[0][0];
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                if(m_values[r][c] < minElement) minElement = m_values[r][c];
            }
        }
        return minElement;
    }
    template <class T, int R, int C>
    T Matrix<T, R, C>::max() const
    {
        T maxElement = m_values[0][0];
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                if(m_values[r][c] > maxElement) maxElement = m_values[r][c];
            }
        }
        return maxElement;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    T Matrix<T, R, C>::trace() const
    {
        T sumOfDiagonal = 0;
        for(int i = 0; i < R; ++i) sumOfDiagonal += m_values[i][i];
        return sumOfDiagonal;
    }

    template <class T, int R, int C>
    template <int Rows, int Cols, class SFINAE>
    T Matrix<T, R, C>::determinant() const
    {
        if(R == 2)
        {
            return m_values[0][0] * m_values[1][1] - m_values[0][1] * m_values[1][0];
        }
        else if(R == 3)
        {
            return 0;
        }
        else if(R == 4)
        {
            return 0;
        }
    }

    template <class T, int R, int C>
    template <class E>
    typename E::type Matrix<T, R, C>::diagonal() const
    {
        Matrix<T, R, C>::RowVecType diagonalVector;
        for(int i = 0; i < R; ++i) diagonalVector[i] = m_values[i][i];
        return diagonalVector;
    }

    template <class T, int R, int C>
    void Matrix<T, R, C>::fill(const T& value) &
    {
        for(int r = 0; r < R; ++r)
        {
            for(int c = 0; c < C; ++c)
            {
                m_values[r][c] = value;
            }
        }
    }
}

