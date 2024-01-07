#include "actions.h"
#include "units.h"
#include <cmath>
#include <cassert>
using namespace std;

// Get actions that can be performed by a unit
Vector<Action> getActions(Unit* u)
{
    Vector<Action> acts;
    acts.add(MOVE);
    acts.add(SKIP);
    return acts;
}

// Move units
void moveUnit(Field& field, Unit* u, int trow, int tcol, Grid<bool> grd)
{
    if (u != NULL){
    int srow = u->getRow();
    int scol = u->getCol();
    field.moveUnit(srow, scol, trow, tcol,grd);
}
}

// Select unit
SelectUResult selectUnit(const Field& field, bool side, int row, int col, Unit*& u)
{
    u = field.getUnit(row, col);
    if (u == NULL)
        return SELECTU_NO_UNIT;

    if (u->getSide() != side)
        return SELECTU_WRONG_SIDE;

    return SELECTU_SUCCESS;
}
