#ifndef XORSHIFT128ENGINE_H_INCLUDED
#define XORSHIFT128ENGINE_H_INCLUDED

template <int I>
class Xorshift128EngineT : public RandomEngineT<uint64_t>
{
public:
    typedef uint64_t GeneratedValuesType;

    Xorshift128EngineT(uint64_t seed = 0xb5f3c6a7);

    Xorshift128EngineT(const Xorshift128EngineT<I>&) = default;

    ~Xorshift128EngineT();

    virtual uint64_t nextRaw(); /* generates 32 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;

protected:
    uint64_t s[2];
};
typedef Xorshift128EngineT<> Xorshift128Engine;

#include "../src/Xorshift128Engine.cpp"

#endif // XORSHIFT128ENGINE_H_INCLUDED
