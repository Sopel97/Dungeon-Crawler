#pragma once

#include <vector>
#include <numeric>

template <class GenType>
class Rng
{
public:
    using GeneratorType = GenType;

private:
    GeneratorType m_rng;

public:
    static Rng& instance()
    {
        static Rng rng;
        return rng;
    }

    GeneratorType& rng()
    {
        return m_rng;
    }

    template <class T, class U>
    T sample(T min, T max, U exponent, U probability)
    {
        static std::uniform_real_distribution<U> distr(0.0, 1.0);

        const U r = distr(m_rng);
        if (r > probability) return 0;

        if (min == max) return min;

        const U minSample = min - U(0.5); //because we round them to the nearest integer later
        const U maxSample = max + U(0.5);

        const U t = std::pow(distr(m_rng), exponent);
        const U q = minSample + (maxSample - minSample)*t;

        int quantity = static_cast<T>(std::round(q));
        if (quantity < min) quantity = min; //may happen due to floating point rounding errors
        if (quantity > max) quantity = max;

        return quantity;
    }

    template <class T>
    std::vector<int> weightedShuffleIndices(const std::vector<T>& weights)
    {
        static std::uniform_real_distribution<double> distr(0.0, 1.0);

        const int numWeights = weights.size();

        std::vector<int> indices(numWeights);
        std::iota(indices.begin(), indices.end(), 0);

        std::vector<double> randomSamples(numWeights);
        for (int i = 0; i < numWeights; ++i)
        {
            randomSamples[i] = std::pow(distr(m_rng), 1.0/weights[i]);
        }

        std::sort(indices.begin(), indices.end(), [&randomSamples](int lhs, int rhs) -> bool {return randomSamples[lhs] > randomSamples[rhs]; });

        return indices;
    }

    template <class T, class U>
    const T& weightedChoose(const std::vector<T>& values, const std::vector<U>& weights)
    {
        const int numValues = values.size();

        std::vector<U> weightSums(numValues);
        std::partial_sum(weights.begin(), weights.end(), weightSums.begin());

        std::uniform_real_distribution<double> distr(0.0, weightSums.back());
        return values[std::distance(weightSums.begin(), std::lower_bound(weightSums.begin(), weightSums.end(), distr(m_rng)))];
    }

    template <class T, class U>
    std::vector<const T*> weightedChoose(const std::vector<T>& values, const std::vector<U>& weights, int n = 1)
    {
        if (n == 1) return { &(weightedChoose(values, weights)) };

        const auto indices = weightedShuffleIndices(weights);
        
        std::vector<T> result;
        result.reserve(n);
        for (int i = 0; i < n; ++i)
        {
            result.emplace_back(&(values[indices[i]]));
        }

        return result;
    }

private:
    Rng()
    {

    }
};