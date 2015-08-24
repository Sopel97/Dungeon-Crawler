#ifndef AFFINETRANSFORMATION_H_INCLUDED
#define AFFINETRANSFORMATION_H_INCLUDED

//TODO: make proper transformations and combining them.


template <class T>
class AffineTransformation : public Transformation2<T>
{
public:
    T a, b, c; //matrix {{a,b},{c,d}}+vector{c,f}
    T d, e, f;

    AffineTransformation(); //initially identity transform
    AffineTransformation(T aa, T bb, T cc, T dd, T ee, T ff);
    static AffineTransformation<T> fromTriangleToTriangleMapping(const Triangle<T>& from, const Triangle<T>& to);

    void rotateClockwise(const Angle<T>& angle);
    void rotateAntiClockwise(const Angle<T>& angle);

    void translate(const Vec2<T>& translationVector);

    void scale(const Vec2<T>& scalingVector);
    void scale(T factor);

    void shearInXDirection(T factor);
    void shearInYDirection(T factor);
    void shear(const Vec2<T>& shearingVector);

    void reflectAboutOrigin();
    void reflectAboutXAxis();
    void reflectAboutYAxis();

    virtual void transform(Vec2<T>& point) const;
    virtual Vec2<T> transformed(const Vec2<T>& point) const;

    AffineTransformation<T> combined(const AffineTransformation<T>& transformation) const;
    void combine(const AffineTransformation<T>& transformation);
private:
};


typedef AffineTransformation<double> AffineTransformationD;
typedef AffineTransformation<float> AffineTransformationF;
typedef AffineTransformation<int> AffineTransformationI;

extern template class AffineTransformation<double>;
extern template class AffineTransformation<float>;
extern template class AffineTransformation<int>;

#include "../src/AffineTransformation.cpp"

#endif // AFFINETRANSFORMATION_H_INCLUDED
