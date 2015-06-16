template <class StandardEngine>
StandardRandomNumberGeneratorWrapper<StandardEngine>::StandardRandomNumberGeneratorWrapper(typename StandardEngine::result_type seed) :
    RandomEngineT<typename StandardEngine::result_type>(seed, StandardEngine::max()),
    m_engine(seed)
{

}
template <class StandardEngine>
StandardRandomNumberGeneratorWrapper<StandardEngine>::~StandardRandomNumberGeneratorWrapper()
{

}
template <class StandardEngine>
typename StandardEngine::result_type StandardRandomNumberGeneratorWrapper<StandardEngine>::nextRaw()
{
    return m_engine();
}

template <class StandardEngine>
std::unique_ptr<RandomEngineBase> StandardRandomNumberGeneratorWrapper<StandardEngine>::clone() const
{
    return std::make_unique<StandardRandomNumberGeneratorWrapper<StandardEngine>>(*this);
}
