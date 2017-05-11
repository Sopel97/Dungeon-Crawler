#pragma once

#include "../LibS/Shapes.h"

class AspectRatio
{
private:
    float m_ratio;
public:
    AspectRatio(float r);
    AspectRatio(int h, int v);

    ls::Rectangle2I fitRectCentered(const ls::Rectangle2I& space) const;

    float ratio() const;
    void setRatio(float r);
    void setRatio(int h, int v);
};