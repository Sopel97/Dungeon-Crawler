#include "..\include\CellularAutomaton.h"

namespace ls
{
    template <class Rule>
    CellularAutomaton<Rule>::CellularAutomaton(const Rule& rule, int width, int height, GridTopology topology) :
        m_rule(rule),
        m_topology(topology),
        m_grid(width, height)
    {

    }

    template <class Rule>
    CellularAutomaton<Rule>::CellularAutomaton(const Rule& rule, int width, int height, State fillState, GridTopology topology) :
        m_rule(rule),
        m_topology(topology),
        m_grid(width, height, fillState)
    {

    }

    template <class Rule>
    typename CellularAutomaton<Rule>::State CellularAutomaton<Rule>::at(int x, int y) const
    {
        return m_grid(x, y);
    }

    template <class Rule>
    void CellularAutomaton<Rule>::fill(State state) &
    {
        for(auto& cell : m_grid)
        {
            cell = state;
        }
    }
    template <class Rule>
    template <class FillFunction>
    void CellularAutomaton<Rule>::fill(FillFunction fillingFunction) &
    {
        const int w = m_grid.width();
        const int h = m_grid.height();

        for(int x = 0; x < w; ++x)
        {
            for(int y = 0; y < h; ++y)
            {
                m_grid(x, y) = fillingFunction(x, y);
            }
        }
    }

    template <class Rule>
    void CellularAutomaton<Rule>::setState(int x, int y, State state) &
    {
        m_grid(x, y) = state;
    }

    template <class Rule>
    void CellularAutomaton<Rule>::iterate(int iterations) &
    {
        const int w = m_grid.width();
        const int h = m_grid.height();

        Array2<State> nextGrid(w, h);
        while(iterations--)
        {
            for(int x = 0; x < w; ++x)
            {
                for(int y = 0; y < h; ++y)
                {
                    nextGrid(x, y) = m_rule(*this, x, y);
                }
            }

            m_grid.swap(nextGrid);
        }
    }

    template <class Rule>
    int CellularAutomaton<Rule>::occurencesIn3x3(State state, int x, int y) const
    {
        return occurencesInRadius(state, x, y, 1);
    }
    template <class Rule>
    int CellularAutomaton<Rule>::occurencesIn5x5(State state, int x, int y) const
    {
        return occurencesInRadius(state, x, y, 2);
    }
    template <class Rule>
    int CellularAutomaton<Rule>::occurencesInRadius(State state, int x, int y, int radius) const
    {
        const Vec2I diagonalVector(radius, radius);
        const Vec2I centerVector(x, y);
        return occurencesInRect(state, Rectangle2I(centerVector - diagonalVector, centerVector + diagonalVector));
    }
    template <class Rule>
    int CellularAutomaton<Rule>::occurencesInRect(State state, const Rectangle2I& rect) const
    {
        const int width = m_grid.width();
        const int height = m_grid.height();
        
        int quantity = 0;

        if(m_topology == GridTopology::Finite)
        {
            int xmin = std::max(rect.min.x, 0);
            int ymin = std::max(rect.min.y, 0);
            int xmax = std::min(rect.max.x, width - 1);
            int ymax = std::min(rect.max.y, height - 1);

            for(int xx = xmin; xx <= xmax; ++xx)
            {
                for(int yy = ymin; yy <= ymax; ++yy)
                {
                    if(m_grid(xx, yy) == state) ++quantity;
                }
            }
        }
        else if(m_topology == GridTopology::Toroidal)
        {
            for(int xx = rect.min.x; xx <= rect.max.x; ++xx)
            {
                for(int yy = rect.min.y; yy <= rect.max.y; ++yy)
                {
                    if(m_grid((xx + width) % width, (yy + height) % height) == state) ++quantity;
                }
            }
        }

        return quantity;
    }

    template <class Rule>
    int CellularAutomaton<Rule>::occurencesInMooreNeighbourhood(State state, int x, int y) const
    {
        const int width = m_grid.width();
        const int height = m_grid.height();

        int quantity = 0;

        if(m_topology == GridTopology::Finite)
        {
            if(x > 0 && m_grid(x - 1, y) == state) ++quantity;
            if(x < width - 1 && m_grid(x + 1, y) == state) ++quantity;
            if(y > 0 && m_grid(x, y - 1) == state) ++quantity;
            if(y < height - 1 && m_grid(x, y + 1) == state) ++quantity;
        }
        else if(m_topology == GridTopology::Toroidal)
        {
            if(m_grid((x - 1 + width) % width, y) == state) ++quantity;
            if(m_grid((x + 1) % width, y) == state) ++quantity;
            if(m_grid(x, (y - 1 + height) % height) == state) ++quantity;
            if(m_grid(x, (y + 1) % height) == state) ++quantity;
        }

        return quantity;
    }

    template <class Rule>
    int CellularAutomaton<Rule>::occurencesInNeighbourhood(State state, int x, int y) const
    {
        return occurencesIn3x3(state, x, y) - (m_grid(x, y) == state);
    }
}