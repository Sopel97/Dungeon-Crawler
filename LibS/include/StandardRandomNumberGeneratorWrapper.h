#ifndef STANDARDRANDOMNUMBERGENERATORWRAPPER_H
#define STANDARDRANDOMNUMBERGENERATORWRAPPER_H

template <class RNG>
class StandardRandomNumberGeneratorWrapper : public RandomEngine
{
public:
    StandardRandomNumberGeneratorWrapper(uint32_t seed);
    virtual ~StandardRandomNumberGeneratorWrapper();

    virtual int32_t nextInt32();
    virtual uint32_t nextUint32();
protected:
    RNG m_rng;
};

#include "../src/StandardRandomNumberGeneratorWrapper.cpp"

#endif // STANDARDRANDOMNUMBERGENERATORWRAPPER_H
