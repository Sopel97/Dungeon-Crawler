#include "..\include\CellNoise.h"

#include "..\Util.h"

namespace ls
{
    template <class T>
    CellNoise<T>::CellNoise()
    {
        seed = 12312;
        setDistanceFunction(DistanceFunction::Default);
        setResultComputingFunction(ResultComputingFunction::Default);
    }

    template <class T>
    CellNoise<T>::~CellNoise()
    {

    }
    template <class T>
    T CellNoise<T>::rawNoise1(const T x)
    {
        int segmentX = Util::fastFloor(x);

        std::vector<T> distances; //contains at least 3 distances

        for(int xx = -1; xx <= 1; ++xx)
        {
            int currentSegmentX = segmentX + xx;

            unsigned int lastRandom = lcgRandom(hash((unsigned int)(currentSegmentX), seed, seed * seed));

            unsigned int numberFeaturePoints = probLookup(lastRandom);
            for(unsigned int i = 0; i < numberFeaturePoints; ++i)
            {
                lastRandom = lcgRandom(lastRandom);
                T xxx = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentSegmentX;

                T distance = std::abs(x - xxx); //since distance in 1 dimension is always this
                distances.push_back(distance);
            }
        }
        std::sort(distances.begin(), distances.end());
        T result = computeResult(distances);
        if(result > T(1)) result = T(1);
        else if(result < T(0)) result = T(0);
        return result;
    }
    template <class T>
    T CellNoise<T>::rawNoise2(const T x, const T y)
    {
        int rectX = Util::fastFloor(x);
        int rectY = Util::fastFloor(y);

        std::vector<T> distances; //contains at least 9 distances

        for(int xx = -1; xx <= 1; ++xx)
        {
            for(int yy = -1; yy <= 1; ++yy)
            {
                int currentRectX = rectX + xx;
                int currentRectY = rectY + yy;

                unsigned int lastRandom = lcgRandom(hash((unsigned int)(currentRectX), (unsigned int)(currentRectY), seed));

                unsigned int numberFeaturePoints = probLookup(lastRandom);
                for(unsigned int i = 0; i < numberFeaturePoints; ++i)
                {
                    lastRandom = lcgRandom(lastRandom);
                    T xxx = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentRectX;

                    lastRandom = lcgRandom(lastRandom);
                    T yyy = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentRectY;

                    T distance = distance2(x, y, xxx, yyy);
                    distances.push_back(distance);
                }
            }
        }
        std::sort(distances.begin(), distances.end());
        T result = computeResult(distances);
        if(result > T(1)) result = T(1);
        else if(result < T(0)) result = T(0);
        return result;

    }

    template <class T>
    T CellNoise<T>::rawNoise3(const T x, const T y, const T z)
    {
        int cubeX = Util::fastFloor(x);
        int cubeY = Util::fastFloor(y);
        int cubeZ = Util::fastFloor(z);

        std::vector<T> distances; //contains at least 27 distances

        for(int xx = -1; xx <= 1; ++xx)
        {
            for(int yy = -1; yy <= 1; ++yy)
            {
                for(int zz = -1; zz <= 1; ++zz)
                {
                    int currentCubeX = cubeX + xx;
                    int currentCubeY = cubeY + yy;
                    int currentCubeZ = cubeZ + zz;

                    unsigned int lastRandom = lcgRandom(hash((unsigned int)(currentCubeX), (unsigned int)(currentCubeY), (unsigned int)(currentCubeZ + seed)));

                    unsigned int numberFeaturePoints = probLookup(lastRandom);
                    for(unsigned int i = 0; i < numberFeaturePoints; ++i)
                    {
                        lastRandom = lcgRandom(lastRandom);
                        T xxx = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentCubeX;

                        lastRandom = lcgRandom(lastRandom);
                        T yyy = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentCubeY;

                        lastRandom = lcgRandom(lastRandom);
                        T zzz = (T)lastRandom / (T)0xFFFFFFFFu + (T)currentCubeZ;

                        T distance = distance3(x, y, z, xxx, yyy, zzz);
                        distances.push_back(distance);
                    }
                }
            }
        }
        std::sort(distances.begin(), distances.end());
        T result = computeResult(distances);
        if(result > T(1)) result = T(1);
        else if(result < T(0)) result = T(0);
        return result;
    }

    template <class T>
    unsigned int CellNoise<T>::lcgRandom(unsigned int lastValue)
    {
        return (unsigned int)((1103515245u * lastValue + 12345u) % 0x100000000u);
    }

    template <class T>
    unsigned int CellNoise<T>::hash(unsigned int i, unsigned int j, unsigned int k)
    {
        return (unsigned int)((((((OFFSET_BASIS ^ (unsigned int)i) * FNV_PRIME) ^ (unsigned int)j) * FNV_PRIME) ^ (unsigned int)k) * FNV_PRIME);
    }

    template <class T>
    unsigned int CellNoise<T>::probLookup(unsigned int value)
    {
        if(value < 393325350) return 1;
        if(value < 1022645910) return 2;
        if(value < 1861739990) return 3;
        if(value < 2700834071) return 4;
        if(value < 3372109335) return 5;
        if(value < 3819626178) return 6;
        if(value < 4075350088) return 7;
        if(value < 4203212043) return 8;
        return 9;
    }

    template <class T>
    void CellNoise<T>::setDistanceFunction(DistanceFunction distanceFunction)
    {
        switch(distanceFunction)
        {
        default:
        case DistanceFunction::Default:
        case DistanceFunction::Euclidean:
            distance2 = [] (T x1, T y1, T x2, T y2) -> T {
                T dx = x2 - x1;
                T dy = y2 - y1;
                return static_cast<T>(std::sqrt(dx * dx + dy * dy));
            };
            distance3 = [] (T x1, T y1, T z1, T x2, T y2, T z2) -> T {
                T dx = x2 - x1;
                T dy = y2 - y1;
                T dz = z2 - z1;
                return static_cast<T>(sqrt(dx * dx + dy * dy + dz * dz));
            };
            break;
        case DistanceFunction::Manhattan:
            distance2 = [] (T x1, T y1, T x2, T y2) -> T {
                return std::abs(x2 - x1) + std::abs(y2 - y1);
            };
            distance3 = [] (T x1, T y1, T z1, T x2, T y2, T z2) -> T {
                return std::abs(x2 - x1) + std::abs(y2 - y1) + std::abs(z2 - z1);
            };
            break;
        case DistanceFunction::Chebyshev:
            distance2 = [] (T x1, T y1, T x2, T y2) -> T {
                return std::max(std::abs(x2 - x1), std::abs(y2 - y1));
            };
            distance3 = [] (T x1, T y1, T z1, T x2, T y2, T z2) -> T {
                return std::max(std::max(std::abs(x2 - x1), std::abs(y2 - y1)), std::abs(z2 - z1));
            };
            break;
        }
    }

    template <class T>
    void CellNoise<T>::setResultComputingFunction(ResultComputingFunction resultComputingFunction)
    {
        switch(resultComputingFunction)
        {
        default:
        case ResultComputingFunction::Default:
            computeResult = [] (const std::vector<T>& distances) -> T {
                return distances[0];
            };
            break;
        }
    }
    template <class T>
    template <class CustomResultComputingFunction>
    void CellNoise<T>::setResultComputingFunction(CustomResultComputingFunction resultComputingFunction)
    {
        computeResult = resultComputingFunction;
    }
}