#include "TallDrawable.h"

#include "TallEntityDrawable.h"
#include "TallTileStackDrawable.h"

using namespace Geo;

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
        const TallTileStackDrawable& lhsTile = static_cast<const TallTileStackDrawable&>(lhs);
        const TallTileStackDrawable& rhsTile = static_cast<const TallTileStackDrawable&>(rhs);

        int lhsY = lhsTile.tileY();
        int rhsY = rhsTile.tileY();
        if(lhsY < rhsY) return true;
        else if(lhsY == rhsY)
        {
            int lhsX = lhsTile.tileX();
            int rhsX = rhsTile.tileX();

            return lhsX < rhsX; //should never be equal btw
        }
    }
    else if(!isLhsTile && !isRhsTile) //entity, entity
    {
        const TallEntityDrawable& lhsEntity = static_cast<const TallEntityDrawable&>(lhs);
        const TallEntityDrawable& rhsEntity = static_cast<const TallEntityDrawable&>(rhs);

        const Vec2F& lhsCenter = lhs.center();
        const Vec2F& rhsCenter = rhs.center();

        return lhsCenter.x+lhsCenter.y < rhsCenter.x+rhsCenter.y;
    }
    else //entity, tile
    {
        bool negateResult = !isLhsTile; //tile should be in lhs. Result is negated if it isn't
        const TallDrawable& tile = isLhsTile ? lhs : rhs;
        const TallDrawable& entity = isLhsTile ? rhs : lhs;

        const TallTileStackDrawable& lhsTile = static_cast<const TallTileStackDrawable&>(tile);
        const TallEntityDrawable& rhsEntity = static_cast<const TallEntityDrawable&>(entity);

        //TODO: this

        bool result = false;

        if(negateResult) return !result;
        else return result;

    }

    return false;
}

bool TallDrawable::ptrCompare(const TallDrawable* lhs, const TallDrawable* rhs)
{
    return compare(*lhs, *rhs);
}
