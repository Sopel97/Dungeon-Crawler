template <typename IntType>
RandomEngineT<IntType>::RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::~RandomEngineT()
{

}
template <typename IntType>
RandomEngineT<IntType>::RandomEngineT(IntType seed, IntType max) :
    m_seed(seed),
    m_max(max)
{

}
template <typename IntType>
IntType RandomEngineT<IntType>::next(IntType rangeFirst, IntType rangeLast) // <rangeFirst, rangeLast>
{
    return nextRaw() % (rangeLast - rangeFirst + IntType(1)) + rangeFirst;
}
template <typename IntType>
int32_t RandomEngineT<IntType>::nextInt32(int32_t rangeFirst, int32_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <typename IntType>
uint32_t RandomEngineT<IntType>::nextUint32(uint32_t rangeFirst, uint32_t rangeLast)
{
    return next(rangeFirst, rangeLast);
}
template <typename IntType>
uint64_t RandomEngineT<IntType>::nextUint64(uint64_t rangeFirst, uint64_t rangeLast)
{
    return nextUint64() % (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
int64_t RandomEngineT<IntType>::nextInt64(int64_t rangeFirst, int64_t rangeLast)
{
    return nextInt64() % (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat(float rangeFirst, float rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / static_cast<float>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble(double rangeFirst, double rangeLast) // <rangeFirst, rangeLast) (if m_max can't fit in int type used for nextRaw() then it's <rangeFirst, rangeLast>
{
    return nextRaw() / static_cast<double>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}
template <typename IntType>
long double RandomEngineT<IntType>::nextLongDouble(double rangeFirst, double rangeLast)
{
    return nextRaw() / static_cast<long double>(m_max) * (rangeLast - rangeFirst) + rangeFirst;
}

template <typename IntType>
int32_t RandomEngineT<IntType>::nextInt32()
{
    return nextRaw();
}
template <typename IntType>
uint32_t RandomEngineT<IntType>::nextUint32()
{
    return nextRaw();
}
template <typename IntType>
int64_t RandomEngineT<IntType>::nextInt64()
{
    if(sizeof(IntType) < sizeof(int64_t))
    {
        int64_t result = 0;
        size_t n = (sizeof(uint64_t) / sizeof(IntType)) + (sizeof(uint64_t) % sizeof(IntType) != 0);
        for(size_t i = 0; i < n; ++i)
        {
            result <<= sizeof(IntType) * 8ull; //compiler warns about this, but this case is handled by a condition on top of the function
            result |= next();
        }
        return result;
    }
    else return next();
}
template <typename IntType>
uint64_t RandomEngineT<IntType>::nextUint64()
{
    if(sizeof(IntType) < sizeof(uint64_t))
    {
        uint64_t result = 0;
        size_t n = (sizeof(uint64_t) / sizeof(IntType)) + (sizeof(uint64_t) % sizeof(IntType) != 0);
        for(size_t i = 0; i < n; ++i)
        {
            result <<= sizeof(IntType) * 8ull; //compiler warns about this, but this case is handled by a condition on top of the function
            result |= next();
        }
        return result;
    }
    else return next();
}
template <typename IntType>
IntType RandomEngineT<IntType>::next() //range defined by IntType size and generator
{
    return nextRaw();
}
template <typename IntType>
float RandomEngineT<IntType>::nextFloat() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<float>(m_max);
}
template <typename IntType>
double RandomEngineT<IntType>::nextDouble() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<double>(m_max);
}
template <typename IntType>
long double RandomEngineT<IntType>::nextLongDouble() // <0, 1) (if m_max can't fit in int type used for nextRaw() then it's <0, 1>
{
    return nextRaw() / static_cast<long double>(m_max);
}
template <typename IntType>
bool RandomEngineT<IntType>::nextBool() //0 or 1
{
    return nextRaw() > (m_max >> 1);
}

template <typename IntType>
IntType RandomEngineT<IntType>::seed() const
{
    return m_seed;
}
template <typename IntType>
IntType RandomEngineT<IntType>::max() const //max+1 or max if max+1 dont fir in IntType
{
    return m_max;
}
