#ifndef STANDARDRANDOMNUMBERGENERATORWRAPPER_H
#define STANDARDRANDOMNUMBERGENERATORWRAPPER_H

template <class StandardEngine>
class StandardRandomNumberGeneratorWrapper : public RandomEngineT<typename StandardEngine::result_type>
{
public:
    typedef typename StandardEngine::result_type GeneratedValuesType;

    StandardRandomNumberGeneratorWrapper(GeneratedValuesType seed = 0xb5f3c6a7);
    StandardRandomNumberGeneratorWrapper(const StandardRandomNumberGeneratorWrapper<StandardEngine>&) = default;

    ~StandardRandomNumberGeneratorWrapper();

    virtual GeneratedValuesType nextRaw();

    virtual std::unique_ptr<RandomEngineBase> clone() const;
protected:
    StandardEngine m_engine;
};

#include "../src/StandardRandomNumberGeneratorWrapper.cpp"

#endif // STANDARDRANDOMNUMBERGENERATORWRAPPER_H
