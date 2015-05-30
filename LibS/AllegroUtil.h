#ifndef ALLEGROUTIL_H_INCLUDED
#define ALLEGROUTIL_H_INCLUDED

#include "Util.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

/* File src/AllegroUtil.cpp has to be compiled and linked separately!!! No solution yet. */

class AllegroUtil
{
public:
    static void outputBitmapFlagsToStream(ALLEGRO_BITMAP* bitmap, std::ostream& stream = std::cout);

    static std::vector<ALLEGRO_VERTEX> constructQuadAsTriangleList(float x1, float y1, float x2, float y2, float u1, float v1, ALLEGRO_COLOR color);
    static std::vector<ALLEGRO_VERTEX> constructQuadAsTriangleList(const Vec2F& topLeft, const Vec2F& bottomRight, const Vec2F& uv, ALLEGRO_COLOR color);

    static void appendQuadAsTriangleList(std::vector<ALLEGRO_VERTEX>& vec, float x1, float y1, float x2, float y2, float u1, float v1, ALLEGRO_COLOR color);
    static void appendQuadAsTriangleList(std::vector<ALLEGRO_VERTEX>& vec, const Vec2F& topLeft, const Vec2F& bottomRight, const Vec2F& uv, ALLEGRO_COLOR color);

    static std::vector<ALLEGRO_VERTEX> constructQuadAsTriangleFan(float x1, float y1, float x2, float y2, float u1, float v1, ALLEGRO_COLOR color);
    static std::vector<ALLEGRO_VERTEX> constructQuadAsTriangleFan(const Vec2F& topLeft, const Vec2F& bottomRight, const Vec2F& uv, ALLEGRO_COLOR color);

    static void appendQuadAsTriangleFan(std::vector<ALLEGRO_VERTEX>& vec, float x1, float y1, float x2, float y2, float u1, float v1, ALLEGRO_COLOR color);
    static void appendQuadAsTriangleFan(std::vector<ALLEGRO_VERTEX>& vec, const Vec2F& topLeft, const Vec2F& bottomRight, const Vec2F& uv, ALLEGRO_COLOR color);

    static void drawBitmap(ALLEGRO_BITMAP* bmp, float x, float y, ALLEGRO_COLOR color = al_map_rgb(255, 255, 255));

    class BitmapShifter
    {
    public:
        BitmapShifter(int x, int y);
        ~BitmapShifter();
        void shift(ALLEGRO_BITMAP* bitmap, int x, int y, ALLEGRO_COLOR color);
    private:
        ALLEGRO_BITMAP* m_swapBuffer;
        int m_width;
        int m_height;
    };
};

#endif // ALLEGROUTIL_H_INCLUDED
