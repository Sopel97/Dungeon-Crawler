template <int I>
Xorshift32EngineT<I>::Xorshift32EngineT(uint32_t seed) :
    RandomEngineT<uint32_t>(seed, 0xffffffffu)
{
    x = seed;
    y = x + 0xb5f3c6a7; //this values are temporary, i have no idea what they should be
    z = y * 0xb5f3c6a7;
    w = z ^ 0xb5f3c6a7;
}
template <int I>
Xorshift32EngineT<I>::~Xorshift32EngineT()
{

}
template <int I>
uint32_t Xorshift32EngineT<I>::nextRaw()
{
    uint32_t t = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

template <int I>
std::unique_ptr<RandomEngineBase> Xorshift32EngineT<I>::clone() const
{
    return std::make_unique<Xorshift32EngineT<I>>(*this);
}
