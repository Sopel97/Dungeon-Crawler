#include "TallDrawable.h"

#include "TallEntityDrawable.h"
#include "TallTileColumnDrawable.h"

#include "GameConstants.h"

using namespace ls;

TallDrawable::TallDrawable()
{

}
TallDrawable::~TallDrawable()
{

} 

bool TallDrawable::compare(const TallDrawable& lhs, const TallDrawable& rhs)
{
    //should return true if lhs is closer to top left than rhs (ie. should be rendered before rhs)

    bool isLhsTile = lhs.type() == DrawableType::TileStack;
    bool isRhsTile = rhs.type() == DrawableType::TileStack;

    if(isLhsTile && isRhsTile) //tile, tile
    {
        const TallTileColumnDrawable& lhsTile = static_cast<const TallTileColumnDrawable&>(lhs);
        const TallTileColumnDrawable& rhsTile = static_cast<const TallTileColumnDrawable&>(rhs);

        int lhsY = lhsTile.tileY();
        int lhsX = lhsTile.tileX();
        int rhsY = rhsTile.tileY();
        int rhsX = rhsTile.tileX();

        return lhsX + lhsY < rhsX + rhsY;
    }
    else if(!isLhsTile && !isRhsTile) //entity, entity
    {
        const Vec2F& lhsCenter = lhs.center();
        const Vec2F& rhsCenter = rhs.center();

        return lhsCenter.x + lhsCenter.y < rhsCenter.x + rhsCenter.y;
    }
    else //entity, tile
    {
        bool negateResult = !isLhsTile; //tile should be in lhs. Result is negated if it isn't
        const TallDrawable& tile = isLhsTile ? lhs : rhs;
        const TallDrawable& entity = isLhsTile ? rhs : lhs;

        const TallTileColumnDrawable& lhsTile = static_cast<const TallTileColumnDrawable&>(tile);
        const TallEntityDrawable& rhsEntity = static_cast<const TallEntityDrawable&>(entity);

        bool result = false;

        const Rectangle2F& entityCollider = rhsEntity.boundingRectangle();
        Vec2F entityTopRight(entityCollider.max.x, entityCollider.min.y);
        Vec2F entityBottomLeft(entityCollider.min.x, entityCollider.max.y);
        const Rectangle2F& tileCollider = lhsTile.boundingRectangle();
        Vec2F tileTopRight(tileCollider.max.x, tileCollider.min.y);
        Vec2F tileBottomLeft(tileCollider.min.x, tileCollider.max.y);

        if(entityBottomLeft.x > tileTopRight.x)
        {
            result = (tileTopRight.x + tileTopRight.y < entityBottomLeft.x + entityBottomLeft.y);
        }
        else if(entityTopRight.y > tileBottomLeft.y)
        {
            result = (tileBottomLeft.x + tileBottomLeft.y < entityTopRight.x + entityTopRight.y);
        }

        if(negateResult) return !result;
        else return result;

    }

    return true;
}

bool TallDrawable::ptrCompare(const TallDrawable* lhs, const TallDrawable* rhs)
{
    return compare(*lhs, *rhs);
}
