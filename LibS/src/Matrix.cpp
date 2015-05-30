template <class T, size_t C, size_t R>
Matrix<T, C, R>::Matrix(T value)
{
    for(size_t c = 0; c < C; ++c)
    {
        m_columns[c].fill(value);
    }
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>::Matrix(std::initializer_list<Matrix<T, C, R>::VecTypeCol> cols)
{
    for(size_t c = 0; c < C; ++c)
    {
        m_columns[c] = *(cols.begin() + c);
    }
}
template <class T, size_t C, size_t R>
const typename Matrix<T, C, R>::VecTypeCol& Matrix<T, C, R>::operator[](size_t c) const
{
    return m_columns[c];
}
template <class T, size_t C, size_t R>
typename Matrix<T, C, R>::VecTypeCol& Matrix<T, C, R>::operator[](size_t c)
{
    return m_columns[c];
}

template <class T, size_t C, size_t R>
bool Matrix<T, C, R>::operator ==(const Matrix<T, C, R>& other) const
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            if(m_columns[c][r] != other.m_columns[c][r]) return false;
        }
    }
    return true;
}
template <class T, size_t C, size_t R>
bool Matrix<T, C, R>::operator !=(const Matrix<T, C, R>& other) const
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            if(m_columns[c][r] == other.m_columns[c][r]) return false;
        }
    }
    return true;
}

template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator+(const Matrix<T, C, R>& other) const
{
    Matrix<T, C, R> result(*this);
    return (result += other);
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator+(T scalar) const
{
    Matrix<T, C, R> result(*this);
    return (result += scalar);
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator-(const Matrix<T, C, R>& other) const
{
    Matrix<T, C, R> result(*this);
    return (result -= other);
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator-(T scalar) const
{
    Matrix<T, C, R> result(*this);
    return (result -= scalar);
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator-() const
{
    Matrix<T, C, R> result(*this);
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            result.m_columns[c][r] = -result.m_columns[c][r];
        }
    }
    return result;
}
template <class T, size_t C, size_t R>
template <size_t X>
Matrix<T, X, R> Matrix<T, C, R>::operator*(const Matrix<T, X, C>& other) const
{
    Matrix<T, X, R> result;
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < X; ++r)
        {
            T resultElement = 0;
            for(size_t i = 0; i < C; ++i)
            {
                resultElement += m_columns[i][r] * other.m_columns[c][i];
            }
            result[c][r] = resultElement;
        }
    }
    return result;
}
template <class T, size_t C, size_t R>
typename Matrix<T, C, R>::VecTypeCol Matrix<T, C, R>::operator*(const Matrix<T, C, R>::VecTypeRow& vec) const
{
    Matrix<T, C, R>::VecTypeCol result;
    for(size_t r = 0; r < R; ++r)
    {
        T resultElement = 0;
        for(size_t i = 0; i < C; ++i)
        {
            resultElement += m_columns[i][r] * vec[i];
        }
        result[r] = resultElement;
    }
    return result;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator*(T scalar) const
{
    Matrix<T, C, R> result(*this);
    return (result *= scalar);
}
template <class T, size_t C, size_t R>
Matrix<T, C, R> Matrix<T, C, R>::operator/(T scalar) const
{
    Matrix<T, C, R> result(*this);
    return (result /= scalar);
}


template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator+=(const Matrix<T, C, R>& other)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] += other.m_columns[c][r];
        }
    }
    return *this;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator+=(T scalar)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] += scalar;
        }
    }
    return *this;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator-=(const Matrix<T, C, R>& other)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] -= other.m_columns[c][r];
        }
    }
    return *this;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator-=(T scalar)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] -= scalar;
        }
    }
    return *this;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator*=(T scalar)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] *= scalar;
        }
    }
    return *this;
}
template <class T, size_t C, size_t R>
Matrix<T, C, R>& Matrix<T, C, R>::operator/=(T scalar)
{
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            m_columns[c][r] /= scalar;
        }
    }
    return *this;
}

template <class T, size_t C, size_t R>
T Matrix<T, C, R>::sum() const
{
    T sumOfElements = 0;
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            sumOfElements += m_columns[c][r];
        }
    }
    return sumOfElements;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::product() const
{
    T productOfElements = 1;
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            productOfElements *= m_columns[c][r];
        }
    }
    return productOfElements;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::mean() const
{
    return sum() / C * R;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::min() const
{
    T minElement = m_columns[0][0];
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            if(m_columns[c][r] < minElement) minElement = m_columns[c][r];
        }
    }
    return minElement;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::max() const
{
    T maxElement = m_columns[0][0];
    for(size_t c = 0; c < C; ++c)
    {
        for(size_t r = 0; r < R; ++r)
        {
            if(m_columns[c][r] > maxElement) maxElement = m_columns[c][r];
        }
    }
    return maxElement;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::trace() const
{
    static_assert(C == R, "A square matrix is required to make this operation.");
    T sumOfDiagonal = 0;
    for(size_t i = 0; i < C; ++i) sumOfDiagonal += m_columns[i][i];
    return sumOfDiagonal;
}
template <class T, size_t C, size_t R>
T Matrix<T, C, R>::determinant() const
{
    static_assert(C == R, "A square matrix is required to make this operation.");
    return 0; //TODO: determinant calculation
}
template <class T, size_t C, size_t R>
typename Matrix<T, C, R>::VecTypeCol Matrix<T, C, R>::diagonal() const
{
    static_assert(C == R, "A square matrix is required to make this operation.");
    Matrix<T, C, R>::VecTypeCol diagonalVector;
    for(size_t i = 0; i < C; ++i) diagonalVector[i] = m_columns[i][i];
    return diagonalVector;
}

template <class T, size_t C, size_t R>
void Matrix<T, C, R>::fill(T value)
{
    for(size_t c = 0; c < C; ++c)
    {
        m_columns[c].fill(value);
    }
}


