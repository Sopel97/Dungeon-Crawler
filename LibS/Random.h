#ifndef RANDOM_H_INCLUDED
#define RANDOM_H_INCLUDED

#include <memory>
#include "make_unique.h"

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
