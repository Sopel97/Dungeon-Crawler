#pragma once

#include "Configuration.h"

#include "ComponentFactory.h"

#include <memory>

#include "../LibS/Shapes.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class MapLayer;
class ProjectileModel;
class ProjectileRenderer;
class ProjectileController;
class Projectile;
class ComponentCommonData;
class World;
class Entity;

class ProjectilePrefab
{
public:
    ProjectilePrefab(
        const ComponentFactory<Projectile, ProjectileModel>& modelFac,
        const ComponentFactory<Projectile, ProjectileRenderer>& rendererFac,
        const ComponentFactory<Projectile, ProjectileController>& controllerFac);
    ~ProjectilePrefab();

    void loadFromConfiguration(ConfigurationNode& config);

    std::unique_ptr<Projectile> instantiate(World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition) const;
protected:
    std::unique_ptr<Projectile> m_projectile;

    const ComponentFactory<Projectile, ProjectileModel>* m_modelFactory;
    const ComponentFactory<Projectile, ProjectileRenderer>* m_rendererFactory;
    const ComponentFactory<Projectile, ProjectileController>* m_controllerFactory;

    static int m_lastId;
};
