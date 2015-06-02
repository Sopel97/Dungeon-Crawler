#ifndef RANDOMENGINEBASE_H
#define RANDOMENGINEBASE_H

class RandomEngineBase
{
public:
    RandomEngineBase() = default;
    //following ones are base methods. They should stay virtual, they represent random generators for basic c++ types.

    virtual int32_t nextInt32(int32_t rangeFirst, int32_t rangeLast) = 0;
    virtual uint32_t nextUint32(uint32_t rangeFirst, uint32_t rangeLast) = 0;
    virtual int64_t nextInt64(int64_t rangeFirst, int64_t rangeLast) = 0;
    virtual uint64_t nextUint64(uint64_t rangeFirst, uint64_t rangeLast) = 0;
    virtual float nextFloat(float rangeFirst, float rangeLast) = 0;
    virtual double nextDouble(double rangeFirst, double rangeLast) = 0;
    virtual long double nextLongDouble(double rangeFirst, double rangeLast) = 0;

    virtual int32_t nextInt32() = 0;
    virtual uint32_t nextUint32() = 0;
    virtual int64_t nextInt64() = 0;
    virtual uint64_t nextUint64() = 0;
    virtual float nextFloat() = 0;
    virtual double nextDouble() = 0;
    virtual bool nextBool() = 0;
    virtual long double nextLongDouble() = 0;

    //templated versions of the above to allow static dispatch between them without loosing virtuality

    template <class T>
    //this one is not safe since the range of the result highly depends on the type
    inline T next(); //this inline \doesn't change anything drastically (sincle compilers don't rely on this much) but allows it to be header only (i resolves issues with multiple definition error)
    template <class T>
    inline T next(T rangeMin, T rangeMax);


    virtual std::unique_ptr<RandomEngineBase> clone() const = 0;

    virtual ~RandomEngineBase(){};
protected:
};

#include "../src/RandomEngineBase.cpp"

#endif // RANDOMENGINEBASE_H
