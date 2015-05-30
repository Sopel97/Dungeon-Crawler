#ifndef ARRAY2_H
#define ARRAY2_H
#include <iostream>
#include <vector>
#include <utility>
using std::size_t;

template <class T>
class Array2
{
public:
    class Col
    {
    public:
        typedef T* iterator;
        Col() :
            m_dataCol(nullptr),
            m_colSize(0)
        {
        }

        Col(T* dataCol, size_t size) :
            m_dataCol(dataCol),
            m_colSize(size)
        {
        }

        Col(Col& col) :
            m_dataCol(col.m_dataCol),
            m_colSize(col.m_colSize)
        {
        }

        Col(Col&& col) :
            m_dataCol(col.m_dataCol),
            m_colSize(col.m_colSize)
        {
        }

        ~Col()
        {
        }

        T& operator[] (size_t y) const
        {
            return *(m_dataCol + y);
        }
        T& at(size_t y) const
        {
            return *(m_dataCol + y);
        }
        Col& operator= (Col && col) {
            m_dataCol = col.m_dataCol;
            m_colSize = col.m_colSize;
            return *this;
        }

        Col& operator= (const Col& col)
        {
            return operator= (std::move(Col(col)));
        }

        Col& operator= (const std::initializer_list<T>& list)
        {
            int i = 0;
            for(auto&& v : list)
            {
                operator[](i) = v;
                ++i;
            }
        }

        T* data() const
        {
            return m_dataCol;
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        size_t size() const
        {
            return m_colSize;
        }

        bool empty() const
        {
            return m_dataCol == nullptr;
        }

        iterator begin() const
        {
            return m_dataCol;
        }

        iterator end() const
        {
            return m_dataCol + m_colSize;
        }

        Col previousCol(int n = 1) const
        {
            return Col(m_dataCol - (m_colSize*n), m_colSize);
        }
        Col nextCol(int n = 1) const
        {
            return Col(m_dataCol + (m_colSize*n), m_colSize);
        }
        Col& moveToPreviousCol(int n = 1)
        {
            m_dataCol -= (m_colSize*n);
        }
        Col& moveToNextCol(int n = 1)
        {
            m_dataCol += (m_colSize*n);
        }

    private:
        T* m_dataCol;
        size_t m_colSize;
    };

    class Row
    {
    public:
        class iterator
        {
        public:
            typedef iterator self_type;
            typedef T value_type;
            typedef T& reference;
            typedef T* pointer;
            typedef std::forward_iterator_tag iterator_category;
            typedef int difference_type;

            iterator(pointer ptr, int jump) : m_start(ptr), m_ptr(ptr), m_jump(jump) { }

            self_type       operator++ (int)                  { self_type i = *this; m_ptr += m_jump; return i;       }
            self_type       operator-- (int)                  { self_type i = *this; m_ptr -= m_jump; return i;       }
            self_type&      operator++ ()                     { m_ptr += m_jump; return *this;                        }
            self_type&      operator-- ()                     { m_ptr -= m_jump; return *this;                        }

            self_type       operator+  (int n)                { self_type i = *this; i.m_ptr += m_jump * n; return i; }
            self_type       operator-  (int n)                { self_type i = *this; i.m_ptr -= m_jump * n; return i; }
            self_type&      operator+= (int n)                { m_ptr += m_jump * n; return *this;                    }
            self_type&      operator-= (int n)                { m_ptr -= m_jump * n; return *this;                    }

            difference_type operator-  (const self_type& n)   { return (m_ptr - n.m_ptr) / m_jump;                    }

            reference       operator[] (int n)                { return m_start[m_jump * n];                           }
            reference       operator*  ()                     { return *m_ptr;                                        }
            pointer         operator-> ()                     { return m_ptr;                                         }

            bool            operator== (const self_type& rhs) { return m_ptr == rhs.m_ptr;                            }
            bool            operator<  (const self_type& rhs) { return m_ptr <  rhs.m_ptr;                            }
            bool            operator>  (const self_type& rhs) { return m_ptr >  rhs.m_ptr;                            }
            bool            operator<= (const self_type& rhs) { return m_ptr <= rhs.m_ptr;                            }
            bool            operator>= (const self_type& rhs) { return m_ptr >= rhs.m_ptr;                            }
            bool            operator!= (const self_type& rhs) { return m_ptr != rhs.m_ptr;                            }

        private:
            pointer m_start;
            pointer m_ptr;
            int m_jump;
        };

        Row() :
            m_dataRow(nullptr),
            m_rowSize(0)
        {
        }

        Row(T* dataRow, size_t size, size_t colSize) :
            m_dataRow(dataRow),
            m_rowSize(size),
            m_colSize(colSize)
        {
        }

        Row(Row& row) :
            m_dataRow(row.m_dataRow),
            m_rowSize(row.m_rowSize),
            m_colSize(row.m_colSize)
        {
        }

        Row(Row&& row) :
            m_dataRow(row.m_dataRow),
            m_rowSize(row.m_rowSize),
            m_colSize(row.m_colSize)
        {
        }

        ~Row()
        {
        }

        T& operator[] (size_t x) const
        {
            return *(m_dataRow + x * m_colSize);
        }
        T& at(size_t x) const
        {
            return *(m_dataRow + x * m_colSize);
        }
        Row& operator= (Row && row)
        {
            m_dataRow = row.m_dataRow;
            m_rowSize = row.m_rowSize;
            m_colSize = row.m_colSize;
            return *this;
        }

        Row& operator= (const Row& row)
        {
            return operator= (std::move(Row(row)));
        }

        Row& operator= (const std::initializer_list<T>& list)
        {
            int i = 0;
            for(auto&& v : list)
            {
                operator[](i) = v;
                ++i;
            }
        }

        T* data() const //data is not continguous
        {
            return m_dataRow;
        }

        void fill(const T& value)
        {
            std::fill(begin(), end(), value);
        }

        size_t size() const
        {
            return m_rowSize;
        }

        bool empty() const
        {
            return m_dataRow == nullptr;
        }

        iterator begin() const
        {
            return iterator(m_dataRow, m_colSize);
        }

        iterator end() const
        {
            return iterator(m_dataRow + m_rowSize * m_colSize, m_colSize);
        }
        Row previousRow(int n = 1) const
        {
            return Row(m_dataRow - n, m_rowSize, m_colSize);
        }
        Row nextRow(int n = 1) const
        {
            return Row(m_dataRow + n, m_rowSize, m_colSize);
        }
        Row& moveToPreviousRow(int n = 1)
        {
            m_dataRow -= n;
        }
        Row& moveToNextRow(int n = 1)
        {
            m_dataRow += n;
        }
    private:
        T* m_dataRow;
        size_t m_rowSize;
        size_t m_colSize;
    };

    typedef T* iterator;

    Array2() :
        m_data(nullptr),
        m_sizeX(0),
        m_sizeY(0)
    {
    }

    Array2(size_t sizeX, size_t sizeY) :
        m_sizeX(sizeX),
        m_sizeY(sizeY)
    {
        m_data = new T[m_sizeX * m_sizeY];
    }

    Array2(size_t sizeX, size_t sizeY, const T& initValue) :
        m_sizeX(sizeX),
        m_sizeY(sizeY)
    {
        size_t totalSize = m_sizeX * m_sizeY;
        m_data = new T[totalSize];
        for(size_t i = 0; i < totalSize; ++i)
        {
            m_data[i] = initValue;
        }
    }

    Array2(const Array2<T>& arrayCopy) :
        m_sizeX(arrayCopy.m_sizeX),
        m_sizeY(arrayCopy.m_sizeY)
    {
        size_t totalSize = m_sizeX * m_sizeY;
        m_data = new T[totalSize];
        std::copy(arrayCopy.begin(), arrayCopy.end(), m_data);
    }

    Array2(Array2<T>&& array) :
        m_data(std::move(array.m_data)),
        m_sizeX(array.m_sizeX),
        m_sizeY(array.m_sizeY)
    {
        array.m_data = nullptr;
    }

    Array2(const std::initializer_list<std::initializer_list<T>>& list)
    {
        m_sizeY = list.size();
        m_sizeX = (*(list.begin())).size();
        size_t totalSize = m_sizeX * m_sizeY;
        m_data = new T[totalSize];
        int y = 0;
        for(auto& row : list)
        {
            int x = 0;
            for(auto&& v : row)
            {
                at(x,y) = v;
                ++x;
            }
            ++y;
        }
    }

    ~Array2()
    {
        if(m_data) delete m_data;
    }

    Col operator[] (size_t x) const
    {
        return Col(m_data + x * m_sizeY, m_sizeY);
    }

    T& operator() (size_t x, size_t y) const
    {
        return m_data[x * m_sizeY + y];
    }
    T& at(size_t x, size_t y) const
    {
        return m_data[x * m_sizeY + y];
    }
    Array2& operator= (Array2<T> && array)
    {
        if(m_data) delete m_data;

        m_data = std::move(array.m_data);
        m_sizeX = array.m_sizeX;
        m_sizeY = array.m_sizeY;

        array.m_data = nullptr;

        return *this;
    }

    Array2& operator= (const Array2<T>& arrayCopy)
    {
        return operator=(std::move(Array2(arrayCopy)));
    }

    Array2& operator= (const std::initializer_list<std::initializer_list<T>>& list)
    {
        return operator=(std::move(Array2(list)));
    }
    void fill(const T& value)
    {
        std::fill(begin(), end(), value);
    }

    T* data() const
    {
        return m_data;
    }

    size_t sizeX() const
    {
        return m_sizeX;
    }

    size_t sizeY() const
    {
        return m_sizeY;
    }

    size_t cols() const
    {
        return m_sizeX;
    }

    size_t rows() const
    {
        return m_sizeY;
    }

    std::pair<size_t, size_t> size() const
    {
        return std::pair<size_t, size_t>(m_sizeX, m_sizeY);
    }

    bool empty() const
    {
        return m_data == nullptr;
    }

    iterator begin() const
    {
        return m_data;
    }

    iterator end() const
    {
        size_t totalSize = m_sizeX * m_sizeY;
        return m_data + totalSize;
    }

    Row row(size_t n) const
    {
        return Row(m_data + n, m_sizeX, m_sizeY);
    }

    Col col(size_t n) const
    {
        return operator[](n);
    }

private:
    T* m_data;
    size_t m_sizeX;
    size_t m_sizeY;
};
#endif // ARRAY2_H
