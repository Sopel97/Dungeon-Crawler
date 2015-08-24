/* for static dispatch with templates */

template <>
inline bool RandomEngineBase::next()
{
    return this->nextBool();
}

template <>
inline int8_t RandomEngineBase::next()
{
    return this->nextInt8();
}
template <>
inline int8_t RandomEngineBase::next(int8_t left, int8_t right)
{
    return this->nextInt8(left, right);
}

template <>
inline uint8_t RandomEngineBase::next()
{
    return this->nextUint8();
}
template <>
inline uint8_t RandomEngineBase::next(uint8_t left, uint8_t right)
{
    return this->nextUint8(left, right);
}

template <>
inline int16_t RandomEngineBase::next()
{
    return this->nextInt16();
}
template <>
inline int16_t RandomEngineBase::next(int16_t left, int16_t right)
{
    return this->nextInt16(left, right);
}

template <>
inline uint16_t RandomEngineBase::next()
{
    return this->nextUint16();
}
template <>
inline uint16_t RandomEngineBase::next(uint16_t left, uint16_t right)
{
    return this->nextUint16(left, right);
}

template <>
inline int32_t RandomEngineBase::next()
{
    return this->nextInt32();
}
template <>
inline int32_t RandomEngineBase::next(int32_t left, int32_t right)
{
    return this->nextInt32(left, right);
}

template <>
inline uint32_t RandomEngineBase::next()
{
    return this->nextUint32();
}
template <>
inline uint32_t RandomEngineBase::next(uint32_t left, uint32_t right)
{
    return this->nextUint32(left, right);
}

template <>
inline int64_t RandomEngineBase::next()
{
    return this->nextInt64();
}
template <>
inline int64_t RandomEngineBase::next(int64_t left, int64_t right)
{
    return this->nextInt64(left, right);
}

template <>
inline uint64_t RandomEngineBase::next()
{
    return this->nextUint64();
}
template <>
inline uint64_t RandomEngineBase::next(uint64_t left, uint64_t right)
{
    return this->nextUint64(left, right);
}

template <>
inline float RandomEngineBase::next()
{
    return this->nextFloat();
}
template <>
inline float RandomEngineBase::next(float left, float right)
{
    return this->nextFloat(left, right);
}

template <>
inline double RandomEngineBase::next()
{
    return this->nextDouble();
}
template <>
inline double RandomEngineBase::next(double left, double right)
{
    return this->nextDouble(left, right);
}

template <>
inline long double RandomEngineBase::next()
{
    return this->nextLongDouble();
}
template <>
inline long double RandomEngineBase::next(long double left, long double right)
{
    return this->nextLongDouble(left, right);
}

//general case
template <class T>
inline T RandomEngineBase::next()
{
    return this->nextLongDouble();
}
template <class T>
inline T RandomEngineBase::next(T left, T right)
{
    return this->nextLongDouble(left, right); //most general one
}

/* end of templated methods */

template <int I>
RandomEngineT<I>::RandomEngineT()
{

}
template <int I>
RandomEngineT<I>::~RandomEngineT()
{

}

template <int I>
bool RandomEngineT<I>::nextBool()
{
    return this->nextUint32() > 0x7FFFFFFFu;
}
template <int I>
int8_t RandomEngineT<I>::nextInt8()
{
    return this->nextInt32() & 0xFF;
}
template <int I>
uint8_t RandomEngineT<I>::nextUint8()
{
    return this->nextUint32() & 0xFFu;
}
template <int I>
int16_t RandomEngineT<I>::nextInt16()
{
    return this->nextInt32() & 0xFFFF;
}
template <int I>
uint16_t RandomEngineT<I>::nextUint16()
{
    return this->nextUint32() & 0xFFFFu;
}
template <int I>
int64_t RandomEngineT<I>::nextInt64()
{
    return (static_cast<int64_t>(this->nextInt32()) << 32) + static_cast<int64_t>(this->nextInt32());
}
template <int I>
uint64_t RandomEngineT<I>::nextUint64()
{
    return (static_cast<uint64_t>(this->nextUint32()) << 32) + static_cast<uint64_t>(this->nextUint32());
}

template <int I>
float RandomEngineT<I>::nextFloat()
{
    return static_cast<float>(this->nextUint32() & 0xFFFFFFu) / static_cast<float>(0xFFFFFFu);
}
template <int I>
double RandomEngineT<I>::nextDouble()
{
    return static_cast<double>(this->nextUint64() & 0xFFFFFFFFFFFFFull) / static_cast<double>(0xFFFFFFFFFFFFFull);
}
template <int I>
long double RandomEngineT<I>::nextLongDouble()
{
    return static_cast<long double>(this->nextUint64()) / static_cast<long double>(0xFFFFFFFFFFFFFFFFull);
}



template <int I>
int8_t RandomEngineT<I>::nextInt8(int8_t min, int8_t max)
{
    return nextInt8() % (max-min+1) + min;
}
template <int I>
uint8_t RandomEngineT<I>::nextUint8(uint8_t min, uint8_t max)
{
    return nextUint8() % (max-min+1u) + min;
}
template <int I>
int16_t RandomEngineT<I>::nextInt16(int16_t min, int16_t max)
{
    return nextInt16() % (max-min+1) + min;
}
template <int I>
uint16_t RandomEngineT<I>::nextUint16(uint16_t min, uint16_t max)
{
    return nextUint16() % (max-min+1u) + min;
}
template <int I>
int32_t RandomEngineT<I>::nextInt32(int32_t min, int32_t max)
{
    return nextInt32() % (max-min+1) + min;
}
template <int I>
uint32_t RandomEngineT<I>::nextUint32(uint32_t min, uint32_t max)
{
    return nextUint32() % (max-min+1u) + min;
}
template <int I>
int64_t RandomEngineT<I>::nextInt64(int64_t min, int64_t max)
{
    return nextInt64() % (max-min+1) + min;
}
template <int I>
uint64_t RandomEngineT<I>::nextUint64(uint64_t min, uint64_t max)
{
    return nextUint64() % (max-min+1ull) + min;
}

template <int I>
float RandomEngineT<I>::nextFloat(float left, float right)
{
    return nextFloat() * (right-left) + left;
}
template <int I>
double RandomEngineT<I>::nextDouble(double left, double right)
{
    return nextDouble() * (right-left) + left;
}
template <int I>
long double RandomEngineT<I>::nextLongDouble(long double left, long double right)
{
    return nextLongDouble() * (right-left) + left;
}
