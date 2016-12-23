#pragma once

#include "..\Fwd.h"

#include "..\Array2.h"
#include "Rectangle2.h"

namespace ls
{
    template <class Rule> //class representing a rule
    class CellularAutomaton
    {
    public:
        using RuleType = Rule;
        using State = typename Rule::State;

        enum class DefaultState
        {
            White,
            Black
        };
        enum class GridTopology
        {
            Finite,
            Toroidal //for toroidal space it is still required that passed coordinates are inside the bounds
        };


        CellularAutomaton(const Rule& rule, int width, int height, GridTopology topology = GridTopology::Finite);
        CellularAutomaton(const Rule& rule, int width, int height, State fillState, GridTopology topology = GridTopology::Finite);

        State at(int x, int y) const;

        void setState(int x, int y, State state) &;

        void fill(State state) &;
        template <class FillFunction>
        void fill(FillFunction fillingFunction) &; //fill function must take x, y as coordinates and output valid State

        void iterate(int numberOfIterations = 1) &;

        int occurencesIn3x3(State state, int x, int y) const; //x,y determine center of the 3x3 region
        int occurencesIn5x5(State state, int x, int y) const; //x,y determine center of the 5x5 region
        int occurencesInRadius(State state, int x, int y, int radius) const; //x,y determine center of the region, radius is a maximal chessboard distance
        int occurencesInRect(State state, const Rectangle2I& rect) const;
        int occurencesInMooreNeighbourhood(State state, int x, int y) const; //only sides
        int occurencesInNeighbourhood(State state, int x, int y) const; //sides and diagonals

    protected:
        Rule m_rule;
        GridTopology m_topology;
        Array2<State> m_grid;
    };

    template <class SetOfStates> //enum representing all possible states
    class QuantityRule3x3
    {
    public:
        using State = SetOfStates;

        QuantityRule3x3(State countedState, const std::array<State, 10u>& outputs) :
            m_countedState(countedState),
            m_outputs(outputs)
        {

        }

        State operator()(const CellularAutomaton<QuantityRule3x3>& automaton, int x, int y)
        {
            return m_outputs[automaton.occurencesIn3x3(m_countedState, x, y)];
        }

        void setOutputForQuantity(State outputState, int quantity)
        {
            m_outputs[quantity] = outputState;
        }

    protected:
        State m_countedState;
        std::array<State, 10u> m_outputs; //the quantity of state in 3x3 region is the index
    };

    class ConwaysGameOfLifeRule
    {
    public:
        enum class State
        {
            Dead,
            Live
        };

        ConwaysGameOfLifeRule() {}

        State operator()(const CellularAutomaton<ConwaysGameOfLifeRule>& automaton, int x, int y)
        {
            const int numberOfLivingNeighbours = automaton.occurencesInNeighbourhood(State::Live, x, y);
            const auto& currentState = automaton.at(x, y);

            if(currentState == State::Live)
            {
                if(numberOfLivingNeighbours < 2) return State::Dead;
                else if(numberOfLivingNeighbours > 3) return State::Dead;
            }
            else //Dead
            {
                if(numberOfLivingNeighbours == 3) return State::Live;
            }

            return currentState;
        }
    };
    class WireworldRule
    {
    public:
        enum class State
        {
            Empty,
            ElectronHead,
            ElectronTail,
            Conductor
        };

        WireworldRule() {}

        State operator()(const CellularAutomaton<WireworldRule>& automaton, int x, int y)
        {
            State currentState = automaton.at(x, y);

            if(currentState == State::Empty) return State::Empty;
            else if(currentState == State::ElectronHead) return State::ElectronTail;
            else if(currentState == State::ElectronTail) return State::Conductor;
            else //(currentState == State::Conductor)
            {
                const int numberOfElectronHeadsInNeighbourhood = automaton.occurencesInMooreNeighbourhood(State::ElectronHead, x, y);
                if(numberOfElectronHeadsInNeighbourhood == 1 || numberOfElectronHeadsInNeighbourhood == 2) 
                    return State::ElectronHead;
            }

            return currentState;
        }
    };
}

#include "../src/CellularAutomaton.cpp"
