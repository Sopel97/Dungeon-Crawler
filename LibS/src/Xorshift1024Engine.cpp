template <int I>
Xorshift1024EngineT<I>::Xorshift1024EngineT(uint64_t seed) :
    RandomEngineT<uint64_t>(seed, 0xffffffffffffffffULL)
{
    Xorshift64Engine engine(seed);
    for(int i = 0; i < 16; ++i) s[i] = engine.nextRaw();
    p = 0;
}
template <int I>
Xorshift1024EngineT<I>::~Xorshift1024EngineT()
{

}
template <int I>
uint64_t Xorshift1024EngineT<I>::nextRaw()
{
    uint64_t s0 = s[ p ];
    uint64_t s1 = s[ p = (p + 1) & 15 ];
    s1 ^= s1 << 31; // a
    s1 ^= s1 >> 11; // b
    s0 ^= s0 >> 30; // c
    return (s[ p ] = s0 ^ s1) * 1181783497276652981ULL;
}

template <int I>
std::unique_ptr<RandomEngineBase> Xorshift1024EngineT<I>::clone() const
{
    return std::make_unique<Xorshift1024EngineT<I>>(*this);
}
