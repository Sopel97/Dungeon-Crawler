#ifndef MAKE_UNIQUE_H_INCLUDED
#define MAKE_UNIQUE_H_INCLUDED

#include <memory>

namespace std //because it's not in C++11
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&& ... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

#endif // MAKE_UNIQUE_H_INCLUDED
