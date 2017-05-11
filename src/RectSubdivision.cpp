#include "RectSubdivision.h"

RectSubdivision RectSubdivision::withPixels(RectSubdivision::Orientation orientation, RectSubdivision::Subject subject, int pixels)
{
    return RectSubdivision(orientation, subject, pixels);
}

RectSubdivision RectSubdivision::withRatio(RectSubdivision::Orientation orientation, RectSubdivision::Subject subject, float ratio)
{
    return RectSubdivision(orientation, subject, ratio);
}
RectSubdivision& RectSubdivision::withAspectRatio(AspectRatio ratio, RectSubdivision::Subject subject)
{
    if (subject == Subject::First) m_aspectRatioFirst = ratio;
    else m_aspectRatioSecond = ratio;

    return *this;
}

std::pair<ls::Rectangle2I, ls::Rectangle2I> RectSubdivision::calculateSubRects(const ls::Rectangle2I& rect) const
{
    const int fullWidth = rect.width();
    const int fullHeight = rect.height();

    int width1 = fullWidth;
    int height1 = fullHeight;
    int width2 = fullWidth;
    int height2 = fullHeight;
    ls::Vec2I offset(0, 0);

    if (m_orientation == Orientation::Horizontal)
    {
        width1 = asPixels(rect);
        width2 = fullWidth - width1;

        if (m_subject == Subject::Second) std::swap(width1, width2);

        offset.x = width1;
    }
    else
    {
        height1 = asPixels(rect);
        height2 = fullHeight - height1;

        if (m_subject == Subject::Second) std::swap(height1, height2);

        offset.y = height1;
    }

    ls::Rectangle2I rect1 = ls::Rectangle2I::withSize(rect.min, width1, height1);
    ls::Rectangle2I rect2 = ls::Rectangle2I::withSize(rect.min + offset, width2, height2);

    if (m_aspectRatioFirst.has_value())
    {
        rect1 = m_aspectRatioFirst.value().fitRectCentered(rect1);
    }
    if (m_aspectRatioSecond.has_value())
    {
        rect2 = m_aspectRatioSecond.value().fitRectCentered(rect2);
    }

    return { rect1, rect2 };
}

int RectSubdivision::asPixels(const ls::Rectangle2I& rect) const
{
    if (m_amountType == AmountType::Pixels) return m_amount.pixels;
    else return m_amount.ratio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}
float RectSubdivision::asRatio(const ls::Rectangle2I& rect) const
{
    if (m_amountType == AmountType::Ratio) return m_amount.ratio;
    else return static_cast<float>(m_amount.pixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
}


void RectSubdivision::setPixels(int newPixels)
{
    m_amount.pixels = newPixels;
    m_amountType = AmountType::Pixels;
}

void RectSubdivision::setRatio(float newRatio)
{
    m_amount.ratio = newRatio;
    m_amountType = AmountType::Ratio;
}

void RectSubdivision::moveByPixels(const ls::Rectangle2I& rect, int deltaPixels)
{
    if (m_amountType == AmountType::Pixels) m_amount.pixels += deltaPixels;
    else
    {
        float deltaRatio = static_cast<float>(deltaPixels) / (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
        if (m_subject == Subject::Second) deltaRatio = -deltaRatio;

        m_amount.ratio += deltaRatio;
    }
}
void RectSubdivision::moveByRatio(const ls::Rectangle2I& rect, float deltaRatio)
{
    if (m_amountType == AmountType::Ratio) m_amount.ratio += deltaRatio;
    else
    {
        float deltaPixels = deltaRatio * (m_orientation == Orientation::Horizontal ? rect.width() : rect.height());
        if (m_subject == Subject::Second) deltaPixels = -deltaPixels;

        m_amount.pixels = deltaPixels;
    }
}

RectSubdivision::RectSubdivision(RectSubdivision::Orientation orientation, RectSubdivision::Subject subject, int pixels) :
    m_orientation(orientation),
    m_amountType(AmountType::Pixels),
    m_subject(subject),
    m_aspectRatioFirst(std::nullopt),
    m_aspectRatioSecond(std::nullopt)
{
    m_amount.pixels = pixels;
}
RectSubdivision::RectSubdivision(RectSubdivision::Orientation orientation, RectSubdivision::Subject subject, float ratio) :
    m_orientation(orientation),
    m_amountType(AmountType::Ratio),
    m_subject(subject),
    m_aspectRatioFirst(std::nullopt),
    m_aspectRatioSecond(std::nullopt)
{
    m_amount.ratio = ratio;
}