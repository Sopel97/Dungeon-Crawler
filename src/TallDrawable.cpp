#include "TallDrawable.h"


TallDrawable::TallDrawable()
{

}
TallDrawable::~TallDrawable()
{

}

bool TallDrawable::isTile() const
{
    return false;
}
bool TallDrawable::isEntity() const
{
    return false;
}

bool TallDrawable::compare(const TallDrawable& lhs, const TallDrawable& rhs)
{
    return false; //TODO: this
}
