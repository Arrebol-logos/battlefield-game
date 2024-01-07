#include "units.h"
#include "field.h"
#include <cassert>
using namespace std;

/* Unit */

Unit::Unit(UnitType t, bool sd, int row, int col)
    : type(t), side(sd), urow(row), ucol(col)
    {}

bool Unit::getSide() const
{
    return side;
}

int Unit::getMovPoints() const//添加骑士
{
    switch (type) {
        case FOOTMAN:
            return 3;
        case KNIGHT:
            return 5;
        case ARCHER:
            return 3;
    }
}

// Get the coordinate of the current unit
int Unit::getRow() const
{
    return urow;
}

int Unit::getCol() const
{
    return ucol;
}

// Set the coordinates
void Unit::setCoord(int row, int col)
{
    urow = row;
    ucol = col;
}

// Function for symbols
string getSymbolFootman(bool side)
{
    if (side)
        return "FT";
    else
        return "ft";
}
string getSymbolKnight(bool side)
{
    if (side)
        return "KN";
    else
        return "kn";
}
string getSymbolArcher(bool side)
{
    if (side)
        return "AR";
    else
        return "ar";
}

// Get the symbol of the unit
string Unit::getSymbol() const
{
    switch (type) {
    case FOOTMAN:
        return getSymbolFootman(side);
    case KNIGHT:
        return getSymbolKnight(side);
    case ARCHER:
        return getSymbolArcher(side);
    }
    assert (false);
}
//写了一个地形的新类
Terrain::Terrain(TerrainType u, int col, int row)
        : type(u),  trow(row), tcol(col)
{}

int Terrain::getRow() const
{
    return trow;
}

int Terrain::getCol() const
{
    return tcol;
}

string Terrain::getSymbol_terrain() const
{
    if(type == Mountain)
        return "/\\";
    else if (type == Water)
        return "~~";
    else if (type == Forest)
        return "$ ";

}

void Terrain::setCoord(int row, int col)
{
    trow = row;
    tcol = col;
}