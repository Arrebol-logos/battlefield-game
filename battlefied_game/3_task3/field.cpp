#include <string>
#include <cassert>
#include <iomanip>
#include "field.h"
#include "units.h"
using namespace std;


/* Battle field */

// Constructor
Field::Field(int h, int w)
    : units(h, w),terrains(h,w)
{
    assert (h > 0 && h <=20 && w > 0 && w <= 20);
}


// Get the height of the field
int Field::getHeight() const
{
    return units.numRows();
}

// Get the width of the field
int Field::getWidth() const
{
    return units.numCols();
}

// In bounds check
bool Field::inBounds(int row, int col) const
{
    return units.inBounds(row, col);
}

// Get the unit at row and col
Unit* Field::getUnit(int row, int col) const
{
    if (units.inBounds(row, col))
       return units[row][col];
    else {

    }
}

// Set a new unit at row and col
// Return false if the coordinate is already occupied
// or unreachable

bool Field::setUnit(int row, int col, Unit* u)//似乎没有指定unit的类型是kn还是ft还是弓
{
    assert (units.inBounds(row, col));

    if (units[row][col] == NULL) {
        u->setCoord(row, col);
        units[row][col] = u;
        return true;
    }
    else
        return false;
}

// Move a unit to a different coordinate
// Return false if the source does not exist or
// the target coordinate is already occupied or
// unreachable

void Field::deleteUnit(int row, int column){
    units[row][column] = NULL;
}

bool Field::moveUnit(int srow, int scol, int trow, int tcol,Grid<bool> grd)
{
    /*assert (units.inBounds(srow, scol));
    assert (units.inBounds(trow, tcol));
    assert (units[trow][tcol] == NULL);*///检查目标位置有无士兵
    Unit* u = units[srow][scol];
    if (grd[trow][tcol]==1){
    u->setCoord(trow,tcol);
    units[trow][tcol] = u;
    units[srow][scol] = NULL;
    }//相当于把原来的u改掉了，并移动其在units中的位置
}

// Reclaim all the units
Field::~Field()
{
    for (int i = 0; i < units.numRows(); i++)
    for (int j = 0; j < units.numCols(); j++)
        if (units[i][j] != NULL)
            delete units[i][j];
}

bool Field::setTerrain(int row, int col, Terrain* t)//似乎没有指定unit的类型是kn还是ft还是弓
{
    assert (units.inBounds(row, col));

    if (terrains[row][col] == NULL) {
        t->setCoord(row, col);
        terrains[row][col] = t;
        return true;
    }
    else
        return false;
}

Terrain* Field::getTerrain(int row, int col) const {
    assert (units.inBounds(row, col));

    return terrains[row][col];
}