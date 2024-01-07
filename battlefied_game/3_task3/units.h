#ifndef UNITS_H_INCLUDED
#define UNITS_H_INCLUDED

#include <string>
#include "field.h"

/* Type of Units */
enum UnitType {FOOTMAN,KNIGHT,ARCHER};
/* Type of Units */
enum TerrainType {Mountain,Forest,Water};

/* Class for units */
class Unit {
public:
    Unit(UnitType u, bool sd, int col, int row);

    // Check which side the unit belongs to
    bool getSide() const;

    // Get the coordinate of the current unit
    int getRow() const;
    int getCol() const;

    // Set the coordinates
    void setCoord(int row, int col);

    // Get your movement point
    int getMovPoints() const;

    // Return a displayable character
    // Depending on the player you may get different results
    std::string getSymbol() const;

private:
    UnitType type;
    bool side;
    int urow, ucol;
};

/* Class for terrains */
class Terrain {
public:
    Terrain(TerrainType u,  int col, int row);
    int getRow() const;
    int getCol() const;
    std::string getSymbol_terrain() const;
    void setCoord(int row, int col);
private:
    TerrainType type;
    int tcol,trow;
};
#endif // UNITS_H_INCLUDED
