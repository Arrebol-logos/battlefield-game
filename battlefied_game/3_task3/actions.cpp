#include "actions.h"
#include "units.h"
#include <cmath>
#include <cassert>
using namespace std;

// Get actions that can be performed by a unit
Vector<Action> getActions(Unit* u,int flag)//我增加了attack
{
    Vector<Action> acts;
    if ((flag == 0)&&((u->getSymbol() != "AR")&&(u->getSymbol() != "ar"))) {
        acts.add(MOVE);
        acts.add(ATTACK);
        acts.add(SKIP);
    }
    if ((flag == 1)&&((u->getSymbol() != "AR")&&(u->getSymbol() != "ar"))) {
        acts.add(ATTACK);
        acts.add(SKIP);
    }
    if ((flag == 0)&&((u->getSymbol() == "AR")||(u->getSymbol() == "ar"))) {
        acts.add(MOVE);
        acts.add(SHOOT);
        acts.add(SKIP);
    }
    if ((flag == 1)&&((u->getSymbol() == "AR")||(u->getSymbol() == "ar"))) {
        acts.add(SHOOT);
        acts.add(SKIP);
    }

    return acts;
}

// Move units
void moveUnit(Field& field, Unit* u, int trow, int tcol,Grid<bool> grd)
{
    if(u != NULL) {
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
