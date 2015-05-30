#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <lua.h>
#include <string>

#include "../luatables/luatables.h"

typedef LuaTableNode ConfigurationNode;

class Configuration
{
public:
    Configuration(const std::string& path);
    virtual ~Configuration();

    ConfigurationNode operator[](const std::string& nodePath);
    ConfigurationNode operator[](int index);
protected:
    LuaTable m_table;
    std::string m_path;
private:
};


#endif // CONFIGURATION_H
