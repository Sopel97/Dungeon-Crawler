template <class T>
SimplexNoise<T>::SimplexNoise()
{

}
template <class T>
SimplexNoise<T>::~SimplexNoise()
{

}
template <class T>
T SimplexNoise<T>::rawNoise1(const T x) const
{
    auto grad = [] (const int hash, const T x) {
        int h = hash & 15;
        T grad = T(1) + (h & 7);   // Gradient value 1.0, 2.0, ..., 8.0
        if(h & 8) grad = -grad;    // Set a random sign for the gradient
        return grad * x;           // Multiply the gradient with the distance
    };

    int i0 = fastFloor(x);
    int i1 = i0 + 1;
    T x0 = x - i0;
    T x1 = x0 - T(1);

    T n0, n1;

    T t0 = T(1) - x0 * x0;
    t0 *= t0;
    n0 = t0 * t0 * grad(perm[i0 & 255], x0);

    T t1 = T(1) - x1 * x1;
    t1 *= t1;
    n1 = t1 * t1 * grad(perm[i1 & 255], x1);

    //The result is scaled to return values in the interval [-1,1].
    return T(0.395) * (n0 + n1);

}

template <class T>
T SimplexNoise<T>::rawNoise2(const T x, const T y) const
{
    T n0, n1, n2;

    // Skew the input space to determine which simplex cell we're in
    T F2 = T(0.36602540378443864676372317075294); //0.5 * (sqrt(3.0) - 1.0);
    // Hairy factor for 2D
    T s = (x + y) * F2;
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);

    T G2 = T(0.21132486540518711774542560974902); //(3.0 - sqrt(3.0)) / 6.0;
    T t = (i + j) * G2;
    // Unskew the cell origin back to (x,y) space
    T X0 = i - t;
    T Y0 = j - t;
    // The x,y distances from the cell origin
    T x0 = x - X0;
    T y0 = y - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if(x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
    else { i1 = 0; j1 = 1; } // upper triangle, YX order: (0,0)->(0,1)->(1,1)

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6
    T x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    T y1 = y0 - j1 + G2;
    T x2 = x0 - T(1) + T(2) * G2; // Offsets for last corner in (x,y) unskewed coords
    T y2 = y0 - T(1) + T(2) * G2;

    // Work out the hashed gradient indices of the three simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int gi0 = perm[ii + perm[jj]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1]] % 12;
    int gi2 = perm[ii + 1 + perm[jj + 1]] % 12;

    // Calculate the contribution from the three corners
    T t0 = T(0.5) - x0 * x0 - y0 * y0;
    if(t0 < T(0)) n0 = T(0);
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[gi0], x0, y0); // (x,y) of grad3 used for 2D gradient
    }

    T t1 = T(0.5) - x1 * x1 - y1 * y1;
    if(t1 < T(0)) n1 = T(0);
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[gi1], x1, y1);
    }

    T t2 = T(0.5) - x2 * x2 - y2 * y2;
    if(t2 < T(0)) n2 = T(0);
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[gi2], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return T(70) * (n0 + n1 + n2);

}
template <class T>
T SimplexNoise<T>::rawNoise3(const T x, const T y, const T z) const
{
    T n0, n1, n2, n3; // Noise contributions from the four corners

    // Skew the input space to determine which simplex cell we're in
    T F3 = T(1) / T(3);
    T s = (x + y + z) * F3; // Very nice and simple skew factor for 3D
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);
    int k = fastFloor(z + s);

    T G3 = T(1) / T(6); // Very nice and simple unskew factor, too
    T t = (i + j + k) * G3;
    T X0 = i - t; // Unskew the cell origin back to (x,y,z) space
    T Y0 = j - t;
    T Z0 = k - t;
    T x0 = x - X0; // The x,y,z distances from the cell origin
    T y0 = y - Y0;
    T z0 = z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

    if(x0 >= y0)
    {
        if(y0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // X Y Z order
        else if(x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } // X Z Y order
        else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } // Z X Y order
    }
    else   // x0<y0
    {
        if(y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } // Z Y X order
        else if(x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } // Y Z X order
        else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // Y X Z order
    }

    // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
    // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
    // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
    // c = 1/6.
    T x1 = x0 - i1 + G3; // Offsets for second corner in (x,y,z) coords
    T y1 = y0 - j1 + G3;
    T z1 = z0 - k1 + G3;
    T x2 = x0 - i2 + T(2) * G3; // Offsets for third corner in (x,y,z) coords
    T y2 = y0 - j2 + T(2) * G3;
    T z2 = z0 - k2 + T(2) * G3;
    T x3 = x0 - T(1) + T(3) * G3; // Offsets for last corner in (x,y,z) coords
    T y3 = y0 - T(1) + T(3) * G3;
    T z3 = z0 - T(1) + T(3) * G3;

    // Work out the hashed gradient indices of the four simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int gi0 = perm[ii + perm[jj + perm[kk]]] % 12;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1]]] % 12;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2]]] % 12;
    int gi3 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1]]] % 12;

    // Calculate the contribution from the four corners
    T t0 = T(0.6) - x0 * x0 - y0 * y0 - z0 * z0;
    if(t0 < T(0)) n0 = T(0);
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
    }

    T t1 = T(0.6) - x1 * x1 - y1 * y1 - z1 * z1;
    if(t1 < T(0)) n1 = T(0);
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
    }

    T t2 = T(0.6) - x2 * x2 - y2 * y2 - z2 * z2;
    if(t2 < T(0)) n2 = T(0);
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
    }

    T t3 = T(0.6) - x3 * x3 - y3 * y3 - z3 * z3;
    if(t3 < T(0)) n3 = T(0);
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to stay just inside [-1,1]
    return T(32) * (n0 + n1 + n2 + n3);

}
template <class T>
T SimplexNoise<T>::rawNoise4(const T x, const T y, const T z, const T w) const
{
    // The skewing and unskewing factors are hairy again for the 4D case
    T F4 = T(0.30901699437494742410229341718282); //(sqrt(5.0) - 1.0) / 4.0;
    T G4 = T(0.13819660112501051517954131656344); //(5.0 - sqrt(5.0)) / 20.0;
    T n0, n1, n2, n3, n4; // Noise contributions from the five corners

    // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
    T s = (x + y + z + w) * F4; // Factor for 4D skewing
    int i = fastFloor(x + s);
    int j = fastFloor(y + s);
    int k = fastFloor(z + s);
    int l = fastFloor(w + s);
    T t = (i + j + k + l) * G4; // Factor for 4D unskewing
    T X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
    T Y0 = j - t;
    T Z0 = k - t;
    T W0 = l - t;

    T x0 = x - X0; // The x,y,z,w distances from the cell origin
    T y0 = y - Y0;
    T z0 = z - Z0;
    T w0 = w - W0;

    // For the 4D case, the simplex is a 4D shape I won't even try to describe.
    // To find out which of the 24 possible simplices we're in, we need to
    // determine the magnitude ordering of x0, y0, z0 and w0.
    // The method below is a good way of finding the ordering of x,y,z,w and
    // then find the correct traversal order for the simplex we're in.
    // First, six pair-wise comparisons are performed between each possible pair
    // of the four coordinates, and the results are used to add up binary bits
    // for an integer index.
    int c1 = (x0 > y0) ? 32 : 0;
    int c2 = (x0 > z0) ? 16 : 0;
    int c3 = (y0 > z0) ? 8 : 0;
    int c4 = (x0 > w0) ? 4 : 0;
    int c5 = (y0 > w0) ? 2 : 0;
    int c6 = (z0 > w0) ? 1 : 0;
    int c = c1 + c2 + c3 + c4 + c5 + c6;

    int i1, j1, k1, l1; // The integer offsets for the second simplex corner
    int i2, j2, k2, l2; // The integer offsets for the third simplex corner
    int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

    // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
    // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
    // impossible. Only the 24 indices which have non-zero entries make any sense.
    // We use a thresholding to set the coordinates in turn from the largest magnitude.
    // The number 3 in the "simplex" array is at the position of the largest coordinate.
    i1 = simplex[c][0] >= 3 ? 1 : 0;
    j1 = simplex[c][1] >= 3 ? 1 : 0;
    k1 = simplex[c][2] >= 3 ? 1 : 0;
    l1 = simplex[c][3] >= 3 ? 1 : 0;
    // The number 2 in the "simplex" array is at the second largest coordinate.
    i2 = simplex[c][0] >= 2 ? 1 : 0;
    j2 = simplex[c][1] >= 2 ? 1 : 0;
    k2 = simplex[c][2] >= 2 ? 1 : 0;
    l2 = simplex[c][3] >= 2 ? 1 : 0;
    // The number 1 in the "simplex" array is at the second smallest coordinate.
    i3 = simplex[c][0] >= 1 ? 1 : 0;
    j3 = simplex[c][1] >= 1 ? 1 : 0;
    k3 = simplex[c][2] >= 1 ? 1 : 0;
    l3 = simplex[c][3] >= 1 ? 1 : 0;
    // The fifth corner has all coordinate offsets = 1, so no need to look that up.

    T x1 = x0 - i1 + G4; // Offsets for second corner in (x,y,z,w) coords
    T y1 = y0 - j1 + G4;
    T z1 = z0 - k1 + G4;
    T w1 = w0 - l1 + G4;
    T x2 = x0 - i2 + T(2) * G4; // Offsets for third corner in (x,y,z,w) coords
    T y2 = y0 - j2 + T(2) * G4;
    T z2 = z0 - k2 + T(2) * G4;
    T w2 = w0 - l2 + T(2) * G4;
    T x3 = x0 - i3 + T(3) * G4; // Offsets for fourth corner in (x,y,z,w) coords
    T y3 = y0 - j3 + T(3) * G4;
    T z3 = z0 - k3 + T(3) * G4;
    T w3 = w0 - l3 + T(3) * G4;
    T x4 = x0 - T(1) + T(4) * G4; // Offsets for last corner in (x,y,z,w) coords
    T y4 = y0 - T(1) + T(4) * G4;
    T z4 = z0 - T(1) + T(4) * G4;
    T w4 = w0 - T(1) + T(4) * G4;

    // Work out the hashed gradient indices of the five simplex corners
    int ii = i & 255;
    int jj = j & 255;
    int kk = k & 255;
    int ll = l & 255;
    int gi0 = perm[ii + perm[jj + perm[kk + perm[ll]]]] & 31;
    int gi1 = perm[ii + i1 + perm[jj + j1 + perm[kk + k1 + perm[ll + l1]]]] & 31;
    int gi2 = perm[ii + i2 + perm[jj + j2 + perm[kk + k2 + perm[ll + l2]]]] & 31;
    int gi3 = perm[ii + i3 + perm[jj + j3 + perm[kk + k3 + perm[ll + l3]]]] & 31;
    int gi4 = perm[ii + 1 + perm[jj + 1 + perm[kk + 1 + perm[ll + 1]]]] & 31;

    // Calculate the contribution from the five corners
    T t0 = T(0.6) - x0 * x0 - y0 * y0 - z0 * z0 - w0 * w0;
    if(t0 < T(0)) n0 = T(0);
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * dot(grad4[gi0], x0, y0, z0, w0);
    }

    T t1 = T(0.6) - x1 * x1 - y1 * y1 - z1 * z1 - w1 * w1;
    if(t1 < T(0)) n1 = T(0);
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * dot(grad4[gi1], x1, y1, z1, w1);
    }

    T t2 = T(0.6) - x2 * x2 - y2 * y2 - z2 * z2 - w2 * w2;
    if(t2 < T(0)) n2 = T(0);
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * dot(grad4[gi2], x2, y2, z2, w2);
    }

    T t3 = T(0.6) - x3 * x3 - y3 * y3 - z3 * z3 - w3 * w3;
    if(t3 < T(0)) n3 = T(0);
    else
    {
        t3 *= t3;
        n3 = t3 * t3 * dot(grad4[gi3], x3, y3, z3, w3);
    }

    T t4 = T(0.6) - x4 * x4 - y4 * y4 - z4 * z4 - w4 * w4;
    if(t4 < T(0)) n4 = T(0);
    else
    {
        t4 *= t4;
        n4 = t4 * t4 * dot(grad4[gi4], x4, y4, z4, w4);
    }

    // Sum up and scale the result to cover the range [-1,1]
    return T(27.0) * (n0 + n1 + n2 + n3 + n4);

}

template <class T>
T SimplexNoise<T>::rangedRawNoise1(const T x, const T lowerBound, const T higherBound) const
{
    return rawNoise1(x) * (higherBound - lowerBound) * T(T(0.5)) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedRawNoise2(const T x, const T y, const T lowerBound, const T higherBound) const
{
    return rawNoise2(x, y) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedRawNoise3(const T x, const T y, const T z, const T lowerBound, const T higherBound) const
{
    return rawNoise3(x, y, z) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedRawNoise4(const T x, const T y, const T z, const T w, const T lowerBound, const T higherBound) const
{
    return rawNoise4(x, y, z, w) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}

template <class T>
T SimplexNoise<T>::octaveNoise1(const T x, const int octaves, const T persistence, const T scale) const
{
    T total = T(0);
    T frequency = scale;
    T amplitude = T(1);

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    T maxAmplitude = T(0);

    for(int i = 0; i < octaves; i++)
    {
        total += rawNoise1(x * frequency) * amplitude;

        frequency *= T(2);
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template <class T>
T SimplexNoise<T>::octaveNoise2(const T x, const T y, const int octaves, const T persistence, const T scale) const
{
    T total = T(0);
    T frequency = scale;
    T amplitude = T(1);

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    T maxAmplitude = T(0);

    for(int i = 0; i < octaves; i++)
    {
        total += rawNoise2(x * frequency, y * frequency) * amplitude;

        frequency *= T(2);
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template <class T>
T SimplexNoise<T>::octaveNoise3(const T x, const T y, const T z, const int octaves, const T persistence, const T scale) const
{
    T total = T(0);
    T frequency = scale;
    T amplitude = T(1);

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    T maxAmplitude = T(0);

    for(int i = 0; i < octaves; i++)
    {
        total += rawNoise3(x * frequency, y * frequency, z * frequency) * amplitude;

        frequency *= T(2);
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}
template <class T>
T SimplexNoise<T>::octaveNoise4(const T x, const T y, const T z, const T w, const int octaves, const T persistence, const T scale) const
{
    T total = T(0);
    T frequency = scale;
    T amplitude = T(1);

    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    T maxAmplitude = T(0);

    for(int i = 0; i < octaves; i++)
    {
        total += rawNoise4(x * frequency, y * frequency, z * frequency, w * frequency) * amplitude;

        frequency *= T(2);
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

template <class T>
T SimplexNoise<T>::rangedOctaveNoise1(const T x, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const
{
    return octaveNoise1(x, octaves, persistence, scale) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedOctaveNoise2(const T x, const T y, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const
{
    return octaveNoise2(x, y, octaves, persistence, scale) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedOctaveNoise3(const T x, const T y, const T z, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const
{
    return octaveNoise3(x, y, z, octaves, persistence, scale) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}
template <class T>
T SimplexNoise<T>::rangedOctaveNoise4(const T x, const T y, const T z, const T w, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const
{
    return octaveNoise4(x, y, z, w, octaves, persistence, scale) * (higherBound - lowerBound) * T(0.5) + (higherBound + lowerBound) * T(0.5);
}


template <class T>
T SimplexNoise<T>::periodicRangedOctaveNoise1(const T x, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound, const T xPeriod) const
{
    T xx = fmod(x, xPeriod);
    T r = xPeriod * T(0.5) * static_cast<T>(INV_PI);
    T angle = xx / xPeriod * T(2) * static_cast<T>(PI);
    xx = cos(angle) * r;
    T yy = sin(angle) * r;
    return rangedOctaveNoise2(xx, yy, octaves, persistence, scale, lowerBound, higherBound);
}
template <class T>
T SimplexNoise<T>::periodicSeededRangedOctaveNoise1(const T x, const T seed, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound, const T xPeriod) const
{
    T xx = fmod(x, xPeriod);
    T r = xPeriod * T(0.5) * static_cast<T>(INV_PI);
    T angle = xx / xPeriod * T(2) * static_cast<T>(PI);
    xx = cos(angle) * r;
    T yy = sin(angle) * r;
    return rangedOctaveNoise3(xx, yy, seed, octaves, persistence, scale, lowerBound, higherBound);
}
