#ifndef ELEMENTARYCELLULARAUTOMATON_H
#define ELEMENTARYCELLULARAUTOMATON_H

template <int I> //just to make it a template
class ElementaryCellularAutomatonT
{
public:
    ElementaryCellularAutomatonT(size_t rule, size_t width, size_t origin = 0u);
    ElementaryCellularAutomatonT(size_t rule, const std::vector<uint8_t>& initialState);
    ElementaryCellularAutomatonT(size_t rule, std::vector<uint8_t>&& initialState);

    void iterate(size_t times = 1u);
protected:
    std::vector<uint8_t> m_tape;
    std::array<uint8_t, 8u> m_outputs;
};

typedef ElementaryCellularAutomatonT<> ElementaryCellularAutomaton;

#include "../src/ElementaryCellularAutomaton.cpp"

#endif // ELEMENTARYCELLULARAUTOMATON_H
