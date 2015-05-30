#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <memory>
namespace std //because it's not in C++11
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&& ... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}
namespace Random
{
    class RandomEngineBase;
    template <typename IntType = uint32_t> class RandomEngineT;

    template <int I = 0> class CMWCEngineT;
    template <int I = 0> class Xorshift32EngineT; //32 bit version

    template <int I = 0> class Xorshift64EngineT;
    template <int I = 0> class Xorshift128EngineT;
    template <int I = 0> class Xorshift1024EngineT;

    template <int I = 0> class WELL512EngineT;
    template <int I = 0> class WELL1024EngineT;

#include "include/RandomEngineBase.h"
#include "include/RandomEngine.h"
#include "include/CMWCEngine.h"

#include "include/Xorshift32Engine.h"
#include "include/Xorshift64Engine.h"
#include "include/Xorshift128Engine.h"
#include "include/Xorshift1024Engine.h"

#include "include/WELL512Engine.h"
#include "include/WELL1024Engine.h"

}

#endif // RANDOM_H_INCLUDED
