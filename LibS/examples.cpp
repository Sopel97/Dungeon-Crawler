#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "GeometryLight.h"
#include "Noise.h"
#include "Random.h"
#include "Util.h"
#include <cstdlib>

#define PrettyPrinterTest
#include "PrettyPrinter.h"

using namespace std;

template <class T>
class InteractiveShape
{
public:
    T shape;
    ALLEGRO_COLOR color;
    InteractiveShape()
    {

    }
    InteractiveShape(const T& s)
    {
        shape = s;
        color = al_map_rgb(0, 255, 0);
    }
    void draw()
    {
        Geo::Polyline<typename T::ValueType> outline = shape.asPolyline();
        int outlineSize = outline.size();
        ALLEGRO_VERTEX* vertices = new ALLEGRO_VERTEX[outlineSize + 1];
        int j = 0;
        for(int i = 0; i < outlineSize; ++i)
        {
            vertices[j].x = outline.vertices[i].x;
            vertices[j].y = outline.vertices[i].y;
            vertices[j].color = color;

            j += 1;

            /* does not work LOL
            vertices[j].x = outline.elements[i].begin.x;
            vertices[j].y = outline.elements[i].begin.y;
            vertices[j].color = color;
            ++j;
            vertices[j].x = outline.elements[i].end.x;
            vertices[j].y = outline.elements[i].end.y;
            vertices[j].color = color;
            */
        }
        int i = 0;
        vertices[j].x = outline.vertices[i].x;
        vertices[j].y = outline.vertices[i].y;
        vertices[j].color = color;

        al_draw_prim(vertices, NULL, NULL, 0, outlineSize + 1, ALLEGRO_PRIM_LINE_STRIP);
    }
    bool move(ALLEGRO_MOUSE_STATE& before, ALLEGRO_MOUSE_STATE& after)
    {
        bool moved = false;
        Geo::Vec2D posBefore = {(double)before.x, (double)before.y};
        Geo::Vec2D posAfter = {(double)after.x, (double)after.y};
        if(before.buttons & 1)
        {
            if(shape.intersects(posBefore))
            {
                shape += posAfter - posBefore;
                moved = true;
            }
        }
        return moved;
    }
    void setColor(ALLEGRO_COLOR c)
    {
        color = c;
    }
    ~InteractiveShape()
    {

    }
};
class RigidBody
{
public:
    PolygonD polygon;
    Vec2D centerOfMass;
    double distanceToFurthestPoint;
    RigidBody(const PolygonD& poly) : polygon(poly)
    {
        centerOfMass = poly.center();
        distanceToFurthestPoint = 0.0;
        for(const auto& v : poly.vertices)
        {
            double mag = (centerOfMass - v).magnitude();
            if(mag > distanceToFurthestPoint)
            {
                distanceToFurthestPoint = mag;
            }
        }
    }
    void dragDrop(const Vec2D& start, const Vec2D& end)
    {
        Vec2D centroidToStart = start - centerOfMass; //these two won't change later, they are the initial parameters which will be used to maintain distance ratios etc.
        double distanceFromCentroidToStart = centroidToStart.magnitude();

        Vec2D centroidToEnd = end - centerOfMass;

        Vec2D translation = (end - start) * (1.0 - (distanceFromCentroidToStart / distanceToFurthestPoint)); //first translation. The closer the start point to the center of mass the more it will be translated to make rotation (applied later) smaller
        centerOfMass += translation;
        polygon += translation;

        centroidToEnd = end - centerOfMass; //this has to be racalculated after every operation on centerOfMass

        AngleD angle = centroidToEnd.angle(centroidToStart); //angle defining rotation made by mouse in centerOfMass's frame of reference

        AffineTransformationD transformation;  //rotation of points
        transformation.translate(centerOfMass); //3
        transformation.rotateAntiClockwise(angle); //2
        transformation.translate(centerOfMass * -1.0); //1
        transformation.transform(centerOfMass);
        for(auto& v : polygon.vertices) transformation.transform(v);

        centroidToEnd = end - centerOfMass; //again, racalculation

        translation = (end - centroidToEnd.normalized() * distanceFromCentroidToStart) - centerOfMass; //final translation. This one is to make centroidToStart == centroidToEnd
        centerOfMass += translation;
        polygon += translation;

        //this will result in object being moved and rotated (to mimic dragging in real world) preserving anchor point (mouse will sty in the same point in polygon)
    }
};


void next(int counter, const RayD& ray, const Mesh2<CircleD>& mesh, const std::vector<RigidBody>& bodies)
{
    if(counter < 0) return; //max recursion depth
    ALLEGRO_COLOR rayColor = al_map_rgb(255, 0, 0);
    double tmin = 111111;
    RaycastD::Hit closestHit;
    bool anyHit = false;
    for(const auto& circle : mesh.elements)
    {
        auto t = Raycaster::raycast(ray, circle);
        for(const auto& hit : t.hits)
        {
            if(hit.distance < tmin)
            {
                tmin = hit.distance;
                closestHit = hit;
                anyHit = true;
            }
        }
    }
    for(const auto& body : bodies)
    {
        const PolygonD& poly = body.polygon;
        auto t = Raycaster::raycast(ray, poly);
        for(const auto& hit : t.hits)
        {
            if(hit.distance < tmin)
            {
                tmin = hit.distance;
                closestHit = hit;
                anyHit = true;
            }
        }
    }
    al_draw_line(ray.origin.x, ray.origin.y, ray.origin.x + ray.direction.x * tmin, ray.origin.y + ray.direction.y * tmin, rayColor, 1.0);
    if(anyHit)
    {
        Vec2D n = closestHit.normal;
        Vec2D directionAfterReflection = ray.direction - n * 2.0 * ray.direction.dot(n);
        next(counter - 1, RayD(closestHit.hitPoint - ray.direction * 0.001, directionAfterReflection), mesh, bodies);
    }
}
template <class T>
void draw(const Vec2<T>& point, const ALLEGRO_COLOR& color)
{
    al_draw_pixel(point.x, point.y, color);
}
template <class T>
void draw(const Circle<T>& circle, const ALLEGRO_COLOR& color)
{
    al_draw_circle(circle.origin.x, circle.origin.y, circle.radius, color);
}
template <class T>
void drawFilled(const Circle<T>& circle, const ALLEGRO_COLOR& color)
{
    al_draw_filled_circle(circle.origin.x, circle.origin.y, circle.radius, color);
}
template <class T>
void draw(const Geo::Rectangle<T>& rect, const ALLEGRO_COLOR& color)
{
    al_draw_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color, 1);
}
template <class T>
void drawFilled(const Geo::Rectangle<T>& rect, const ALLEGRO_COLOR& color)
{
    al_draw_filled_rectangle(rect.min.x, rect.min.y, rect.max.x, rect.max.y, color);
}
template <class T>
void draw(const LineSegment<T>& line, const ALLEGRO_COLOR& color)
{
    al_draw_line(line.begin.x, line.begin.y, line.end.x, line.end.y, color, 1);
}
template <class T>
void draw(const Ray<T>& ray, const ALLEGRO_COLOR& color)
{
    al_draw_line(ray.origin.x, ray.origin.y, ray.origin.x + ray.direction.x * 10000.0f, ray.origin.y + ray.direction.y * 10000.0f, color, 1);
}
template <class T>
void draw(const Triangle<T>& triangle, const ALLEGRO_COLOR& color)
{
    al_draw_triangle(triangle.vertices[0].x, triangle.vertices[0].y,
                     triangle.vertices[1].x, triangle.vertices[1].y,
                     triangle.vertices[2].x, triangle.vertices[2].y,
                     color, 1);
}
template <class T>
void drawFilled(const Triangle<T>& triangle, const ALLEGRO_COLOR& color)
{
    al_draw_filled_triangle(triangle.vertices[0].x, triangle.vertices[0].y,
                            triangle.vertices[1].x, triangle.vertices[1].y,
                            triangle.vertices[2].x, triangle.vertices[2].y,
                            color);
}
template <class T>
void draw(const Geo::Polygon<T>& polygon, const ALLEGRO_COLOR& color)
{
    std::vector<ALLEGRO_VERTEX> vertices;
    size_t size = polygon.size();
    if(size < 3) return;
    vertices.reserve(size + 1);
    for(size_t i = 0; i <= size; ++i)
    {
        const Vec2<T>& v = polygon.vertices[i % size];
        vertices.push_back(ALLEGRO_VERTEX {static_cast<float>(v.x), static_cast<float>(v.y), 0.0f, 0.0f, 0.0f, color});
    }

    al_draw_prim(vertices.data(), nullptr, nullptr, 0, size + 1, ALLEGRO_PRIM_LINE_STRIP);
}
template <class T>
void draw(const Geo::Polyline<T>& polyline, const ALLEGRO_COLOR& color)
{
    std::vector<ALLEGRO_VERTEX> vertices;
    size_t size = polyline.size();
    vertices.reserve(size);
    for(size_t i = 0; i < size; ++i)
    {
        const Vec2<T>& v = polyline[i];
        vertices.push_back(ALLEGRO_VERTEX {static_cast<float>(v.x), static_cast<float>(v.y), 0.0f, 0.0f, 0.0f, color});
    }

    al_draw_prim(vertices.data(), nullptr, nullptr, 0, size, ALLEGRO_PRIM_LINE_STRIP);
}
template <class ShapeType>
void draw(const Mesh2<ShapeType>& mesh, const ALLEGRO_COLOR& color)
{
    for(const auto& shape : mesh.elements)
        draw(shape, color);
}
template <class ShapeType>
void draw(const std::vector<ShapeType>& mesh, const ALLEGRO_COLOR& color)
{
    for(const auto& shape : mesh)
        draw(shape, color);
}

void delaunayVoronoiExamples()
{
    //may generate wrong polygons close to boundary because polygons that stretch to infinity are not handled yet
    constexpr size_t numberOfPoints = 100u;
    Random::Xorshift64Engine randomEngine;
    RectangleD boundingRect(Vec2D {100.0, 100.0}, Vec2D {1200.0, 700.0});

    std::vector<Vec2D> points;
    points.reserve(numberOfPoints);

    for(size_t i = 0; i < numberOfPoints; ++i)
    {
        points.push_back(boundingRect.pickRandomPoint(randomEngine));
    }

    PointSetDelaunayTriangulationD triangulation(points);
    VoronoiDiagramD voronoi(triangulation);
    std::cout << "Number of generated triangles: " << triangulation.triangleMesh().size() << '\n';
    std::cout << "Number of generated polygons: " << voronoi.polygons().size() << '\n';

    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;

        al_clear_to_color(al_map_rgb(0, 0, 0));

        draw(triangulation.triangleMesh(), al_map_rgb(200, 0, 0));
        draw(voronoi.polygons(), al_map_rgb(0, 255, 0));

        al_flip_display();
    }
}

void cellularAutomatonCaveGenerationExample()
{
    //white floor, black wall
    constexpr size_t cellSize = 10u;
    constexpr size_t width = 100u;
    constexpr size_t height = 70u;
    constexpr size_t padding = 10u;

    CellularAutomaton<QuantityRules<OriginalCellularAutomatonStates>> ca(
                QuantityRules<OriginalCellularAutomatonStates>(
                    OriginalCellularAutomatonStates::Black,
                    std::array<OriginalCellularAutomatonStates, 10u>
    {
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black
    }),
    width, height);
    ca.fill([width, height](size_t x, size_t y) -> OriginalCellularAutomatonStates
    {
        static Random::Xorshift64Engine randomEngine;
        if(width - x <= 2 || x <= 1 || height - y <= 2 || y <= 1) return OriginalCellularAutomatonStates::Black;
        return (randomEngine.nextDouble() < 0.50 ? OriginalCellularAutomatonStates::Black : OriginalCellularAutomatonStates::White);
    });
    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;
        if(al_key_down(&keyboardState, ALLEGRO_KEY_UP))
        {
            ca.iterate();
            al_rest(0.1f);
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == OriginalCellularAutomatonStates::Black ? al_map_rgb(64, 64, 64) : al_map_rgb(255, 255, 255));
            }
        }
        al_flip_display();
    }
}
void conwaysGameOfLifeExample()
{
    //white floor, black wall
    constexpr size_t cellSize = 10u;
    constexpr size_t width = 100u;
    constexpr size_t height = 70u;
    constexpr size_t padding = 10u;

    CellularAutomaton<ConwaysGameOfLifeRules> ca(ConwaysGameOfLifeRules(), width, height, ConwaysGameOfLifeRules::States::Dead, CellularAutomatonGridTopology::Toroidal);

    constexpr auto D = ConwaysGameOfLifeRules::States::Dead;
    constexpr auto L = ConwaysGameOfLifeRules::States::Live;

    std::vector<Array2<ConwaysGameOfLifeRules::States>> patterns(
    {
        {
            {D, D, L, D},
            {L, D, D, L},
            {L, D, D, L},
            {D, L, D, D}
        },

        {
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {L, L, L, D, D, L, L, D, L, L, D, D, L, L, L},
            {D, D, L, D, L, D, L, D, L, D, L, D, L, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, L, D, L, D, L, D, L, D, L, D, L, D, D},
            {L, L, L, D, D, L, L, D, L, L, D, D, L, L, L},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, L, L, D, D, D, L, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D},
            {D, D, D, D, L, D, D, D, D, D, L, D, D, D, D}
        },
        {
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, L, D, D, D, L, L},
            {D, D, D, D, L, L, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, L, L, D, L, D, D, D, D, L, L},
            {L, L, D, L, D, D, L, D, D, L, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D},
            {L, L, D, D, L, L, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, L, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, D, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D},
            {D, D, D, D, D, D, D, D, D, L, L, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D, D}
        }
    });

    constexpr size_t selectedPattern = 2u;

    size_t patternWidth = patterns[selectedPattern].sizeX();
    size_t patternHeight = patterns[selectedPattern].sizeY();

    size_t patternOriginX = width / 2 - patternWidth / 2;
    size_t patternOriginY = height / 2 - patternHeight / 2;


    for(size_t x = 0u; x < patternWidth; ++x)
    {
        for(size_t y = 0u; y < patternHeight; ++y)
        {
            ca.setCell(x + patternOriginX, y + patternOriginY, patterns[selectedPattern](x, y));
        }
    }

    ALLEGRO_KEYBOARD_STATE keyboardState;
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;
        if(al_key_down(&keyboardState, ALLEGRO_KEY_UP))
        {
            ca.iterate();
            al_rest(0.05f);
        }

        al_clear_to_color(al_map_rgb(0, 0, 0));

        for(size_t x = 0u; x < width; ++x)
        {
            for(size_t y = 0u; y < height; ++y)
            {
                drawFilled(
                    RectangleF(
                        Vec2F(padding + x * cellSize, padding + y * cellSize),
                        10.0f,
                        10.0f),
                    ca.cellAt(x, y) == ConwaysGameOfLifeRules::States::Live ? al_map_rgb(0, 200, 0) : al_map_rgb(255, 255, 255));
            }
        }
        al_flip_display();
    }
}
int main()
{
    //Vec2 v1(2.0f, 0.0f);
    //Vec2 v2(0.0f, 0.0f);
    //Vec2 v3;
    //Vec2 v4;
    //Physics::recalculateVelocityAfterCollision(v1, v2, Vec2(0.1f, 1.0f), 1.0f, 1.0f, 0.1f);
    //Physics::recalculateVelocityAfterCollision(v1, v2, Vec2(1.0f, 0.0f), v3, v4, 1.0f, 1.0f, 0.5f);

    //cout << fixed << v3.x << ' ' << v3.y << '\n';
    //cout <<          v4.x << ' ' << v4.y;
    /*Geo::Vec2 a(-3, 9);
    Geo::Vec2 b(6, -3);
    Geo::Vec2 c(0, -2);
    Geo::Vec2 d(5, 3);
    Geo::Vec2 p;
    Geo::LineSegment s1(a, b);
    Geo::LineSegment s2(c, d);
    cout << s1.intersects(s2, p) << '\n';
    cout << p.x << ' ' << p.y;
    cout << '\n';
    Geo::Rectangle r({1,1},{2,2});
    Geo::Mesh<Geo::Rectangle> rectangleMesh(std::vector<Geo::Rectangle>{Geo::Rectangle({1,1},{2,2}),Geo::Rectangle({3,3},{4,4})});
    rectangleMesh += Geo::Vec2(2,2);
    cout << rectangleMesh.elements[1].min.x;
    cout << '\n';
    cout << rectangleMesh.intersects(rectangleMesh);
    //Geo::Shape* shape;
    //shape->intersects(rectangleMesh);
    //rectangleMesh.intersects(shape);
    cout << "\n\n\n\n";
    Geo::Mesh<Geo::LineSegment> outline = rectangleMesh.outline();
    cout << outline.elements[7].begin.x;
    //cout << a.intersects(r);*/

    al_init();
    al_init_primitives_addon();
    al_install_mouse();
    al_install_keyboard();
    al_create_display(1280, 800);

    //delaunayVoronoiExample();
    //cellularAutomatonCaveGenerationExample();
    conwaysGameOfLifeExample();

    return 0;
    //old test below

    //ALLEGRO_COLOR colliding = al_map_rgb(255, 0, 0);
    ALLEGRO_COLOR notcolliding = al_map_rgb(0, 255, 0);
    Matrix<int, 3, 3> m;
    /*
    const int shapeSize = 4;
    const int shape2Size = 4;
    InteractiveShape<Geo::Vec2D> shapes[shapeSize] = {
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(-100+200, 0+200)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(100+200, 300+200)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(300+200, 0+200)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(100+200, -200+200))
                                                     };


    InteractiveShape<Geo::Vec2D> shapes2[shapeSize] = {
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(-100+500, 0+500)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(100+500, 300+500)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(300+500, 0+500)),
                                                        InteractiveShape<Geo::Vec2D>(Geo::Vec2D(100+500, -200+500))
                                                     };

    ALLEGRO_MOUSE_STATE now;
    al_get_mouse_state(&now);
    ALLEGRO_MOUSE_STATE prev;
    prev = now;
    for(;;)
    {
        al_get_mouse_state(&now);
        for(int i = 0; i < shapeSize; ++i) shapes[i].move(prev, now);
        for(int i = 0; i < shape2Size; ++i) shapes2[i].move(prev, now);
        al_clear_to_color(al_map_rgb(0, 0, 0));

        Geo::Vec2D v[shapeSize];
        for(int i = 0; i < shapeSize; ++i)
        {
            v[i] = shapes[i].shape;
        }
        auto poly = Geo::PolygonD(v, shapeSize);
        InteractiveShape<Geo::PolygonD> p(poly);


        Geo::Vec2D v2[shape2Size];
        for(int i = 0; i < shape2Size; ++i)
        {
            v2[i] = shapes2[i].shape;
        }
        auto poly2 = Geo::PolygonD(v2, shape2Size);
        InteractiveShape<Geo::PolygonD> p2(poly2);
        bool intersection = poly.intersects(poly2);
        if(intersection)
        {
            p.color = colliding;
            p2.color = colliding;
        }
        else
        {
            p.color = notcolliding;
            p2.color = notcolliding;
        }
        p.draw();
        p2.draw();
       // Geo::Vec2D c = center(poly);
        //al_draw_pixel(c.x, c.y, shapes[0].color);
        //al_draw_pixel(shape2.shape.x, shape2.shape.y, shape2.color);
        al_flip_display();
        prev = now;
    }



    */
    const int shapeSize = 133;
    std::vector<InteractiveShape<Geo::Vec2D>> points;
    Mesh2<CircleD> c {{{200.0, 200.0}, 150.0}, {{200.0, 300.0}, 50.0}, {{200.0, 400.0}, 10.0}, {{200.0, 500.0}, 90.0}};
    RayD r {{500.0, 500.0}, {300, 400}};
    //Random::Xorshift32Engine engine;
    //Random::Xorshift64Engine engine;
    //Random::Xorshift128Engine engine;
    //Random::Xorshift1024Engine engine;
    Random::CMWCEngine engine;
    //Random::WELL512Engine engine;
    //Random::WELL1024Engine engine;

    AffineTransformationD transformation = AffineTransformationD::fromTriangleToTriangleMapping(TriangleD({{0.0, 0.0}, {0.0, 1.0}, {1.0, 0.5}}),
    TriangleD({{0.2, 0.0}, {0.0, 1.0}, {1.4, 1.0}}));
    // 4th
    // 3rd
    // 2nd
    // 1st
    //transformation.translate(Vec2D{100, 100});
    //transformation.rotateClockwiseByDegrees(30);
    //transformation.translate(Vec2D{-100, -100});
    //transformation.shearInYDirection(0.4);
    //transformation.shearInXDirection(0.4);
    for(int i = 0; i < shapeSize; ++i)
    {
        points.push_back(transformation.transformed(Geo::Vec2D(engine.next(100, 700), engine.next(100, 700))));
    }
    std::vector<InteractiveShape<Geo::CircleD>> circles;
    for(auto& circle : c.elements) circles.emplace_back(circle);
    InteractiveShape<Geo::Vec2D> v2(r.origin);

    PrettyPrinter::test();

    ALLEGRO_MOUSE_STATE now;
    al_get_mouse_state(&now);
    ALLEGRO_MOUSE_STATE prev;
    prev = now;

    ALLEGRO_KEYBOARD_STATE keyboardState;

    std::vector<InteractiveShape<Vec2D>> curvePoints;
    BezierPathD path;
    int numberOfControlPoints = 29;
    for(int i = 0; i < numberOfControlPoints; ++i)
    {
        Vec2D point(rand() % 1000, rand() % 700);
        curvePoints.push_back(InteractiveShape<Vec2D>(point));
        path.add(point);
        std::cout << point.x << ' ' << point.y << '\n';
    }
    auto pointsOnPath = path.evaluateAll(0.001);
    std::vector<ALLEGRO_VERTEX> pathVertexData;
    for(const auto& p : pointsOnPath)
    {
        pathVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(p.x), static_cast<float>(p.y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
    }
    std::vector<RigidBody> bodies;
    bodies.emplace_back(PolygonD({Vec2D{100, 100}, Vec2D{200, 100}, Vec2D{300, 200}, Vec2D{200, 400}, Vec2D{50, 190}}));
    bodies.emplace_back(PolygonD({Vec2D{500, 100}, Vec2D{550, 100}, Vec2D{550, 700}, Vec2D{500, 700}}));
    bodies.push_back(PolygonD::fromTriangle(TriangleD::isosceles(Vec2D {400, 400}, 100, 300)));
    {
        PolygonD poly;
        double r = 300;
        for(int i = 0; i <= 135; ++i)
        {
            poly.vertices.emplace_back(std::cos(degreesToRadians(i))*r, std::sin(degreesToRadians(i))*r);
        }
        r = 290;
        for(int i = 135; i >= 0; --i)
        {
            poly.vertices.emplace_back(std::cos(degreesToRadians(i))*r, std::sin(degreesToRadians(i))*r);
        }
        bodies.push_back(poly);
    }
    //RapidlyExploringRandomTreeD rapidlyExploringRandomTree(RectangleD(Vec2D{100.0, 100.0f}, 400.0, 400.0));
    //RapidlyExploringRandomTreeD rapidlyExploringRandomTree(CircleD(Vec2D{300.0, 300.0f}, 200.0));
    //RapidlyExploringRandomTreeD rapidlyExploringRandomTree(TriangleD(Vec2D{100.0, 100.0f}, Vec2D{500.0, 200.0f}, Vec2D{200.0, 600.0f}));
    //RapidlyExploringRandomTreeD rapidlyExploringRandomTree(PolygonD({Vec2D{100.0, 100.0f}, Vec2D{500.0, 200.0f}, Vec2D{700.0, 600.0f}, Vec2D{50.0, 600.0f}, Vec2D{50.0, 300.0f}}));
    /*
    RapidlyExploringRandomTreeD rapidlyExploringRandomTree(PolygonD({Vec2D(700, 100), Vec2D(900, 100), Vec2D(820, 200), Vec2D(900, 300), Vec2D(800, 400), Vec2D(700, 300), Vec2D(780, 200)}));
    rapidlyExploringRandomTree.addObstacle(LineSegmentD(Vec2D {140, 140}, Vec2D {450, 300}));
    rapidlyExploringRandomTree.addObstacle(RectangleD(Vec2D {200.0, 430.0f}, 200.0, 100.0));
    rapidlyExploringRandomTree.generateNodes(2000);
    auto edges = rapidlyExploringRandomTree.edges();
    std::vector<ALLEGRO_VERTEX> edgesVertexData;
    edgesVertexData.reserve(edges.size() * 2);
    for(const auto& edge : edges)
    {
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(edge.begin.x), static_cast<float>(edge.begin.y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(edge.end.x), static_cast<float>(edge.end.y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
    }
    PolygonD polygon({Vec2D(700, 100), Vec2D(900, 100), Vec2D(820, 200), Vec2D(900, 300), Vec2D(800, 400), Vec2D(700, 300), Vec2D(780, 200)});
    PolygonTriangulationD triangulation(polygon);
    /*for(const auto& triangle : triangulation.result().elements)
    {
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[0].x), static_cast<float>(triangle.vertices[0].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[1].x), static_cast<float>(triangle.vertices[1].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});

        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[1].x), static_cast<float>(triangle.vertices[1].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[2].x), static_cast<float>(triangle.vertices[2].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});

        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[2].x), static_cast<float>(triangle.vertices[2].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
        edgesVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(triangle.vertices[0].x), static_cast<float>(triangle.vertices[0].y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 0, 0)});
    }*/

/*
    for(;;)
    {
        al_get_keyboard_state(&keyboardState);
        if(al_key_down(&keyboardState, ALLEGRO_KEY_ESCAPE)) break;
        BezierCurveD curve;
        for(const auto& p : curvePoints) curve.add(p.shape);
        std::vector<ALLEGRO_VERTEX> curveVertexData;
        auto pointsOnCurve = curve.evaluateAll(0.001);
        for(const auto& p : pointsOnCurve)
        {
            curveVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(p.x), static_cast<float>(p.y), 0.0f, 0.0f, 0.0f, al_map_rgb(255, 255, 255)});
        }

        al_get_mouse_state(&now);
        for(auto& p : points) p.move(prev, now);
        for(int i = 0; i < numberOfControlPoints; ++i) curvePoints[i].move(prev, now);
        for(auto& body : bodies)
        {
            if((prev.buttons & 1) && body.polygon.intersects(Vec2D(prev.x, prev.y)))
            {
                body.dragDrop(Vec2D(prev.x, prev.y), Vec2D(now.x, now.y));
            }
        }
        al_clear_to_color(al_map_rgb(0, 0, 0));
        int i = 0;
        for(auto& ci : circles)
        {
            ci.move(prev, now);
            c.elements[i] = ci.shape;
            ++i;
        }
        v2.move(prev, now);
        r.origin = v2.shape;
        std::vector<Geo::Vec2D> v;
        for(auto s : points) v.push_back(s.shape);

        Geo::ConvexHull<double> ch(v);
        ch.calculate();
        InteractiveShape<Geo::PolygonD> p(ch.convexHull());

        for(int i = 0; i < shapeSize; ++i)
        {
            al_draw_pixel(points[i].shape.x, points[i].shape.y, notcolliding);
        }

        for(int i = 0; i < numberOfControlPoints; ++i)
        {
            al_draw_filled_circle(curvePoints[i].shape.x, curvePoints[i].shape.y, 5.0f, al_map_rgb(0, 0, 255));
        }
        ALLEGRO_COLOR circleColor = al_map_rgb(0, 255, 0);
        for(const auto& ci : c.elements) al_draw_circle(ci.origin.x, ci.origin.y, ci.radius, circleColor, 1.0);
        next(50, r, c, bodies);
        p.draw();

        for(const auto& body : bodies)
        {
            std::vector<ALLEGRO_VERTEX> bodyVertexData;
            for(size_t i = 0; i < body.polygon.size() + 1; ++i)
            {
                int j = i % body.polygon.size();
                bodyVertexData.push_back(ALLEGRO_VERTEX {static_cast<float>(body.polygon.vertices[j].x), static_cast<float>(body.polygon.vertices[j].y), 0.0f, 0.0f, 0.0f, al_map_rgb(0, 255, 255)});
            }
            al_draw_prim(bodyVertexData.data(), nullptr, nullptr, 0, bodyVertexData.size(), ALLEGRO_PRIM_LINE_STRIP);
            al_draw_pixel(body.centerOfMass.x, body.centerOfMass.y, al_map_rgb(0, 255, 255));
        }
        al_draw_prim(curveVertexData.data(), nullptr, nullptr, 0, curveVertexData.size(), ALLEGRO_PRIM_LINE_STRIP);
        al_draw_prim(edgesVertexData.data(), nullptr, nullptr, 0, edgesVertexData.size(), ALLEGRO_PRIM_LINE_LIST);
        //al_draw_prim(pathVertexData.data(), nullptr, nullptr, 0, pathVertexData.size(), ALLEGRO_PRIM_LINE_STRIP);

        al_flip_display();
        prev = now;
    }

*/
    /*
    Noise::SimplexNoiseD simplex;
    for(int x = 0; x<1280;++x)
    {
            double noise = simplex.periodicRangedOctaveNoise1(x, 4, 0.6, 0.0077, 0, 64, 1280);

            al_draw_pixel(x,noise,al_map_rgb(255,255,255));
    }*/

    //Noise::CellNoiseD noise(123412, 123.3, 56.5, 1090.2, 876.8, 128);
    /*Noise::CellNoiseD noise;
    //noise.setDistanceFunction(Noise::CellNoiseD::DistanceFunction::Chebyshev);
    //noise.setDistanceFunction(Noise::CellNoiseD::DistanceFunction::Manhattan);
    //noise.setResultComputingFunction([](const std::vector<double>& distances) -> double {return distances[1]-distances[0];});
    for(int x = 0; x<1280/2;++x)
    {
        for(int y = 0; y<800/2; ++y)
        {
            double n = noise.rawNoise2(x/64.,y/64.);
            al_draw_pixel(x,y,al_map_rgb(n*255.,n*255.,n*255.));
        }
    }*/


    //al_flip_display();
    /*Random::XorshiftEngine engine(0);
    for(int i = 0; i < 1000; ++i)
    {
        std::cout << engine.nextFloat() << '\n';
        //std::cout << rand()/float(RAND_MAX) << '\n';
    }*/
    return 0;
}
