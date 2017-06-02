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

    
private:
    Rng()
    {

    }
};