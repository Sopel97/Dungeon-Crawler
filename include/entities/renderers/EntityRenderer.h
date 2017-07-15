#pragma once 

#include "Configuration.h"

#include "ComponentCommonData.h"
#include "entities/EntityComponent.h"

#include <memory>

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Entity;
class SpriteBatch;

class EntityRenderer : public EntityComponent<EntityRenderer, Entity>
{
public:
    EntityRenderer(Entity& owner);
    EntityRenderer(const EntityRenderer& other, Entity& owner);
    ~EntityRenderer() override;

    virtual void draw(SpriteBatch& spriteBatch) const;
    virtual void drawMeta(SpriteBatch& spriteBatch) const;

    std::unique_ptr<EntityRenderer> clone(Entity& owner) const override;
};
