#ifndef TILELOCATION_H
#define TILELOCATION_H

class MapLayer;

class TileLocation
{
public:
    const MapLayer& map;
    int x;
    int y;
    int z;

    TileLocation(const MapLayer& map_, int x_, int y_, int z_);
};

#endif // TILELOCATION_H
