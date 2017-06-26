#pragma once

template <class T>
class Rng
{
public:
    using GeneratorType = T;

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

    int sample(int min, int max, double exponent, double probability)
    {
        static std::uniform_real_distribution<double> distr(0.0, 1.0);

        const double r = distr(m_rng);
        if (r > probability) return 0;

        if (min == max) return min;

        const double minSample = min - 0.5; //because we round them to the nearest integer later
        const double maxSample = max + 0.5;

        const double t = std::pow(distr(m_rng), exponent);
        const double q = minSample + (maxSample - minSample)*t;

        int quantity = static_cast<int>(std::round(q));
        if (quantity < min) quantity = min; //may happen due to floating point rounding errors
        if (quantity > max) quantity = max;

        return quantity;
    }

private:
    Rng()
    {

    }
};