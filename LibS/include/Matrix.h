#ifndef MATRIX_H
#define MATRIX_H

template <class T, size_t C, size_t R>
class Matrix
{
public:
    typedef typename std::conditional<C == 2, Vec2<T>, typename std::conditional<C == 3, Vec3<T>, Vec4<T>>> VecTypeCol;
    typedef typename std::conditional<R == 2, Vec2<T>, typename std::conditional<R == 3, Vec3<T>, Vec4<T>>> VecTypeRow;

    Matrix() = default;
    Matrix(T value);
    Matrix(std::initializer_list<VecTypeCol> cols);

    Matrix(const Matrix<T, C, R>& other) = default;
    Matrix(Matrix<T, C, R>&& other) = default;

    Matrix<T, C, R>& operator=(const Matrix<T, C, R>& other) = default;
    Matrix<T, C, R>& operator=(Matrix<T, C, R> && other) = default;

    const VecTypeCol& operator[](size_t c) const;
    VecTypeCol& operator[](size_t c);

    bool operator ==(const Matrix<T, C, R>& other) const;
    bool operator !=(const Matrix<T, C, R>& other) const;

    Matrix<T, C, R> operator+(const Matrix<T, C, R>& other) const;
    Matrix<T, C, R> operator+(T scalar) const;
    Matrix<T, C, R> operator-(const Matrix<T, C, R>& other) const;
    Matrix<T, C, R> operator-(T scalar) const;
    Matrix<T, C, R> operator-() const;
    template <size_t X>
    Matrix<T, X, R> operator*(const Matrix<T, X, C>& other) const;
    VecTypeCol operator*(const VecTypeRow& vec) const;
    Matrix<T, C, R> operator*(T scalar) const;
    Matrix<T, C, R> operator/(T scalar) const;


    Matrix<T, C, R>& operator+=(const Matrix<T, C, R>& other);
    Matrix<T, C, R>& operator+=(T scalar);
    Matrix<T, C, R>& operator-=(const Matrix<T, C, R>& other);
    Matrix<T, C, R>& operator-=(T scalar);
    Matrix<T, C, R>& operator*=(T scalar);
    Matrix<T, C, R>& operator/=(T scalar);

    T sum() const;
    T product() const;
    T mean() const;
    T min() const;
    T max() const;
    T trace() const;
    T determinant() const;
    VecTypeCol diagonal() const;

    void fill(T value);

private:
    VecTypeCol m_columns[C];
};

//TODO: predefined types for every matrix from 2x2 to 4x4 and for types int, float, double

typedef Matrix<double, 2, 2> Matrix2x2D;
typedef Matrix<float, 2, 2> Matrix2x2F;
typedef Matrix<int, 2, 2> Matrix2x2I;

typedef Matrix<double, 2, 3> Matrix2x3D;
typedef Matrix<float, 2, 3> Matrix2x3F;
typedef Matrix<int, 2, 3> Matrix2x3I;

typedef Matrix<double, 2, 4> Matrix2x4D;
typedef Matrix<float, 2, 4> Matrix2x4F;
typedef Matrix<int, 2, 4> Matrix2x4I;

typedef Matrix<double, 3, 2> Matrix3x2D;
typedef Matrix<float, 3, 2> Matrix3x2F;
typedef Matrix<int, 3, 2> Matrix3x2I;

typedef Matrix<double, 3, 3> Matrix3x3D;
typedef Matrix<float, 3, 3> Matrix3x3F;
typedef Matrix<int, 3, 3> Matrix3x3I;

typedef Matrix<double, 3, 4> Matrix3x4D;
typedef Matrix<float, 3, 4> Matrix3x4F;
typedef Matrix<int, 3, 4> Matrix3x4I;

typedef Matrix<double, 4, 2> Matrix4x2D;
typedef Matrix<float, 4, 2> Matrix4x2F;
typedef Matrix<int, 4, 2> Matrix4x2I;

typedef Matrix<double, 4, 3> Matrix4x3D;
typedef Matrix<float, 4, 3> Matrix4x3F;
typedef Matrix<int, 4, 3> Matrix4x3I;

typedef Matrix<double, 4, 4> Matrix4x4D;
typedef Matrix<float, 4, 4> Matrix4x4F;
typedef Matrix<int, 4, 4> Matrix4x4I;

#include "../src/Matrix.cpp"

#endif // MATRIX_H
