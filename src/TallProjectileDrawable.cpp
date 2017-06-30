#include "TallProjectileDrawable.h"

#include "projectiles/Projectile.h"
#include "projectiles/models/ProjectileModel.h"
#include "projectiles/renderers/ProjectileRenderer.h"

using namespace ls;

TallProjectileDrawable::TallProjectileDrawable(Projectile* projectile) :
    m_projectile(projectile)
{
    float radius = m_projectile->model().collider().radius;
    Vec2F halfSize{ radius / 2.0f, radius / 2.0f };
    m_center = m_projectile->model().position();
    m_boundingRectangle = Rectangle2F(m_center - halfSize, m_center + halfSize);
}

TallProjectileDrawable::~TallProjectileDrawable()
{

}

const Rectangle2F& TallProjectileDrawable::boundingRectangle() const
{
    return m_boundingRectangle;
}
const Vec2F& TallProjectileDrawable::center() const
{
    return m_center;
}

TallDrawable::DrawableType TallProjectileDrawable::type() const
{
    return TallDrawable::DrawableType::Projectile;
}

void TallProjectileDrawable::draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_projectile->renderer().draw(renderTarget, renderStates);
}
void TallProjectileDrawable::drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates)
{
    m_projectile->renderer().drawMeta(renderTarget, renderStates);
}
