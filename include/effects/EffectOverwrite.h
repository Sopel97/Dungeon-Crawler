#pragma once

#include <map>
#include <vector>

enum class EffectOverwrite
{
    Never,
    Longer,
    Always,
    NumEffectOverwrites
};

class EffectOverwriteHelper
{
private:
    static const std::map<std::string, EffectOverwrite> m_stringToEnum;
    static const std::vector<std::string> m_enumToString;

public:
    static EffectOverwrite stringToEnum(const std::string& s);
    static std::string enumToString(EffectOverwrite id);

    static constexpr int numAttributes()
    {
        return static_cast<int>(EffectOverwrite::NumEffectOverwrites);
    }
};