#ifndef XORSHIFT1024ENGINE_H_INCLUDED
#define XORSHIFT1024ENGINE_H_INCLUDED

template <int I>
class Xorshift1024EngineT : public RandomEngineT<uint64_t>
{
public:
    typedef uint64_t GeneratedValuesType;

    Xorshift1024EngineT(uint64_t seed = 0xb5f3c6a7);

    Xorshift1024EngineT(const Xorshift1024EngineT<I>&) = default;

    ~Xorshift1024EngineT();

    virtual uint64_t nextRaw(); /* generates 32 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;

protected:
    uint64_t s[16];
    uint64_t p;
};
typedef Xorshift1024EngineT<> Xorshift1024Engine;

#include "../src/Xorshift1024Engine.cpp"

#endif // XORSHIFT1024ENGINE_H_INCLUDED
