template <int I>
Xorshift128EngineT<I>::Xorshift128EngineT(uint64_t seed) :
    RandomEngineT<uint64_t>(seed, 0xffffffffffffffffULL)
{
    s[0] = seed;
    s[1] = seed;
}
template <int I>
Xorshift128EngineT<I>::~Xorshift128EngineT()
{

}
template <int I>
uint64_t Xorshift128EngineT<I>::nextRaw()
{
    uint64_t s1 = s[ 0 ];
    const uint64_t s0 = s[ 1 ];
    s[ 0 ] = s0;
    s1 ^= s1 << 23;
    return (s[ 1 ] = (s1 ^ s0 ^ (s1 >> 17) ^ (s0 >> 26))) + s0;
}

template <int I>
std::unique_ptr<RandomEngineBase> Xorshift128EngineT<I>::clone() const
{
    return std::make_unique<Xorshift128EngineT<I>>(*this);
}
