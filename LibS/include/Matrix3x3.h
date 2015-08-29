#ifndef MATRIX3X3_H
#define MATRIX3X3_H

//Will be removed soon
template <class T>
class Matrix3x3
{
public:
    Matrix3x3() = default;
    Matrix3x3(const Matrix3x3<T>&) = default;
    Matrix3x3(Matrix3x3<T>&&) = default;

    Matrix3x3(std::initializer_list<std::initializer_list<T>> matrixAsLists);
    Matrix3x3(const Vec3<T>& v1, const Vec3<T>& v2, const Vec3<T>& v3);

    Matrix3x3<T>& operator=(const Matrix3x3<T>&) = default;
    Matrix3x3<T>& operator=(Matrix3x3<T> &&) = default;

    Matrix3x3<T>& operator+=(const Matrix3x3<T>& otherMatrix);
    Matrix3x3<T>& operator-=(const Matrix3x3<T>& otherMatrix);
    Matrix3x3<T>& operator*=(const Matrix3x3<T>& otherMatrix);

    Matrix3x3<T>& operator*=(const T& scalar);
    Matrix3x3<T>& operator/=(const T& scalar);

    Matrix3x3<T> operator+(const Matrix3x3<T>& otherMatrix) const;
    Matrix3x3<T> operator-(const Matrix3x3<T>& otherMatrix) const;
    Matrix3x3<T> operator*(const Matrix3x3<T>& otherMatrix) const;

    Matrix3x3<T> operator*(const T& scalar) const;
    Matrix3x3<T> operator/(const T& scalar) const;

    Matrix3x3<T> operator-() const;

    bool operator==(const Matrix3x3<T>& otherMatrix) const;
    bool operator!=(const Matrix3x3<T>& otherMatrix) const;

    Vec3<T> operator*(const Vec3<T>& vector) const;

    T(&operator[](size_t row))[3]{return m_elements[row];};  //move it to cpp file

    void inverse();
    void transpose();
    Vec3<T> diagonal() const;
    T determinant() const;

    Matrix3x3<T> inversed() const;
    Matrix3x3<T> transposed() const;
private:
    constexpr static size_t SIZE = 3u;
    T m_elements[SIZE][SIZE];
};

#include "../src/Matrix3x3.cpp"

#endif // MATRIX3X3_H
