#include "effects/EffectOverwrite.h"

const std::map<std::string, EffectOverwrite> EffectOverwriteHelper::m_stringToEnum{
    { "Never", EffectOverwrite::Never },
    { "Longer", EffectOverwrite::Longer },
    { "Always", EffectOverwrite::Always }
};
const std::vector<std::string> EffectOverwriteHelper::m_enumToString{
    "Never",
    "Longer",
    "Always"
};

EffectOverwrite EffectOverwriteHelper::stringToEnum(const std::string& s)
{
    return m_stringToEnum.at(s);
}
std::string EffectOverwriteHelper::enumToString(EffectOverwrite id)
{
    return m_enumToString[static_cast<int>(id)];
}