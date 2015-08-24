#ifndef RANDOMENGINE_H
#define RANDOMENGINE_H

class RandomEngineBase
{
public:
    virtual int32_t nextInt32() = 0;
    virtual uint32_t nextUint32() = 0;

    virtual bool nextBool() = 0;
    virtual int8_t nextInt8() = 0;
    virtual uint8_t nextUint8() = 0;
    virtual int16_t nextInt16() = 0;
    virtual uint16_t nextUint16() = 0;
    virtual int64_t nextInt64() = 0;
    virtual uint64_t nextUint64() = 0;

    virtual float nextFloat() = 0;
    virtual double nextDouble() = 0;
    virtual long double nextLongDouble() = 0;


    virtual int32_t nextInt32(int32_t min, int32_t max) = 0;
    virtual uint32_t nextUint32(uint32_t min, uint32_t max) = 0;

    virtual int8_t nextInt8(int8_t min, int8_t max) = 0;
    virtual uint8_t nextUint8(uint8_t min, uint8_t max) = 0;
    virtual int16_t nextInt16(int16_t min, int16_t max) = 0;
    virtual uint16_t nextUint16(uint16_t min, uint16_t max) = 0;
    virtual int64_t nextInt64(int64_t min, int64_t max) = 0;
    virtual uint64_t nextUint64(uint64_t min, uint64_t max) = 0;

    virtual float nextFloat(float left, float right) = 0; //[left, right)
    virtual double nextDouble(double left, double right) = 0; //[left, right)
    virtual long double nextLongDouble(long double left, long double right) = 0; //[left, right]

    template <class T>
    inline T next();
    template <class T>
    inline T next(T left, T right);
};

template <int I>
class RandomEngineT : public RandomEngineBase
{
public:
    RandomEngineT();
    virtual ~RandomEngineT();

    virtual int32_t nextInt32() = 0;
    virtual uint32_t nextUint32() = 0;

    virtual bool nextBool();
    virtual int8_t nextInt8();
    virtual uint8_t nextUint8();
    virtual int16_t nextInt16();
    virtual uint16_t nextUint16();
    virtual int64_t nextInt64();
    virtual uint64_t nextUint64();

    virtual float nextFloat();
    virtual double nextDouble();
    virtual long double nextLongDouble();


    virtual int32_t nextInt32(int32_t min, int32_t max);
    virtual uint32_t nextUint32(uint32_t min, uint32_t max);

    virtual int8_t nextInt8(int8_t min, int8_t max);
    virtual uint8_t nextUint8(uint8_t min, uint8_t max);
    virtual int16_t nextInt16(int16_t min, int16_t max);
    virtual uint16_t nextUint16(uint16_t min, uint16_t max);
    virtual int64_t nextInt64(int64_t min, int64_t max);
    virtual uint64_t nextUint64(uint64_t min, uint64_t max);

    virtual float nextFloat(float left, float right); //[left, right)
    virtual double nextDouble(double left, double right); //[left, right)
    virtual long double nextLongDouble(long double left, long double right); //[left, right]

};

typedef RandomEngineT<0> RandomEngine;

#include "../src/RandomEngine.cpp"

#endif // RANDOMENGINE_H
