#include "..\include\ElementaryCellularAutomaton.h"

namespace ls
{

    template <class T>
    ElementaryCellularAutomatonTemplate<T>::ElementaryCellularAutomatonTemplate(uint8_t rule, int width, int origin, TapeTopology topology) :
        m_topology(topology),
        m_tape(width, 0)
    {
        m_tape[origin] = 1u;
        setRule(rule);
    }
    template <class T>
    ElementaryCellularAutomatonTemplate<T>::ElementaryCellularAutomatonTemplate(uint8_t rule, const std::vector<uint8_t>& initialState, TapeTopology topology) :
        m_topology(topology),
        m_tape(initialState)
    {
        setRule(rule);
    }
    template <class T>
    ElementaryCellularAutomatonTemplate<T>::ElementaryCellularAutomatonTemplate(uint8_t rule, std::vector<uint8_t>&& initialState, TapeTopology topology) :
        m_topology(topology),
        m_tape(std::move(initialState))
    {
        setRule(rule);
    }
    template <class T>
    void ElementaryCellularAutomatonTemplate<T>::setRule(uint8_t rule) &
    {
        for(int i = 0; i < 8; ++i)
        {
            m_outputs[i] = rule & 1u;
            rule >>= 1u;
        }
    }

    template <class T>
    void ElementaryCellularAutomatonTemplate<T>::iterate(int iterations) &
    {
        const int size = m_tape.size();

        std::vector<uint8_t> newTape;
        for(int i = 0; i < iterations; ++i)
        {
            newTape.reserve(size);

            for(int j = 1; j < size - 1; ++j)
            {
                uint8_t outputIndex = 0u;
                outputIndex += m_tape[j - 1] << 2u;
                outputIndex += m_tape[j] << 1u;
                outputIndex += m_tape[j + 1];

                newTape[j] = m_outputs[outputIndex];
            }
            if(m_topology == TapeTopology::Finite)
            {
                newTape[0] = m_outputs[(m_tape[0] << 1u) + m_tape[1]];
                newTape[size - 1] = m_outputs[(m_tape[size - 2] << 1u) + m_tape[size - 1]];
            }
            else
            {
                newTape[0] = m_outputs[(m_tape.back() << 2u) + (m_tape[0] << 1u) + m_tape[1]];
                newTape[size - 1] = m_outputs[(m_tape[size - 2] << 2u) + (m_tape[size - 1] << 1u) + m_tape.front()];
            }

            m_tape.swap(newTape);
        }
    }

    template <class T>
    const std::vector<uint8_t>& ElementaryCellularAutomatonTemplate<T>::tape() const &
    {
        return m_tape;
    }
    template <class T>
    std::vector<uint8_t> ElementaryCellularAutomatonTemplate<T>::tape() &&
    {
        return std::move(m_tape);
    }

}