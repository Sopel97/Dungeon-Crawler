#pragma once

#include <vector>
#include <optional>

#include <SFML/Graphics/Color.hpp>

#include "Light.h"

#include "../LibS/Shapes.h"

template <class Float>
struct DefaultLightClipperConstParams
{
    static constexpr Float spaceBetweenNeighbourRays = Float(0.2);
    static constexpr Float maxRayInaccuracy = Float(5.0);
    static constexpr Float minPointDistance = Float(2.0);
    static constexpr Float sweepMargin = Float(0.001);
};

template <class Float, class ConstParams = DefaultLightClipperConstParams<Float>>
class LightClipper
{
    using Vec2 = ls::Vec2<Float>;
    using Rectangle2 = ls::Rectangle2<Float>;

public:
    LightClipper(const Light& light, std::vector<Rectangle2> geom) :
        m_lightOrigin(light.position()),
        m_lightBounds(light.bounds()),
        m_lightColor(light.color()),
        m_geometry(std::move(geom))
    {
        update();
    }

    const std::vector<Vec2>& litAreaPolygon() const
    {
        return m_litAreaPolygon;
    }

private:
    struct Vector
    {
        Vec2 position;
        Float angle;

        Vector(const Vec2& pos) :
            position(pos),
            angle(pseudoAngle(pos))
        {

        }
    };

    struct Segment
    {
        Vector begin;
        Vector end;

        Segment(const Vec2& a, const Vec2& b) :
            begin(a),
            end(b)
        {
        }
    };

    struct Ray
    {
        Vec2 direction;
        Float expectedDist;
        Float angle;

        Ray(const Vec2& dir) :
            direction(dir.normalized()),
            expectedDist(dir.magnitude()),
            angle(pseudoAngle(dir))
        {

        }
    };

    Vec2 m_lightOrigin;
    Rectangle2 m_lightBounds;
    sf::Color m_lightColor;
    // stored translated such that ray origin is (0, 0)
    std::vector<Rectangle2> m_geometry;

    // following stored with local coordinates for easier computation, 
    // origin (0, 0) is defined by m_lightOrigin
    std::vector<Segment> m_segments;
    std::vector<Ray> m_rays;

    std::vector<Vec2> m_litAreaPolygon;

    using SegmentIterator = typename std::vector<Segment>::iterator;

    static Float pseudoAngle(Vec2 v)
    {
        static constexpr Float smallestAngle = -1000.0;
        using std::copysign;
        using std::abs;

        if (v.y == Float(0.0) && v.x <= Float(0.0)) return smallestAngle;
        return copysign(Float(1.0) - v.x / (abs(v.x) + abs(v.y)), v.y);
    }

    void addSegment(const Vec2& a, const Vec2& b)
    {
        static constexpr Float eps = Float(0.001);

        // 0.001 because parallel lines cause some trouble
        if (a.y * b.y < eps) // may pass angle discontinuity
        {
            // order them correctly so we know how to generate sub segments if needed
            if (a.y > b.y) std::swap(a, b);
            const Float t = -a.y / (b.y - a.y);
            const Float offCrossX = a.x + (b.x - a.x) * t;
            if (offCrossX < 0.0) // does pass through angle discontinuity, split
            {
                //pseudoangle must guarantee that this vertex (y == 0.0f) gets smallest angle possible, otherwise sometimes segments are missed
                const Vec2 crossPoint = Vec2(offCrossX, 0.0);

                // they can't share a vertex because then angles break optimization
                // when vertices are shared angle span of one of the segments becomes >180 degrees 
                // (from 0.0f to some number on the opposite quadrant, but the angle is directed)
                // therefore we need to order the points so we know in what order to generate sub segments
                // so we can offset one of them correctly
                m_segments.emplace_back(a, Vec2(offCrossX, 0.0));
                m_segments.emplace_back(Vec2(offCrossX, eps), b);

                return;
            }
        }

        m_segments.emplace_back(a, b);
    }

    void trimAndTranslateRectangles()
    {
        for (auto& rect : m_geometry)
        {
            rect.min.x = max(rect.min.x, m_lightBounds.min.x) - m_lightOrigin.x;
            rect.min.y = max(rect.min.y, m_lightBounds.min.y) - m_lightOrigin.y;
            rect.max.x = min(rect.max.x, m_lightBounds.max.x) - m_lightOrigin.x;
            rect.max.y = min(rect.max.y, m_lightBounds.max.y) - m_lightOrigin.y;
        }
    }

    void addSegmentsFromRectangle(const Rectangle2& rect)
    {
        using std::min;
        using std::max;

        const Float left = rect.min.x;
        const Float top = rect.min.y;
        const Float right = rect.max.x;
        const Float bottom = rect.max.y;

        if (left >= Float(0.0))
        {
            addSegment(Vec2(left, top), Vec2(left, bottom));
        }

        if (top >= Float(0.0))
        {
            addSegment(Vec2(left, top), Vec2(right, top));
        }

        if (right <= Float(0.0))
        {
            addSegment(Vec2(right, top), Vec2(right, bottom));
        }

        if (bottom <= Float(0.0))
        {
            addSegment(Vec2(left, bottom), Vec2(right, bottom));
        }
    }

    void addSegmentsForBounds()
    {
        const Float left = m_lightBounds.min.x;
        const Float top = m_lightBounds.min.y;
        const Float right = m_lightBounds.max.x;
        const Float bottom = m_lightBounds.max.y;

        addSegment(Vec2(left, top), Vec2(left, bottom));
        addSegment(Vec2(left, top), Vec2(right, top));
        addSegment(Vec2(right, top), Vec2(right, bottom));
        addSegment(Vec2(left, bottom), Vec2(right, bottom));
    }

    void addRay(const Vec2& through)
    {
        m_rays.emplace_back(through);
    }

    void addSplitRay(const Vec2& through)
    {
        const Vec2 offset = through;
        const Vec2 sideOffset = offset.normal() * ConstParams::spaceBetweenNeighbourRays;
        // addRay(through); not needed for approximate result
        addRay(through + sideOffset);
        addRay(through - sideOffset);
    }

    void addSplitRaysForRectangleVertices(const Rectangle2& rect)
    {
        using std::min;
        using std::max;

        const Float left = rect.min.x;
        const Float top = rect.min.y;
        const Float right = rect.max.x;
        const Float bottom = rect.max.y;

        bool topLeftVisible = false;
        bool topRightVisible = false;
        bool bottomLeftVisible = false;
        bool bottomRightVisible = false;

        if (left >= Float(0.0))
        {
            topLeftVisible = true;
            bottomLeftVisible = true;
        }

        if (top >= Float(0.0))
        {
            topLeftVisible = true;
            topRightVisible = true;
        }

        if (right <= Float(0.0))
        {
            topRightVisible = true;
            bottomRightVisible = true;
        }

        if (bottom <= Float(0.0))
        {
            bottomLeftVisible = true;
            bottomRightVisible = true;
        }

        if (topLeftVisible) addSplitRay(Vec2(left, top));
        if (topRightVisible) addSplitRay(Vec2(right, top));
        if (bottomLeftVisible) addSplitRay(Vec2(left, bottom));
        if (bottomRightVisible) addSplitRay(Vec2(right, bottom));
    }

    void addRaysForBounds()
    {
        const Float left = m_lightBounds.min.x;
        const Float top = m_lightBounds.min.y;
        const Float right = m_lightBounds.max.x;
        const Float bottom = m_lightBounds.max.y;

        addRay(Vec2(left, top));
        addRay(Vec2(right, top));
        addRay(Vec2(left, bottom));
        addRay(Vec2(right, bottom));
    }

    void orderRaysByAngle()
    {
        std::sort(
            m_rays.begin(),
            m_rays.end(),
            [](const Ray& lhs, const Ray& rhs) -> bool
        {
            return lhs.angle < rhs.angle;
        }
        );
    }

    void orderSegmentsByAngle()
    {
        // order vertices inside segments by angle
        for (auto& segment : m_segments)
        {
            if (segment.end.angle < segment.begin.angle)
                std::swap(segment.begin, segment.end);
        }

        // sort segments by angle of first vertex
        std::sort(
            m_segments.begin(),
            m_segments.end(),
            [](const Segment& lhs, const Segment& rhs) -> bool
        {
            return lhs.begin.angle < rhs.begin.angle;
        }
        );
    }

    static std::optional<Vec2> intersectionPoint(const Ray& ray, const Segment& segment)
    {
        static constexpr Float inclusionMargin = Float(0.001);

        const Vec2 segmentVector = segment.end.position - segment.begin.position;

        const Float rdx = ray.direction.x;
        const Float rdy = ray.direction.y;
        const Float sbx = segment.begin.position.x;
        const Float sby = segment.begin.position.y;
        const Float sdx = segmentVector.x;
        const Float sdy = segmentVector.y;

        const Float t2 = (rdx*sby - rdy*sbx) / (sdx*rdy - sdy*rdx);
        if (t2 < -inclusionMargin || t2 > Float(1.0) + inclusionMargin) return {};
        const Float t1 = (sbx + sdx*t2) / rdx;
        if (t1 < -inclusionMargin) return {};

        return ray.direction * t1;
    }

    void maybeAddInnerPolygonVertex(const Ray& ray, const Vec2& vertexLocal)
    {
        // discard the vertex if it was made by a ray that
        // was supposed to hit a vertex much further
        // only vertices made by rays that hit exactly what they should or something further are needed
        // this is only approximation since we cast rays with an offset
        // but is good enough

        // TODO: maybe remove colinear points
        if (vertexLocal.magnitude() + ConstParams::maxRayInaccuracy > ray.expectedDist)
        {
            const Vec2 vertex = vertexLocal + m_lightOrigin;

            if (!m_litAreaPolygon.empty())
            {
                const Vec2 prevVertex = m_litAreaPolygon.back();

                const Vec2 diff = vertex - prevVertex;

                // dont add if vertex is too close to the one before
                if (std::abs(diff.x) > ConstParams::minPointDistance || std::abs(diff.y) > ConstParams::minPointDistance)
                {
                    m_litAreaPolygon.emplace_back(vertex);
                }
            }
            else
            {
                m_litAreaPolygon.emplace_back(vertex);
            }
        }
    }

    static std::optional<Vec2> closestIntersectionPoint(const Ray& ray, SegmentIterator begin, SegmentIterator end)
    {
        Vec2 closestPoint;
        Float closestDistSqr = std::numeric_limits<Float>::max();

        for (; begin != end; ++begin)
        {
            const Segment& segment = *begin;

            if (auto collisionPointOpt = intersectionPoint(ray, segment))
            {
                const Vec2& collisionPoint = collisionPointOpt.value();

                const Float distSqr = collisionPoint.magnitudeSquared();
                if (distSqr < closestDistSqr)
                {
                    closestPoint = collisionPoint;
                    closestDistSqr = distSqr;
                }
            }
        }

        if (closestDistSqr != std::numeric_limits<Float>::max()) return closestPoint;

        return {};
    }

    static Vec2 closestPoint(const Vec2& point, const Vec2& a, const Vec2& b)
    {
        const Vec2 segmentVector = b - a;
        const Float t = (point - a).dot(segmentVector) / segmentVector.magnitudeSquared();

        Vec2 closestPointOnSegment;
        if (t >= 1.0) closestPointOnSegment = b;
        else if (t <= 0.0) closestPointOnSegment = a;
        else closestPointOnSegment = a + t*segmentVector;

        return closestPointOnSegment;
    }

    void update()
    {
        trimAndTranslateRectangles();

        // make segment list
        m_segments.reserve(m_geometry.size() * 2 + 4); // 4 for bounds

                                                 // bounds
        addSegmentsForBounds();

        // add rectangles
        for (const auto& rect : m_geometry)
        {
            addSegmentsFromRectangle(rect);
        }

        // add rays from initial vertices (don't add for split ones)
        // also stored with local coordinates (all rays have origin in (0, 0))
        m_rays.reserve(m_geometry.size() * 6 + 4); // 4 for bounds

                                             // bounds
        addRaysForBounds();

        // rectangles
        for (const auto& rect : m_geometry)
        {
            addSplitRaysForRectangleVertices(rect);
        }

        // order rays and segments
        // needed for sweepline approach
        orderRaysByAngle();
        orderSegmentsByAngle();

        auto segmentIterator = m_segments.begin();
        auto rayIterator = m_rays.begin();

        auto firstSegment = m_segments.begin();

        // step through segments in order
        // keep current segments
        // if ray is before next segment then 
        //   discard segments that end before that ray
        //   set all current segments as (possibly, due to float imprecision) colliding with that ray
        //   compute collision point(s), choose the closest
        // else add the segment to list of current
        for (; rayIterator != m_rays.end();)
        {
            const Ray& ray = *rayIterator;
            bool rayCloser = true;

            if (segmentIterator != m_segments.end())
            {
                const Segment& segment = *segmentIterator;

                rayCloser = ray.angle + ConstParams::sweepMargin < segment.begin.angle;
            }

            if (rayCloser)
            {
                // remove segments that don't intersect ray's angle (end before the ray)

                while (firstSegment != segmentIterator && firstSegment->end.angle + ConstParams::sweepMargin < ray.angle)
                {
                    ++firstSegment;
                }

                // find closest intersection point
                if (auto closestPointOpt = closestIntersectionPoint(ray, firstSegment, segmentIterator))
                {
                    const auto& closestPoint = closestPointOpt.value();
                    maybeAddInnerPolygonVertex(ray, closestPoint);
                }

                ++rayIterator;
            }
            else
            {
                // just add the segment
                ++segmentIterator;
            }
        }
    }
};