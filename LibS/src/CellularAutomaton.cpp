

template <class Rules>
CellularAutomaton<Rules>::CellularAutomaton(const Rules& rules, size_t width, size_t height, CellularAutomatonGridTopology topology) :
    m_rules(rules),
    m_grid(width, height),
    m_topology(topology)
{

}

template <class Rules>
CellularAutomaton<Rules>::CellularAutomaton(const Rules& rules, size_t width, size_t height, States fillState, CellularAutomatonGridTopology topology) :
    m_rules(rules),
    m_grid(width, height, fillState),
    m_topology(topology)
{

}

template <class Rules>
typename CellularAutomaton<Rules>::States CellularAutomaton<Rules>::cellAt(size_t x, size_t y) const
{
    return m_grid(x, y);
}

template <class Rules>
void CellularAutomaton<Rules>::fill(States fillState)
{
    for(auto& cell : m_grid)
    {
        cell = fillState;
    }
}
template <class Rules>
template <class FillFunction>
void CellularAutomaton<Rules>::fill(FillFunction fillFunction)
{
    size_t w = m_grid.sizeX();
    size_t h = m_grid.sizeY();

    for(size_t x = 0u; x < w; ++x)
    {
        for(size_t y = 0u; y < h; ++y)
        {
            m_grid(x, y) = fillFunction(x, y);
        }
    }
}

template <class Rules>
void CellularAutomaton<Rules>::setCell(size_t x, size_t y, States state)
{
    m_grid(x, y) = state;
}

template <class Rules>
void CellularAutomaton<Rules>::iterate(size_t times)
{
    size_t w = m_grid.sizeX();
    size_t h = m_grid.sizeY();

    while(times--)
    {
        Array2<States> nextGrid(w, h);

        for(size_t x = 0u; x < w; ++x)
        {
            for(size_t y = 0u; y < h; ++y)
            {
                nextGrid(x, y) = m_rules(*this, x, y);
            }
        }

        m_grid = std::move(nextGrid);
    }
}

template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateIn3x3(States state, size_t x, size_t y) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        size_t xmin = std::max(x, 1u) - 1u;
        size_t ymin = std::max(y, 1u) - 1u;
        size_t xmax = std::min(x + 1, m_grid.sizeX() - 1u);
        size_t ymax = std::min(y + 1, m_grid.sizeY() - 1u);

        for(size_t xx = xmin; xx <= xmax; ++xx)
        {
            for(size_t yy = ymin; yy <= ymax; ++yy)
            {
                if(m_grid(xx, yy) == state) ++quantity;
            }
        }
    }
    else if(m_topology == CellularAutomatonGridTopology::Toroidal)
    {
        int width = m_grid.sizeX();
        int height = m_grid.sizeY();

        int xmin = static_cast<int>(x) - 1;
        int ymin = static_cast<int>(y) - 1;
        int xmax = static_cast<int>(x) + 1;
        int ymax = static_cast<int>(y) + 1;

        for(int xx = xmin; xx <= xmax; ++xx)
        {
            for(int yy = ymin; yy <= ymax; ++yy)
            {
                if(m_grid((xx + width) % width, (yy + height) % height) == state) ++quantity; //+width/height is to prevent undefined modulo operation on negative numbers
            }
        }
    }

    return quantity;
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInMooreNeighbourhood(States state, size_t x, size_t y) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        if(x > 0                  && m_grid(x - 1, y) == state) ++quantity;
        if(x < m_grid.sizeX() - 1 && m_grid(x + 1, y) == state) ++quantity;
        if(y > 0                  && m_grid(x, y - 1) == state) ++quantity;
        if(y < m_grid.sizeY() - 1 && m_grid(x, y + 1) == state) ++quantity;
    }
    else if(m_topology == CellularAutomatonGridTopology::Toroidal)
    {
        int width = m_grid.sizeX();
        int height = m_grid.sizeY();

        if(m_grid((static_cast<int>(x) - 1 + width) % width, static_cast<int>(y)) == state) ++quantity;
        if(m_grid((static_cast<int>(x) + 1) % width, static_cast<int>(y)) == state) ++quantity;
        if(m_grid(static_cast<int>(x), (static_cast<int>(y) - 1 + height) % height) == state) ++quantity;
        if(m_grid(static_cast<int>(x), (static_cast<int>(y) + 1) % height) == state) ++quantity;
    }
    return quantity;
}

template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInNeighbourhood(States state, size_t x, size_t y) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        size_t xmin = std::max(x, 1u) - 1u;
        size_t ymin = std::max(y, 1u) - 1u;
        size_t xmax = std::min(x + 1, m_grid.sizeX() - 1u);
        size_t ymax = std::min(y + 1, m_grid.sizeY() - 1u);

        for(size_t xx = xmin; xx <= xmax; ++xx)
        {
            for(size_t yy = ymin; yy <= ymax; ++yy)
            {
                if(xx == x && yy == y) continue;
                if(m_grid(xx, yy) == state) ++quantity;
            }
        }
    }
    else if(m_topology == CellularAutomatonGridTopology::Toroidal)
    {
        int width = m_grid.sizeX();
        int height = m_grid.sizeY();

        int xmin = static_cast<int>(x) - 1;
        int ymin = static_cast<int>(y) - 1;
        int xmax = static_cast<int>(x) + 1;
        int ymax = static_cast<int>(y) + 1;

        for(int xx = xmin; xx <= xmax; ++xx)
        {
            for(int yy = ymin; yy <= ymax; ++yy)
            {
                int xxx = (xx + width) % width;
                int yyy = (yy + height) % height;
                if(xxx == static_cast<int>(x) && yyy == static_cast<int>(y)) continue;

                if(m_grid(xxx, yyy) == state) ++quantity;
            }
        }
    }

    return quantity;
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInRegion(States state, size_t x, size_t y, size_t w, size_t h) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        size_t xmax = std::min(x + w, m_grid.sizeX() - 1);
        size_t ymax = std::min(y + h, m_grid.sizeY() - 1);

        for(size_t xx = x; xx <= xmax; ++xx)
        {
            for(size_t yy = y; yy <= ymax; ++yy)
            {
                if(m_grid(xx, yy) == state) ++quantity;
            }
        }
    }
    else if(m_topology == CellularAutomatonGridTopology::Toroidal)
    {
        int width = m_grid.sizeX();
        int height = m_grid.sizeY();

        size_t xmax = x + w;
        size_t ymax = y + h;

        for(int xx = x; xx <= xmax; ++xx)
        {
            for(int yy = y; yy <= ymax; ++yy)
            {
                if(m_grid(xx % width, yy % height) == state) ++quantity;
            }
        }
    }

    return quantity;
}
