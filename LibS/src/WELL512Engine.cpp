template <int I>
WELL512EngineT<I>::WELL512EngineT(uint64_t seed) :
    RandomEngineT<uint32_t>(seed, 0xffffffffu)
{
    stateIndex = 0;
    for(int i = 0; i < 16; ++i) stateArray[i] = seed;  //there may be neede better initialization
}
template <int I>
WELL512EngineT<I>::~WELL512EngineT()
{

}
template <int I>
uint32_t WELL512EngineT<I>::nextRaw()
{
    z0 = VRm1();
    z1 = MAT0NEG(-16, V0()) ^ MAT0NEG(-15, VM1());
    z2 = MAT0POS(11, VM2());
    newV1() = z1 ^ z2;
    newV0() = MAT0NEG(-2, z0) ^ MAT0NEG(-18, z1) ^ MAT3NEG(-28, z2) ^ MAT4NEG(-5, 0xda442d24U, newV1());
    stateIndex = (stateIndex + 15) & 0x0000000fU;
    return stateArray[stateIndex];
}


template <int I>
std::unique_ptr<RandomEngineBase> WELL512EngineT<I>::clone() const
{
    return std::make_unique<WELL512EngineT<I>>(*this);
}
