#include "AspectRatio.h"

AspectRatio::AspectRatio(float r) :
    m_ratio(r)
{

}
AspectRatio::AspectRatio(int h, int v) :
    m_ratio(static_cast<float>(h) / v)
{

}

ls::Rectangle2I AspectRatio::fitRectCentered(const ls::Rectangle2I& space) const
{
    //try full height
    {
        const int fullHeight = space.height();
        const int width = fullHeight * m_ratio;
        const int widthDiff = space.width() - width;
        if (widthDiff >= 0)
            return ls::Rectangle2I::withSize(space.min + ls::Vec2I(widthDiff / 2, 0), width, fullHeight);
    }

    //go with full width
    {
        const int fullWidth = space.width();
        const int height = fullWidth / m_ratio;
        const int heightDiff = space.height() - height;

        return ls::Rectangle2I::withSize(space.min + ls::Vec2I(0, heightDiff / 2), fullWidth, height);
    }
}

float AspectRatio::ratio() const
{
    return m_ratio;
}
void AspectRatio::setRatio(float r)
{
    m_ratio = r;
}
void AspectRatio::setRatio(int h, int v)
{
    m_ratio = static_cast<float>(h) / v;
}