#pragma once

#include <optional>

#include "AspectRatio.h"

class RectSubdivision
{
public:
    enum class Orientation
    {
        Horizontal,
        Vertical
    };
    enum class Subject
    {
        First,
        Second
    };

private:
    enum class AmountType
    {
        Pixels,
        Ratio
    };
    union Amount
    {
        int pixels;
        float ratio;
    };

    Amount m_amount;
    AmountType m_amountType;
    Subject m_subject;
    Orientation m_orientation;
    std::optional<AspectRatio> m_aspectRatioFirst;
    std::optional<AspectRatio> m_aspectRatioSecond;

public:

    static RectSubdivision withPixels(Orientation orientation, Subject subject, int pixels);
    static RectSubdivision withRatio(Orientation orientation, Subject subject, float ratio);

    RectSubdivision& withAspectRatio(AspectRatio ratio, Subject subject);

    RectSubdivision(const RectSubdivision& other) = default;
    RectSubdivision(RectSubdivision&& other) = default;

    RectSubdivision& operator=(const RectSubdivision& other) = default;
    RectSubdivision& operator=(RectSubdivision&& other) = default;

    void setPixels(int newPixels);
    void setRatio(float newRatio);

    std::pair<ls::Rectangle2I, ls::Rectangle2I> calculateSubRects(const ls::Rectangle2I& rect) const;

    void moveByPixels(const ls::Rectangle2I& rect, int deltaPixels);
    void moveByRatio(const ls::Rectangle2I& rect, float deltaRatio);

    int asPixels(const ls::Rectangle2I& rect) const;
    float asRatio(const ls::Rectangle2I& rect) const;

private:
    RectSubdivision(Orientation orientation, Subject subject, int pixels);
    RectSubdivision(Orientation orientation, Subject subject, float ratio);
};