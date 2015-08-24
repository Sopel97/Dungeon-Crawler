template <class T>
AffineTransformation<T>::AffineTransformation() :
    a(1), b(0), c(0),
    d(0), e(1), f(0)
{
}

template <class T>
AffineTransformation<T>::AffineTransformation(T aa, T bb, T cc, T dd, T ee, T ff) :
    a(aa), b(bb), c(cc),
    d(dd), e(ee), f(ff)
{
}

template <class T>
AffineTransformation<T> AffineTransformation<T>::fromTriangleToTriangleMapping(const Triangle<T>& from, const Triangle<T>& to)
{
    AffineTransformation<T> result;
    const T x1 = from.vertices[0].x;
    const T x2 = from.vertices[1].x;
    const T x3 = from.vertices[2].x;

    const T y1 = from.vertices[0].y;
    const T y2 = from.vertices[1].y;
    const T y3 = from.vertices[2].y;

    const T z1 = to.vertices[0].x;
    const T z2 = to.vertices[1].x;
    const T z3 = to.vertices[2].x;

    const T w1 = to.vertices[0].y;
    const T w2 = to.vertices[1].y;
    const T w3 = to.vertices[2].y;

    result.a = -(y1 * (z3 - z2) - y2 * z3 + y3 * z2 + (y2 - y3) * z1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    result.b = (x1 * (z3 - z2) - x2 * z3 + x3 * z2 + (x2 - x3) * z1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    result.c = (x1 * (y3 * z2 - y2 * z3) + y1 * (x2 * z3 - x3 * z2) + (x3 * y2 - x2 * y3) * z1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    result.d = (w1 * (y3 - y2) - w2 * y3 + w3 * y2 + (w2 - w3) * y1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    result.e = -(w1 * (x3 - x2) - w2 * x3 + w3 * x2 + (w2 - w3) * x1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    result.f = (w1 * (x3 * y2 - x2 * y3) + x1 * (w2 * y3 - w3 * y2) + (w3 * x2 - w2 * x3) * y1) / (x1 * (y3 - y2) - x2 * y3 + x3 * y2 + (x2 - x3) * y1);
    return result;
}

template <class T>
void AffineTransformation<T>::rotateClockwise(const Angle<T>& angle)
{
    rotateAntiClockwise(-angle);
}
template <class T>
void AffineTransformation<T>::rotateAntiClockwise(const Angle<T>& angle)
{
    T sinus = sin(angle.radians());
    T cosinus = cos(angle.radians());
    T aa = cosinus;
    T bb = sinus;
    T dd = -sinus;
    T ee = cosinus;
    combine(AffineTransformation<T>(aa, bb, T(0), dd, ee, T(0)));
}

template <class T>
void AffineTransformation<T>::translate(const Vec2<T>& translationVector)
{
    T cc = translationVector.x;
    T ff = translationVector.y;
    combine(AffineTransformation<T>(T(1), T(0), cc, T(0), T(1), ff));
}

template <class T>
void AffineTransformation<T>::scale(const Vec2<T>& scalingVector)
{
    T aa = scalingVector.x;
    T ee = scalingVector.y;
    combine(AffineTransformation<T>(aa, T(0), T(0), T(0), ee, T(0)));
}

template <class T>
void AffineTransformation<T>::scale(T factor)
{
    T ae = factor;
    combine(AffineTransformation<T>(ae, T(0), T(0), T(0), ae, T(0)));
}

template <class T>
void AffineTransformation<T>::shearInXDirection(T factor)
{
    T bb = factor;
    combine(AffineTransformation<T>(T(1), bb, T(0), T(0), T(1), T(0)));
}
template <class T>
void AffineTransformation<T>::shearInYDirection(T factor)
{
    T dd = factor;
    combine(AffineTransformation<T>(T(1), T(0), T(0), dd, T(1), T(0)));
}
template <class T>
void AffineTransformation<T>::shear(const Vec2<T>& shearingVector)
{
    T bb = shearingVector.x;
    T dd = shearingVector.y;
    combine(AffineTransformation<T>(T(1), bb, T(0), dd, T(1), T(0)));
}

template <class T>
void AffineTransformation<T>::reflectAboutOrigin()
{
    combine(AffineTransformation<T>(T(-1), T(0), T(0), T(0), T(-1), T(0)));
}
template <class T>
void AffineTransformation<T>::reflectAboutXAxis()
{
    combine(AffineTransformation<T>(T(1), T(0), T(0), T(0), T(-1), T(0)));
}
template <class T>
void AffineTransformation<T>::reflectAboutYAxis()
{
    combine(AffineTransformation<T>(T(-1), T(0), T(0), T(0), T(1), T(0)));
}

template <class T>
void AffineTransformation<T>::transform(Vec2<T>& point) const
{
    T px = point.x;
    T py = point.y;
    point.x = a * px + b * py + c;
    point.y = d * px + e * py + f;
}

template <class T>
Vec2<T> AffineTransformation<T>::transformed(const Vec2<T>& point) const
{
    return Vec2<T> {a* point.x + b* point.y + c, d* point.x + e* point.y + f};
}
template <class T>
AffineTransformation<T> AffineTransformation<T>::combined(const AffineTransformation<T>& transformation) const
{
    AffineTransformation<T> combinedTransformation;
    combinedTransformation.a = a * transformation.a + b * transformation.d;
    combinedTransformation.b = a * transformation.b + b * transformation.e;
    combinedTransformation.d = d * transformation.a + e * transformation.d;
    combinedTransformation.e = d * transformation.b + e * transformation.e;

    combinedTransformation.c += a * transformation.c + b * transformation.f;
    combinedTransformation.f += d * transformation.c + e * transformation.f;
    return combinedTransformation;
}
template <class T>
void AffineTransformation<T>::combine(const AffineTransformation<T>& transformation)
{
    T aa = a;
    T bb = b;
    T dd = d;
    T ee = e;

    a = aa * transformation.a + bb * transformation.d;
    b = aa * transformation.b + bb * transformation.e;
    c += aa * transformation.c + bb * transformation.f;
    d = dd * transformation.a + ee * transformation.d;
    e = dd * transformation.b + ee * transformation.e;
    f += dd * transformation.c + ee * transformation.f;
}
