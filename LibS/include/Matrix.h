#pragma once

#include "..\Fwd.h"

#include "../Util.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace ls
{
    template <class T, int R, int C>
    class Matrix
    {
        static_assert(std::is_arithmetic<T>::value, "T must be of arithmetic type");
        static_assert(R >= 2 && R <= 4 && C >= 2 && C <= 4, "Matrix must not be smaller than 2x2 nor bigger than 4x4.");
    public:
        using ValueType = T;
        static constexpr int rows = R;
        static constexpr int columns = C;

        using ColVecType = typename Util::VectorType<T, R>::type;
        using RowVecType = typename Util::VectorType<T, C>::type;

        static Matrix<T, R, C> zero();
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        static Matrix<T, R, C> identity();

        constexpr Matrix() = default;
        explicit Matrix(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value);
        constexpr explicit Matrix(const std::array<std::array<T, C>, R>& values) noexcept(std::is_nothrow_copy_constructible<std::array<std::array<T, C>, R>>::value);
        constexpr explicit Matrix(std::array<std::array<T, C>, R>&& values) noexcept(std::is_nothrow_move_constructible<std::array<std::array<T, C>, R>>::value);

        constexpr Matrix(const Matrix<T, R, C>& other) = default;
        constexpr Matrix(Matrix<T, R, C>&& other) = default;

        Matrix<T, R, C>& operator=(const Matrix<T, R, C>& other) & = default;
        Matrix<T, R, C>& operator=(Matrix<T, R, C> && other) & = default;

        constexpr const T& operator()(int row, int col) const;
        T& operator()(int row, int col);

        bool operator ==(const Matrix<T, R, C>& rhs) const;
        bool operator !=(const Matrix<T, R, C>& rhs) const;

        Matrix<T, R, C> operator+(const Matrix<T, R, C>& rhs) const;
        Matrix<T, R, C> operator+(const T& rhs) const;
        Matrix<T, R, C> operator-(const Matrix<T, R, C>& rhs) const;
        Matrix<T, R, C> operator-(const T& rhs) const;
        Matrix<T, R, C> operator-() const;
        template <int C2>
        Matrix<T, R, C2> operator*(const Matrix<T, C, C2>& rhs) const;
        ColVecType operator*(const RowVecType& rhs) const;
        Matrix<T, R, C> operator*(const T& rhs) const;
        Matrix<T, R, C> operator/(const T& rhs) const;


        Matrix<T, R, C>& operator+=(const Matrix<T, R, C>& rhs) &;
        Matrix<T, R, C>& operator+=(const T& rhs) &;
        Matrix<T, R, C>& operator-=(const Matrix<T, R, C>& rhs) &;
        Matrix<T, R, C>& operator-=(const T& rhs) &;
        Matrix<T, R, C>& operator*=(const T& rhs) &;
        Matrix<T, R, C>& operator/=(const T& rhs) &;

        T sum() const;
        T product() const;
        T mean() const;
        T min() const;
        T max() const;
        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        T trace() const;

        template <int Rows = R, int Cols = C, class SFINAE = typename std::enable_if<Rows == Cols, T>::type>
        T determinant() const;

        template <class E = std::enable_if<R == C, T>>
        typename E::type diagonal() const;

        void fill(const T& value) &;

    private:

        std::array<std::array<T, C>, R> m_values; //as array of rows
    };

    //TODO: predefined types for every matrix from 2x2 to 4x4 and for types int, float, double

    using Matrix2x2D = Matrix<double, 2, 2>;
    using Matrix2x2F = Matrix<float, 2, 2>;
    using Matrix2x2I = Matrix<int, 2, 2>;

    using Matrix2x3D = Matrix<double, 2, 3>;
    using Matrix2x3F = Matrix<float, 2, 3>;
    using Matrix2x3I = Matrix<int, 2, 3>;

    using Matrix2x4D = Matrix<double, 2, 4>;
    using Matrix2x4F = Matrix<float, 2, 4>;
    using Matrix2x4I = Matrix<int, 2, 4>;

    using Matrix3x2D = Matrix<double, 3, 2>;
    using Matrix3x2F = Matrix<float, 3, 2>;
    using Matrix3x2I = Matrix<int, 3, 2>;

    using Matrix3x3D = Matrix<double, 3, 3>;
    using Matrix3x3F = Matrix<float, 3, 3>;
    using Matrix3x3I = Matrix<int, 3, 3>;

    using Matrix3x4D = Matrix<double, 3, 4>;
    using Matrix3x4F = Matrix<float, 3, 4>;
    using Matrix3x4I = Matrix<int, 3, 4>;

    using Matrix4x2D = Matrix<double, 4, 2>;
    using Matrix4x2F = Matrix<float, 4, 2>;
    using Matrix4x2I = Matrix<int, 4, 2>;

    using Matrix4x3D = Matrix<double, 4, 3>;
    using Matrix4x3F = Matrix<float, 4, 3>;
    using Matrix4x3I = Matrix<int, 4, 3>;

    using Matrix4x4D = Matrix<double, 4, 4>;
    using Matrix4x4F = Matrix<float, 4, 4>;
    using Matrix4x4I = Matrix<int, 4, 4>;
}

#include "../src/Matrix.cpp"
