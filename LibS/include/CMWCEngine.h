#ifndef CMWCENGINE_H_INCLUDED
#define CMWCENGINE_H_INCLUDED

template <int I>
class CMWCEngineT : public RandomEngineT<uint32_t> //with lag of 4096 //find better version
{
public:
    typedef uint32_t GeneratedValuesType;

    CMWCEngineT(uint32_t seed = 0xb5f3c6a7);
    CMWCEngineT(const CMWCEngineT<I>&) = default;

    ~CMWCEngineT();

    virtual uint32_t nextRaw(); /* generates 16 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;
protected:
    static const int PHI = 0x9e3779b9;
    uint32_t Q[4096];
    uint32_t c;
};
typedef CMWCEngineT<> CMWCEngine;

#include "../src/CMWCEngine.cpp"
#endif // CMWCENGINE_H_INCLUDED
