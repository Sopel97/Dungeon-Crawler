#ifndef CELLNOISE_H_INCLUDED
#define CELLNOISE_H_INCLUDED

template <class T>
class CellNoise
{
public:
    enum class DistanceFunction
    {
        Default,
        Euclidean,
        Manhattan,
        Chebyshev
    };
    enum class ResultComputingFunction
    {
        Default
    };

    int seed;

    CellNoise();
    ~CellNoise();

    T rawNoise1(const T x);
    T rawNoise2(const T x, const T y);
    T rawNoise3(const T x, const T y, const T z);

    void setDistanceFunction(DistanceFunction distanceFunction);
    void setResultComputingFunction(ResultComputingFunction resultComputingFunction);
    template <class CustomResultComputingFunction>
    void setResultComputingFunction(CustomResultComputingFunction resultComputingFunction);

private:
    const unsigned int FNV_PRIME = 16777619u;
    const unsigned int OFFSET_BASIS = 2166136261u;

    std::function<T(T, T, T, T)> distance2;
    std::function<T(T, T, T, T, T, T)> distance3;
    std::function<T(const std::vector<T>&)> computeResult;

    unsigned int lcgRandom(unsigned int lastValue);
    unsigned int hash(unsigned int i, unsigned int j, unsigned int k);
    unsigned int probLookup(unsigned int value);
};

typedef CellNoise<double> CellNoiseD;
typedef CellNoise<float> CellNoiseF;
typedef CellNoise<int> CellNoiseI;

#include "../src/CellNoise.cpp"

#endif // CELLNOISE_H_INCLUDED
