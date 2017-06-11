#pragma once

#include "Configuration.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class EntityModel;
class EntityRenderer;
class EntityController;
class Entity;
class ComponentCommonData;

class EntityPrefab
{
public:
    EntityPrefab(std::unique_ptr<EntityModel> model, std::unique_ptr<EntityRenderer> renderer, std::unique_ptr<EntityController> controller);
    ~EntityPrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    std::unique_ptr<Entity> instantiate() const;
protected:
    std::unique_ptr<ComponentCommonData> m_modelCommonData;
    std::unique_ptr<ComponentCommonData> m_rendererCommonData;
    std::unique_ptr<ComponentCommonData> m_controllerCommonData;

    std::unique_ptr<Entity> m_entity;

    static int m_lastId;
};

