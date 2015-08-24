template <class RNG>
StandardRandomNumberGeneratorWrapper<RNG>::StandardRandomNumberGeneratorWrapper(uint32_t seed = 5489u) :
    m_rng(seed)
{

}
template <class RNG>
StandardRandomNumberGeneratorWrapper<RNG>::~StandardRandomNumberGeneratorWrapper()
{

}

template <class RNG>
int32_t StandardRandomNumberGeneratorWrapper<RNG>::nextInt32()
{
    return static_cast<int32_t>(m_rng());
}
template <class RNG>
uint32_t StandardRandomNumberGeneratorWrapper<RNG>::nextUint32()
{
    return static_cast<uint32_t>(m_rng());
}
