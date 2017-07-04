#pragma once

#include "EntityRenderer.h"

#include "Configuration.h"

#include "ResourceLoaders.h"

#include <memory>

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class Entity;

class MovingEntityRenderer : public EntityRenderer
{
public:
    MovingEntityRenderer(Entity& owner);
    MovingEntityRenderer(const MovingEntityRenderer& other, Entity& owner);
    ~MovingEntityRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) const override;

    const sf::Texture& texture() const;

    std::unique_ptr<EntityRenderer> clone(Entity& owner) const override;
protected:
    ResourceHandle<sf::Texture> m_texture;
    ResourceHandle<sf::Texture> m_metaTexture;
    ls::Vec2I m_sprites;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const sf::Texture& texture) const;
};
