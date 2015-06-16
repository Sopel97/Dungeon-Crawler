
template <>
inline int32_t RandomEngineBase::next()
{
    return this->nextInt32();
}
template <>
inline int32_t RandomEngineBase::next(int32_t rangeMin, int32_t rangeMax)
{
    return this->nextInt32(rangeMin, rangeMax);
}

template <>
inline uint32_t RandomEngineBase::next()
{
    return this->nextUint32();
}
template <>
inline uint32_t RandomEngineBase::next(uint32_t rangeMin, uint32_t rangeMax)
{
    return this->nextUint32(rangeMin, rangeMax);
}


template <>
inline int64_t RandomEngineBase::next()
{
    return this->nextInt64();
}
template <>
inline int64_t RandomEngineBase::next(int64_t rangeMin, int64_t rangeMax)
{
    return this->nextInt64(rangeMin, rangeMax);
}


template <>
inline uint64_t RandomEngineBase::next()
{
    return this->nextUint64();
}
template <>
inline uint64_t RandomEngineBase::next(uint64_t rangeMin, uint64_t rangeMax)
{
    return this->nextUint64(rangeMin, rangeMax);
}


template <>
inline float RandomEngineBase::next()
{
    return this->nextFloat();
}
template <>
inline float RandomEngineBase::next(float rangeMin, float rangeMax)
{
    return this->nextFloat(rangeMin, rangeMax);
}


template <>
inline double RandomEngineBase::next()
{
    return this->nextDouble();
}
template <>
inline double RandomEngineBase::next(double rangeMin, double rangeMax)
{
    return this->nextDouble(rangeMin, rangeMax);
}


template <>
inline long double RandomEngineBase::next()
{
    return this->nextLongDouble();
}
template <>
inline long double RandomEngineBase::next(long double rangeMin, long double rangeMax)
{
    return this->nextLongDouble(rangeMin, rangeMax);
}

template <>
inline bool RandomEngineBase::next()
{
    return this->nextBool();
}

//general case
template <class T>
inline T RandomEngineBase::next()
{
    return this->nextLongDouble();
}
template <class T>
inline T RandomEngineBase::next(T rangeMin, T rangeMax)
{
    return this->nextLongDouble(rangeMin, rangeMax); //most general one
}
