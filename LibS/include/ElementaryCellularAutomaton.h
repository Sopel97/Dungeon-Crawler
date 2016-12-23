#pragma once

#include "..\Fwd.h"

#include <vector>
#include <array>

namespace ls
{
    //TODO: circular, finite 

    template <class T>
    class ElementaryCellularAutomatonTemplate
    {
    public:
        enum class TapeTopology
        {
            Finite,
            Circular
        };

        ElementaryCellularAutomatonTemplate(uint8_t rule, int width, int origin = 0, TapeTopology topology = TapeTopology::Finite);
        ElementaryCellularAutomatonTemplate(uint8_t rule, const std::vector<uint8_t>& initialState, TapeTopology topology = TapeTopology::Finite);
        ElementaryCellularAutomatonTemplate(uint8_t rule, std::vector<uint8_t>&& initialState, TapeTopology topology = TapeTopology::Finite);

        void setRule(uint8_t rule) &;

        void iterate(int iterations = 1) &;

        const std::vector<uint8_t>& tape() const &;
        std::vector<uint8_t> tape() &&;
    protected:
        TapeTopology m_topology;
        std::vector<uint8_t> m_tape; //TODO: consider using bool
        std::array<uint8_t, 8u> m_outputs;
    };

}

#include "../src/ElementaryCellularAutomaton.cpp"
