#ifndef RAYCAST_H
#define RAYCAST_H

template <class T>
class Raycast
{
public:
    struct Hit
    {
        T distance;
        Vec2D normal;
        Vec2D hitPoint;
    };

    Ray<T> ray; //read-only, will debate whether it should be a reference
    std::vector<Hit> hits; //distances

    Raycast() = default;
    Raycast(const Ray<T>& _ray, const std::vector<Hit>& _hits);
    Raycast(const Ray<T>& _ray, std::vector<Hit>&& _hits);

    Raycast(const Raycast<T>&) = default;
    Raycast(Raycast<T>&&) = default;

    Raycast<T>& operator = (const Raycast<T>&) = default;
    Raycast<T>& operator = (Raycast<T> &&) = default;

    size_t hitCount() const;

    void sortHitPointsByDistance();

    static Raycast<T> empty(const Ray<T>& r);
};

typedef Raycast<double> RaycastD;
typedef Raycast<float> RaycastF;
typedef Raycast<int> RaycastI;

#include "../src/Raycast.cpp"

#endif // RAYCAST_H
