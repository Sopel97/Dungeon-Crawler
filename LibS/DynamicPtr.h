#pragma once

#include <memory>

namespace ls
{
    template <class T>
    class DynamicPtr;

    template <class T>
    void swap(DynamicPtr<T>& lhs, DynamicPtr<T>& rhs);

    template <class T>
    class DynamicPtr
    {
    public:
        constexpr DynamicPtr() noexcept :
            m_ptr(nullptr),
            m_isOwning(false)
        {

        }

        constexpr DynamicPtr(nullptr_t) noexcept :
        DynamicPtr()
        {

        }

        DynamicPtr(T* ptr) :
            m_ptr(ptr),
            m_isOwning(false)
        {

        }

        DynamicPtr(std::unique_ptr<T>&& ptr) noexcept :
            m_ptr(ptr.release()),
            m_isOwning(true)
        {

        }

        DynamicPtr(const DynamicPtr<T>& other) :
            m_ptr(other.m_ptr),
            m_isOwning(false)
        {

        }

        DynamicPtr(DynamicPtr<T>&& other) noexcept :
        m_ptr(other.m_ptr),
            m_isOwning(other.m_isOwning)
        {
            other.m_ptr = nullptr;
            other.m_isOwning = false;
        }

        DynamicPtr<T>& operator=(const DynamicPtr<T>& other) &
        {
            deleteIfOwning();

            m_ptr = other.m_ptr;
            m_isOwning = false;

            return *this;
        }

        DynamicPtr<T>& operator=(T* ptr) &
        {
            deleteIfOwning();

            m_ptr = ptr;
            m_isOwning = false;

            return *this;
        }

        DynamicPtr<T>& operator=(DynamicPtr<T>&& other) & noexcept
        {
            deleteIfOwning();

            m_ptr = other.m_ptr;
            m_isOwning = other.m_isOwning;

            other.m_ptr = nullptr;
            other.m_isOwning = false;

            return *this;
        }

        DynamicPtr<T>& operator=(nullptr_t) &
        {
            deleteIfOwning();

            m_ptr = ptr;
            m_isOwning = false;

            return *this;
        }

        DynamicPtr<T> operator=(std::unique_ptr<T>&& ptr) & noexcept
        {
            deleteIfOwning();

            m_ptr = ptr.release();
            m_isOwning = true;

            return *this;
        }

        void reset()
        {
            deleteIfOwning();

            m_ptr = nullptr;
            m_isOwning = false;
        }

        void swap(DynamicPtr<T>& other)
        {
            swap(*this, other);
        }

        bool isOwning() const
        {
            return m_isOwning;
        }

        T* get() const
        {
            return m_ptr;
        }

        explicit operator bool() const
        {
            return m_ptr != nullptr;
        }

        T& operator*() const
        {
            return *m_ptr;
        }

        T* operator->() const
        {
            return m_ptr;
        }

        ~DynamicPtr()
        {
            deleteIfOwning();
        }
    private:
        T* m_ptr;
        bool m_isOwning;

        DynamicPtr(T* ptr, bool isOwning) :
            m_ptr(ptr),
            m_isOwning(isOwning)
        {

        }

        void deleteIfOwning()
        {
            if(m_isOwning) delete m_ptr;
        }
    };

    template <class T>
    void swap(DynamicPtr<T>& lhs, DynamicPtr<T>& rhs)
    {
        DynamicPtr<T> tmp = std::move(lhs);
        lhs = std::move(rhs);
        rhs = std::move(tmp);
    }

    template <class T>
    bool operator==(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return lhs.get() == rhs.get();
    }
    template <class T>
    bool operator!=(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    bool operator<(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return lhs.get() < rhs.get();
    }
    template <class T>
    bool operator>(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return rhs.get() < lhs.get();
    }
    template <class T>
    bool operator<=(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return !(rhs.get() < lhs.get());
    }
    template <class T>
    bool operator>=(const DynamicPtr<T>& lhs, const DynamicPtr<T>& rhs)
    {
        return !(lhs.get() < rhs.get());
    }


    template <class T>
    bool operator==(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return lhs.get() == nullptr;
    }
    template <class T>
    bool operator!=(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return !(lhs == nullptr);
    }

    template <class T>
    bool operator<(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return lhs.get() < nullptr;
    }
    template <class T>
    bool operator>(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return nullptr < lhs.get();
    }
    template <class T>
    bool operator<=(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return !(nullptr < lhs.get());
    }
    template <class T>
    bool operator>=(const DynamicPtr<T>& lhs, nullptr_t)
    {
        return !(lhs.get() < nullptr);
    }


    template <class T>
    bool operator==(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return nullptr == rhs.get();
    }
    template <class T>
    bool operator!=(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return !(nullptr == rhs);
    }

    template <class T>
    bool operator<(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return nullptr < rhs.get();
    }
    template <class T>
    bool operator>(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return rhs.get() < nullptr;
    }
    template <class T>
    bool operator<=(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return !(rhs.get() < nullptr);
    }
    template <class T>
    bool operator>=(nullptr_t, const DynamicPtr<T>& rhs)
    {
        return !(nullptr < rhs.get());
    }
}
