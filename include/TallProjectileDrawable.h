#pragma once

#include "TallDrawable.h"

#include "../LibS/Geometry.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
}
class Projectile;

class TallProjectileDrawable : public TallDrawable
{
public:
    TallProjectileDrawable(Projectile* projectile);
    ~TallProjectileDrawable() override;

    const ls::Rectangle2F& boundingRectangle() const override;
    const ls::Vec2F& center() const override;

    TallDrawable::DrawableType type() const override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates) override;

protected:
    Projectile* m_projectile;
    ls::Rectangle2F m_boundingRectangle;
    ls::Vec2F m_center;
};
