#include "effects/EffectPrefab.h"

int EffectPrefab::m_lastId = -1;

EffectPrefab::EffectPrefab() :
    m_id(++m_lastId)
{
}

EffectPrefab::~EffectPrefab()
{
}

void EffectPrefab::loadFromConfiguration(ConfigurationNode& config)
{
    ConfigurationNode attributesConfig = config["attributes"];
    const int numAttributes = attributesConfig.length();
    for (int i = 1; i <= numAttributes; ++i)
    {
        const std::string attributeName = attributesConfig[i][1].get<std::string>();
        const AttributeId attributeId = AttributeIdHelper::stringToEnum(attributeName);
        const int attributeValue = attributesConfig[i][2].get<int>();
        m_attributes += Attribute{ attributeId, attributeValue };
    }

    ConfigurationNode durationConfig = config["duration"];
    if (durationConfig.exists())
    {
        m_duration = durationConfig.get<float>();
    }
    else
    {
        m_duration = std::nullopt;
    }
}

Effect EffectPrefab::instantiate() const
{
    return Effect(m_id, m_attributes, m_duration);
}
