#include "configuration/Configuration.h"

Configuration::Configuration(const std::string& path) :
    m_table(LuaTable::fromFile(path.c_str()))
{
}

Configuration::~Configuration()
{

}

ConfigurationNode Configuration::operator[](const std::string& nodePath)
{
    return m_table[nodePath.c_str()];
}

ConfigurationNode Configuration::operator[](int index)
{
    return m_table[index];
}

LuaTable& Configuration::luaTable()
{
    return m_table;
}
