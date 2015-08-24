#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED


//TODO: rewrite to separate 2d case from 3d case
template <class ShapeType>
class Mesh2 : public Shape2<typename ShapeType::ValueType>
{
public:
    typedef typename ShapeType::ValueType T;

    struct RandomPointPickerPreprocessedData : public Shape2<T>::RandomPointPickerPreprocessedData
    {
        RandomPointPickerPreprocessedData(const Mesh2<ShapeType>& mesh)
        {
            shapesByArea.reserve(mesh.elements.size());
            for(auto& shape : mesh.elements)
            {
                shapesByArea.push_back(std::make_pair(&shape, shape.area()));
            }
            size_t numberOfShapes = shapesByArea.size();
            for(size_t i = 1; i < numberOfShapes; ++i)
            {
                shapesByArea[i].second += shapesByArea[i-1].second; //so the area is cumulative
            }
        }
        std::vector<std::pair<const ShapeType*, T>> shapesByArea; //<triangle ptr to triangulation result, CUMULATIVE area> - cumulative area so binary searching is possible without sorting
    };

    std::vector<ShapeType> elements;
    Mesh2();
    Mesh2(const std::vector<ShapeType>& e);
    Mesh2(std::vector<ShapeType>&& e);
    Mesh2(const std::initializer_list<ShapeType>& list);
    Mesh2(ShapeType* e, size_t count);

    Mesh2(const Mesh2<ShapeType>& m);
    Mesh2(Mesh2<ShapeType>&& m);

    virtual ~Mesh2(){}

    Mesh2<ShapeType>& operator=(const Mesh2<ShapeType>& m);
    Mesh2<ShapeType>& operator=(Mesh2<ShapeType> && m);

    Mesh2<ShapeType> operator+(const Vec2<T>& v) const;
    Mesh2<ShapeType> operator-(const Vec2<T>& v) const;

    Mesh2<ShapeType>& operator+=(const Vec2<T>& v);
    Mesh2<ShapeType>& operator-=(const Vec2<T>& v);

    void add(const ShapeType& e);
    void add(const std::vector<ShapeType>& e);
    void add(const Mesh2<ShapeType>& m);
    void add(const std::initializer_list<ShapeType>& list);

    void add(ShapeType&& e);
    void add(std::vector<ShapeType>&& e);
    void add(Mesh2<ShapeType>&& m);

    void translate(const Vec2<T>& v);
    void scale(const Vec2<T>& s);

    T distanceTo(const Vec2<T>& v1) const;
    Vec2<T> nearestPointTo(const Vec2<T>& point) const;

    size_t size() const;

    //std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> createPreprocessedDataForRandomPointPicker() const;

    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine) const;
    //Vec2<T> pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const; //preprocessed data is of base type. All shapes have to cast it to use it.
};

#include "../src/Mesh2.cpp"
#endif // MESH_H_INCLUDED
