template <int I>
WELL1024EngineT<I>::WELL1024EngineT(uint64_t seed) :
    RandomEngineT<uint32_t>(seed, 0xffffffffu)
{
    stateIndex = 0;
    for(int i = 0; i < 32; ++i) stateArray[i] = seed;  //there may be neede better initialization
}
template <int I>
WELL1024EngineT<I>::~WELL1024EngineT()
{

}
template <int I>
uint32_t WELL1024EngineT<I>::nextRaw()
{
    z0 = VRm1();
    z1 = V0() ^ MAT0POS(8, VM1());
    z2 = MAT0NEG(-19, VM2()) ^ MAT0NEG(-14, VM3());
    newV1() = z1 ^ z2;
    newV0() = MAT0NEG(-11, z0) ^ MAT0NEG(-7, z1) ^ MAT0NEG(-13, z2);
    stateIndex = (stateIndex + 31) & 0x0000001fU;
    return stateArray[stateIndex];
}

template <int I>
std::unique_ptr<RandomEngineBase> WELL1024EngineT<I>::clone() const
{
    return std::make_unique<WELL1024EngineT<I>>(*this);
}
