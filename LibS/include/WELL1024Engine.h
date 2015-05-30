#ifndef WELL1024ENGINE_H
#define WELL1024ENGINE_H
template <int I>
class WELL1024EngineT : public RandomEngineT<uint32_t>
{
public:
    typedef uint32_t GeneratedValuesType;

    WELL1024EngineT(uint64_t seed = 0xb5f3c6a7);

    WELL1024EngineT(const WELL1024EngineT<I>&) = default;

    ~WELL1024EngineT();

    virtual uint32_t nextRaw(); /* generates 32 bit number */

    virtual std::unique_ptr<RandomEngineBase> clone() const;

protected:
    uint32_t stateArray[32];
    uint32_t stateIndex;
    uint32_t z0, z1, z2;
private: //many functions made from defines, this is ugly and could possibly be done better.
    static const int32_t W = 32;
    static const int32_t M1 = 3;
    static const int32_t M2 = 24;
    static const int32_t M3 = 10;

    static inline int32_t MAT0POS(int32_t t, int32_t v) {return (v ^ (v >> t));}
    static inline int32_t MAT0NEG(int32_t t, int32_t v) {return (v ^ (v << (-(t))));}

    inline uint32_t& V0()       {return stateArray[stateIndex                   ];}
    inline uint32_t& VM1()      {return stateArray[(stateIndex + M1) & 0x0000001fU];}
    inline uint32_t& VM2()      {return stateArray[(stateIndex + M2) & 0x0000001fU];}
    inline uint32_t& VM3()      {return stateArray[(stateIndex + M3) & 0x0000001fU];}
    inline uint32_t& VRm1()     {return stateArray[(stateIndex + 31) & 0x0000001fU];}
    inline uint32_t& newV0()    {return stateArray[(stateIndex + 31) & 0x0000001fU];}
    inline uint32_t& newV1()    {return stateArray[stateIndex                   ];}
};
typedef WELL1024EngineT<> WELL1024Engine;

#include "../src/WELL1024Engine.cpp"

#endif // WELL1024ENGINE_H
