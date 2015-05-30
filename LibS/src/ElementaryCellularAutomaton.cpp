template <int I>
ElementaryCellularAutomatonT<I>::ElementaryCellularAutomatonT(size_t rule, size_t width, size_t origin) :
    m_tape(width, 0u)
{
    m_tape[origin] = 1u;
    for(size_t i = 0; i < 8u; ++i)
    {
        m_outputs[i] = rule % 2u;
        rule /= 2u;
    }
}
template <int I>
ElementaryCellularAutomatonT<I>::ElementaryCellularAutomatonT(size_t rule, const std::vector<uint8_t>& initialState) :
    m_tape(initialState)
{

}
template <int I>
ElementaryCellularAutomatonT<I>::ElementaryCellularAutomatonT(size_t rule, std::vector<uint8_t>&& initialState) :
    m_tape(std::move(initialState))
{

}

template <int I>
void ElementaryCellularAutomatonT<I>::iterate(size_t times)
{
    size_t size = m_tape.size();

    std::vector<uint8_t> newTape;
    newTape.reserve(size);

    for(size_t i = 1; i < size - 1; ++i)
    {
        size_t value = 0u;
        value += m_tape[i - 1] << 2;
        value += m_tape[i] << 1;
        value += m_tape[i + 1];

        newTape[i] = m_outputs[value];
    }
    newTape[0] = m_outputs[(m_tape[0] << 1) + m_tape[1]];
    newTape[size - 1] = m_outputs[(m_tape[size - 2] << 1) + m_tape[size - 1]];

    m_tape = std::move(newTape);
}
