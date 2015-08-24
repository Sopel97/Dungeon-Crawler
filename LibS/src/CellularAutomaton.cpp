

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
size_t CellularAutomaton<Rules>::quantityOfStateIn3x3(States state, int x, int y) const
{
    return quantityOfStateInRadius(state, x, y, 3u);
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateIn5x5(States state, int x, int y) const
{
    return quantityOfStateInRadius(state, x, y, 5u);
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInRadius(States state, int x, int y, size_t radius) const
{
    return quantityOfStateInRegion(state, static_cast<int>(x) - radius/2, static_cast<int>(y) - radius/2, radius, radius);
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInRect(States state, const RectangleI& rect) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        int xmin = std::max(rect.min.x, 0);
        int ymin = std::max(rect.min.y, 0);
        int xmax = std::min(rect.max.x, static_cast<int>(m_grid.sizeX() - 1));
        int ymax = std::min(rect.max.y, static_cast<int>(m_grid.sizeY() - 1));

        for(int xx = xmin; xx <= xmax; ++xx)
        {
            for(int yy = ymin; yy <= ymax; ++yy)
            {
                if(m_grid(xx, yy) == state) ++quantity;
            }
        }
    }
    else if(m_topology == CellularAutomatonGridTopology::Toroidal)
    {
        int width = m_grid.sizeX();
        int height = m_grid.sizeY();

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
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInMooreNeighbourhood(States state, int x, int y) const
{
    size_t quantity = 0u;

    if(m_topology == CellularAutomatonGridTopology::Finite)
    {
        if(x > 0                                    && m_grid(x - 1, y) == state) ++quantity;
        if(x < static_cast<int>(m_grid.sizeX()) - 1 && m_grid(x + 1, y) == state) ++quantity;
        if(y > 0                                    && m_grid(x, y - 1) == state) ++quantity;
        if(y < static_cast<int>(m_grid.sizeY()) - 1 && m_grid(x, y + 1) == state) ++quantity;
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
size_t CellularAutomaton<Rules>::quantityOfStateInNeighbourhood(States state, int x, int y) const
{
    return quantityOfStateInRadius(state, x, y, 3u) - (m_grid(x, y) == state);
}
template <class Rules>
size_t CellularAutomaton<Rules>::quantityOfStateInRegion(States state, int x, int y, size_t w, size_t h) const
{
    return quantityOfStateInRect(state, RectangleI(Vec2I(x, y), w - 1u, h - 1u));
}
