#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H

enum class OriginalCellularAutomatonStates
{
    White,
    Black
};

enum class CellularAutomatonGridTopology
{
    Finite,
    Toroidal //for toroidal space it is still required that passed coordinates are valid finite coordinates
};

template <class Rules> //class representing a rule
class CellularAutomaton
{
public:
    typedef typename Rules::States States;

    CellularAutomaton(const Rules& rule, size_t width, size_t height, CellularAutomatonGridTopology topology = CellularAutomatonGridTopology::Finite);
    CellularAutomaton(const Rules& rule, size_t width, size_t height, States fillState, CellularAutomatonGridTopology topology = CellularAutomatonGridTopology::Finite);

    States cellAt(size_t x, size_t y) const;

    void setCell(size_t x, size_t y, States state);

    void fill(States fillState);
    template <class FillFunction>
    void fill(FillFunction fillFuntion); //fill function must take x, y as coordinates and output value from States

    void iterate(size_t times = 1u); //can be QuantityRules or any other with proper () operator

    size_t quantityOfStateIn3x3(States state, size_t x, size_t y) const; //x,y determine center of the 3x3 region
    size_t quantityOfStateInMooreNeighbourhood(States state, size_t x, size_t y) const; //only sides
    size_t quantityOfStateInNeighbourhood(States state, size_t x, size_t y) const; //sides and diagonals
    size_t quantityOfStateInRegion(States state, size_t x, size_t y, size_t w, size_t h) const; //x,y determine top left of the region. w,h determine size. Does NOT check bounds

protected:
    Rules m_rules;
    Array2<States> m_grid;
    CellularAutomatonGridTopology m_topology;
};

template <class PossibleStates> //enum representing all possible states
class QuantityRules
{
public:
    typedef PossibleStates States;

    QuantityRules(States state, const std::array<States, 10u>& outputs) :
        m_state(state),
        m_outputs(outputs)
    {

    }

    States operator()(const CellularAutomaton<QuantityRules>& automaton, size_t x, size_t y)
    {
        return m_outputs[automaton.quantityOfStateIn3x3(m_state, x, y)];
    }

    void setOutputForQuantity(States outputState, size_t quantity)
    {
        m_outputs[quantity] = outputState;
    }

protected:
    States m_state;
    std::array<States, 10u> m_outputs; //the quantity of state in 3x3 region is the index
};

class ConwaysGameOfLifeRules
{
public:
    enum class States
    {
        Dead,
        Live
    };

    ConwaysGameOfLifeRules(){}

    States operator()(const CellularAutomaton<ConwaysGameOfLifeRules>& automaton, size_t x, size_t y)
    {
        size_t numberOfLiveNeighbours = automaton.quantityOfStateInNeighbourhood(States::Live, x, y);
        const auto& thisCell = automaton.cellAt(x, y);

        if(thisCell == States::Live)
        {
            if(numberOfLiveNeighbours < 2u) return States::Dead;
            if(numberOfLiveNeighbours > 3u) return States::Dead;

            return States::Live;
        }
        else //Dead
        {
            if(numberOfLiveNeighbours == 3u) return States::Live;

            return States::Dead;
        }
    }
};
class WireworldRules
{
public:
    enum class States
    {
        Empty,
        ElectronHead,
        ElectronTail,
        Conductor
    };

    WireworldRules(){}

    States operator()(const CellularAutomaton<WireworldRules>& automaton, size_t x, size_t y)
    {
        const auto& thisCell = automaton.cellAt(x, y);

        if(thisCell == States::Empty) return States::Empty;
        else if(thisCell == States::ElectronHead) return States::ElectronTail;
        else if(thisCell == States::ElectronTail) return States::Conductor;
        else if(thisCell == States::Conductor)
        {
            size_t numberOfElectronHeadsInNeighbourhood = automaton.quantityOfStateInMooreNeighbourhood(States::ElectronHead, x, y);
            if(numberOfElectronHeadsInNeighbourhood == 1u || numberOfElectronHeadsInNeighbourhood == 2u) return States::ElectronHead;

            return States::Conductor;
        }
    }
};

#include "../src/CellularAutomaton.cpp"

#endif // CELLULARAUTOMATON_H
