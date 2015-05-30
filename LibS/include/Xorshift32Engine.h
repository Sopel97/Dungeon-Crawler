#ifndef XORSHIFTENGINE_H_INCLUDED
#define XORSHIFTENGINE_H_INCLUDED

template <int I>
class Xorshift32EngineT : public RandomEngineT<uint32_t>
{
public:
    typedef uint32_t GeneratedValuesType;

    Xorshift32EngineT(uint32_t seed = 0xb5f3c6a7);

    Xorshift32EngineT(const Xorshift32EngineT<I>&) = default;

    ~Xorshift32EngineT();

    virtual uint32_t nextRaw(); /* generates 32 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;

protected:
    uint32_t x, y, z, w;
};
typedef Xorshift32EngineT<> Xorshift32Engine;

#include "../src/Xorshift32Engine.cpp"

#endif // XORSHIFTENGINE_H_INCLUDED
