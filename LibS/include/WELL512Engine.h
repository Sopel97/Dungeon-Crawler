#ifndef WELL512ENGINE_H
#define WELL512ENGINE_H

template <int I>
class WELL512EngineT : public RandomEngineT<uint32_t>
{
public:
    typedef uint32_t GeneratedValuesType;

    WELL512EngineT(uint64_t seed = 0xb5f3c6a7);

    WELL512EngineT(const WELL512EngineT<I>&) = default;

    ~WELL512EngineT();

    virtual uint32_t nextRaw(); /* generates 32 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;

protected:
    uint32_t stateArray[16];
    uint32_t stateIndex;
    uint32_t z0, z1, z2;
private: //many functions made from defines, this is ugly and could possibly be done better.
    static const int32_t W = 32;
    static const int32_t M1 = 13;
    static const int32_t M2 = 9;
    static const int32_t M3 = 5;

    static inline int32_t MAT0POS(int32_t t, int32_t v) {return (v ^ (v >> t));}
    static inline int32_t MAT0NEG(int32_t t, int32_t v) {return (v ^ (v << (-(t))));}
    static inline int32_t MAT3NEG(int32_t t, int32_t v) {return (v << (-(t)));}
    static inline int32_t MAT4NEG(int32_t t, int32_t b, int32_t v) {return (v ^ ((v << (-(t))) & b));}

    inline uint32_t& V0()       {return stateArray[stateIndex                   ];}
    inline uint32_t& VM1()      {return stateArray[(stateIndex + M1) & 0x0000000fU];}
    inline uint32_t& VM2()      {return stateArray[(stateIndex + M2) & 0x0000000fU];}
    inline uint32_t& VM3()      {return stateArray[(stateIndex + M3) & 0x0000000fU];}
    inline uint32_t& VRm1()     {return stateArray[(stateIndex + 15) & 0x0000000fU];}
    inline uint32_t& VRm2()     {return stateArray[(stateIndex + 14) & 0x0000000fU];}
    inline uint32_t& newV0()    {return stateArray[(stateIndex + 15) & 0x0000000fU];}
    inline uint32_t& newV1()    {return stateArray[stateIndex                   ];}
    inline uint32_t& newVRm1()  {return stateArray[(stateIndex + 14) & 0x0000000fU];}
};
typedef WELL512EngineT<> WELL512Engine;

#include "../src/WELL512Engine.cpp"

#endif // WELL512ENGINE_H
