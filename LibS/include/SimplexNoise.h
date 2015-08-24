#ifndef SIMPLEXNOISE_H_INCLUDED
#define SIMPLEXNOISE_H_INCLUDED

template <class T>
class SimplexNoise
{
public:
    SimplexNoise();

    ~SimplexNoise();

    //every rawNoise function returns values in range <-1, 1>
    T rawNoise1(const T x) const;
    T rawNoise2(const T x, const T y) const;
    T rawNoise3(const T x, const T y, const T z) const;
    T rawNoise4(const T x, const T y, const T z, const T w) const;

    T rangedRawNoise1(const T x,                                  const T lowerBound, const T higherBound) const;
    T rangedRawNoise2(const T x, const T y,                       const T lowerBound, const T higherBound) const;
    T rangedRawNoise3(const T x, const T y, const T z,            const T lowerBound, const T higherBound) const;
    T rangedRawNoise4(const T x, const T y, const T z, const T w, const T lowerBound, const T higherBound) const;

    T octaveNoise1(const T x,                                  const int octaves, const T persistence, const T scale) const;
    T octaveNoise2(const T x, const T y,                       const int octaves, const T persistence, const T scale) const;
    T octaveNoise3(const T x, const T y, const T z,            const int octaves, const T persistence, const T scale) const;
    T octaveNoise4(const T x, const T y, const T z, const T w, const int octaves, const T persistence, const T scale) const;

    T rangedOctaveNoise1(const T x,                                  const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const;
    T rangedOctaveNoise2(const T x, const T y,                       const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const;
    T rangedOctaveNoise3(const T x, const T y, const T z,            const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const;
    T rangedOctaveNoise4(const T x, const T y, const T z, const T w, const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound) const;

    T periodicRangedOctaveNoise1(const T x,                                  const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound, const T xPeriod) const;
    T periodicSeededRangedOctaveNoise1(const T x, const T seed,              const int octaves, const T persistence, const T scale, const T lowerBound, const T higherBound, const T xPeriod) const; //may be temporary, will use 2d noise for seed

private:
    T dot(const int* g, const T x, const T y) const { return g[0] * x + g[1] * y; }
    T dot(const int* g, const T x, const T y, const T z) const { return g[0] * x + g[1] * y + g[2] * z; }
    T dot(const int* g, const T x, const T y, const T z, const T w) const { return g[0] * x + g[1] * y + g[2] * z + g[3] * w; }

    static int grad3[12][3];
    static int grad4[32][4];
    static int perm[512];
    static int simplex[64][4];
};

template<class T>
int SimplexNoise<T>::grad3[12][3] =
{
    {1, 1, 0}, { -1, 1, 0}, {1, -1, 0}, { -1, -1, 0},
    {1, 0, 1}, { -1, 0, 1}, {1, 0, -1}, { -1, 0, -1},
    {0, 1, 1}, {0, -1, 1},  {0, 1, -1}, {0, -1, -1}
};

template<class T>
int SimplexNoise<T>::grad4[32][4] =
{
    {0, 1, 1, 1},   {0, 1, 1, -1},   {0, 1, -1, 1},   {0, 1, -1, -1},
    {0, -1, 1, 1},  {0, -1, 1, -1},  {0, -1, -1, 1},  {0, -1, -1, -1},
    {1, 0, 1, 1},   {1, 0, 1, -1},   {1, 0, -1, 1},   {1, 0, -1, -1},
    { -1, 0, 1, 1}, { -1, 0, 1, -1}, { -1, 0, -1, 1}, { -1, 0, -1, -1},
    {1, 1, 0, 1},   {1, 1, 0, -1},   {1, -1, 0, 1},   {1, -1, 0, -1},
    { -1, 1, 0, 1}, { -1, 1, 0, -1}, { -1, -1, 0, 1}, { -1, -1, 0, -1},
    {1, 1, 1, 0},   {1, 1, -1, 0},   {1, -1, 1, 0},   {1, -1, -1, 0},
    { -1, 1, 1, 0}, { -1, 1, -1, 0}, { -1, -1, 1, 0}, { -1, -1, -1, 0}
};
template<class T>
int SimplexNoise<T>::perm[512]  =
{
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
    35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
    55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
    18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
    189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
    172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
    107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,

    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142,
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117,
    35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71,
    134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41,
    55, 46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89,
    18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182,
    189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43,
    172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239,
    107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};
template<class T>
int SimplexNoise<T>::simplex[64][4]  =
{
    {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 0, 0, 0}, {0, 2, 3, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 2, 3, 0},
    {0, 2, 1, 3}, {0, 0, 0, 0}, {0, 3, 1, 2}, {0, 3, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 3, 2, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
    {1, 2, 0, 3}, {0, 0, 0, 0}, {1, 3, 0, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
    {1, 0, 2, 3}, {1, 0, 3, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {2, 0, 3, 1}, {0, 0, 0, 0}, {2, 1, 3, 0},
    {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
    {2, 0, 1, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 0, 1, 2}, {3, 0, 2, 1}, {0, 0, 0, 0}, {3, 1, 2, 0},
    {2, 1, 0, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {3, 1, 0, 2}, {0, 0, 0, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
};
typedef SimplexNoise<double> SimplexNoiseD;
typedef SimplexNoise<float> SimplexNoiseF;

extern template class SimplexNoise<double>;
extern template class SimplexNoise<float>;

#include "../src/SimplexNoise.cpp"
#endif // NOISE_H_INCLUDED

