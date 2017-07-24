#pragma once

#include "Configuration.h"

#include "ComponentFactory.h"

#include "../LibS/Shapes.h"

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
    EntityPrefab(
        const ComponentFactory<Entity, EntityModel>& modelFac,
        const ComponentFactory<Entity, EntityRenderer>& rendererFac);
    ~EntityPrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    std::unique_ptr<Entity> instantiate(const ls::Vec2F& pos) const;
protected:
    std::unique_ptr<Entity> m_entity;

    const ComponentFactory<Entity, EntityModel>* m_modelFactory;
    const ComponentFactory<Entity, EntityRenderer>* m_rendererFactory;

    static int m_lastId;
};

