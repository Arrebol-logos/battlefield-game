#include <sstream>
#include <iomanip>
#include <cassert>
#include "engine.h"
#include "actions.h"
#include "units.h"
#include "algorithms.h"
using namespace std;

// Add units
bool addFootman(Field* field, bool sd, int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Unit* ft = new Unit(FOOTMAN, sd, row, col);
    return field->setUnit(row, col, ft);
}

bool addKnight(Field* field, bool sd, int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Unit* kn = new Unit(KNIGHT, sd, row, col);
    return field->setUnit(row, col, kn);
}

bool addMountain(Field* field,int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Terrain* mo = new Terrain(Mountain,row, col);
    return field->setTerrain(row, col, mo);
}

bool addForest(Field* field,int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Terrain* mo = new Terrain(Forest,row, col);
    return field->setTerrain(row, col, mo);
}

bool addWater(Field* field,int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Terrain* mo = new Terrain(Water,row, col);
    return field->setTerrain(row, col, mo);
}

bool checkBounds(ostream& os, const Field& field, int row, int col);
bool readRowCol(const string& str, int& row, int& col);
bool queryRowCol(istream& is, ostream& os, int& row, int& col);
bool performActions(ostream& os, istream& is, Field& field, Unit* u,int& flag_1,int& flag);
bool checkWinning(ostream& os, Field& field);

// Main loop for playing the game
Field* loadMap(std::istream& is)
{
    string row_new,column_new,special_terrain,special_units,data;
    getline(is, data);
    stringstream all_data(data);
    all_data >> row_new >> column_new >> special_terrain >> special_units;
    Field* field =  new Field(stoi(row_new),stoi(column_new));
    for (int i=0;i<stoi(special_terrain);i++){
        string row,column,tre,data_;
        getline(is, data_);
        stringstream all_data_(data_);
        all_data_ >> row >> column >> tre;
        if (tre == "M") {
            addMountain(field,stoi(row),stoi(column));
        }
        else if (tre == "F") {
            addForest(field,stoi(row),stoi(column));
        }
        else if (tre == "W") {
            addWater(field,stoi(row),stoi(column));
        }
    }
    for (int i=0;i<stoi(special_units);i++){
        string row,column,side,kind,data_;
        getline(is, data_);
        stringstream all_data_(data_);
        all_data_ >> row >> column >> side >> kind;
        if (kind == "FT") {
            if (side == "A") {
               addFootman(field,true,stoi(row),stoi(column));
        }
            else if (side == "B") {
                addFootman(field,false,stoi(row),stoi(column));
            }
        }
        else if (kind == "KN") {
            if (side == "A") {
                addKnight(field,true,stoi(row),stoi(column));
            }
            else if (side == "B") {
                addKnight(field, false,stoi(row),stoi(column));
            }

        }
    }
    return field ;  ; //我也不知道return啥
}
void play(Field& field, istream& is, ostream& os)
{
    string winner = " ";
    bool side = true;
    int numTurns = 1;
    while (is) {
        os << endl;
        // Print the new map
        displayField(os, field);
        int flag = 0;
        Unit *s;
        // Perform actions
            string player;
            if (side)
                player = "Player A";
            else
                player = "Player B";
            os << "Turn " << numTurns << " (" << player << ")" << endl;
            // Choose the source coordination
            int flag_1 = 0;
            os << "Please select a unit: ";
            int srow, scol;
            if (!queryRowCol(is, os, srow, scol)){
                flag = -1;
                continue;
            }
            if (!checkBounds(os, field, srow, scol)){
                flag = -1;
                continue;}

            // Select a unit
            Unit *u;
            SelectUResult sres = selectUnit(field, side, srow, scol, u);
            s = u;
            if (sres == SELECTU_NO_UNIT) {
                os << "No unit at (" << srow << ", " << scol << ")!" << endl;
                flag = -1;
                continue;
            } else if (sres == SELECTU_WRONG_SIDE) {
                os << "Unit at (" << srow << ", " << scol << ") is an enemy!" << endl;
                flag = -1;
                continue;
            }

            // Perform actions
            if (performActions(os, is, field, u ,flag_1,flag)) {
                if (flag_1 == 2) {
                    flag = 2;//跳到结尾
                }
                side = !side;
                numTurns++;
            }
        }
        /*while (flag == 1) {
            int flag_1 = 1;
            if (performActions(os,is,field,s,flag_1,flag)) {
                flag = 2;
            }
        }*/
    }

// Check if (row, col) is in bounds
bool checkBounds(ostream& os, const Field& field, int row, int col)
{
    if (!field.inBounds(row, col)) {
        os << "(" <<row << ", " << col << ") is out of bound" << endl;
        return false;
    }
    return true;
}

// Read row and column from the string
bool readRowCol(const string& str, int& row, int& col)
{
    stringstream ss(str);
    ss >> row >> col;
    if (ss)
        return true;
    else
        return false;
}

// Query row and column from input stream
bool queryRowCol(istream& is, ostream& os, int& row, int& col)
{
    string line;
    if (!getline(is, line)) {
        os << "Read line failed!" << endl;
        return false;
    }

    if (!readRowCol(line, row, col)) {
        os << "Incorrect input! "<< endl;
        return false;
    }

    return true;
}

// Display actions
void displayActions(ostream& os, const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++) {
        os << i+1 << ".";
        switch (acts[i]) {
        case MOVE:
            os << "Move ";
            break;

        case SKIP:
            os << "Skip ";
            break;

        default:
            assert(false);
        }
    }
    os << endl;
}

// Display a menu and query for the actions
bool queryAction(ostream& os, istream& is, Vector<Action>& acts, Action& res)
{
    int i;
    string line;
    getline(is, line);
    stringstream ss(line);
    ss >> i;
    if (!ss) return false;
    i--;
    if (i < 0 || i >= acts.size())
        return false;
    else {
        res = acts[i];
        return true;
    }
}

// Collection actions that can be performed by a unit
Vector<Action> collectActions(Unit* u)
{
    Vector<Action> acts = getActions(u);

    return acts;
}

// Check if there is only skip action
bool isAllSkip(const Vector<Action>& acts)
{
    for (int i = 0; i < acts.size(); i++)
        if (acts[i] != SKIP)
            return false;
    return true;
}

// Convert field to costs
Grid<int> getFieldCosts(const Field& field)
{
    int h = field.getHeight();
    int w = field.getWidth();
    Grid<int> costs(h, w);

    for (int i = 0; i < h; i++)
    for (int j = 0; j < w; j++) {
        if ((field.getUnit(i,j) == NULL)){
            Terrain* the_land = field.getTerrain(i,j);
            if (the_land != NULL) {
                if (the_land->getSymbol_terrain() == "~~") {
                    costs[i][j] = 999;
                } else if (the_land->getSymbol_terrain() == "/\\") {
                    costs[i][j] = 999;
                } else if (the_land->getSymbol_terrain() == "$ ") {
                    costs[i][j] = 2;
                }
            }
            else {
                costs[i][j] = 1;
            }
        }
        else {
            costs[i][j] = 999;
        }
    }

    return costs;
}

// Perform the move action
bool performMove(ostream& os, istream& is, Field& field, Unit* u)
{
    // Display the reachable points
    Grid<bool> grd =
        searchReachable(getFieldCosts(field), u->getRow(), u->getCol(), u->getMovPoints());
    os << endl;
    displayField(os, field, grd);

    // Choose the target coordinate
    os << "Please enter your destination: ";
    int trow, tcol;
    if (!queryRowCol(is, os, trow, tcol)){
        /*displayField(os, field)*/;
        return false;}
    if (!checkBounds(os,field,trow,tcol)){
        /*displayField(os, field);*///直接套用了
        return false;}
    if (!grd[trow][tcol]){
        os << "Cannot reach (" << trow << ", " << tcol << ")!"<< endl;
        /*os << "" << endl;
        displayField(os, field);*///检查是否这个地方移动力不够
        return false;}

    // Move the target
    moveUnit(field, u, trow, tcol,grd);

    return true;
}

// Perform actions
bool performActions(ostream& os, istream&is, Field& field, Unit* u,int& flag_1,int& flag)
{
    Vector<Action> acts = collectActions(u);
    if (isAllSkip(acts))
        return true;

    displayActions(os, acts);
    os << "Select your action: ";
    Action act;
    if (!queryAction(os, is, acts, act)) {
        os << "Invalid action!" << endl;
        return false;
    }

    switch(act) {
    case MOVE:
        flag_1 = 2;
        return performMove(os, is, field, u);

    case SKIP:
        flag_1 = 2;
        return true;

    default:
        os << "(Action not implemented yet)" << endl;
        return false;
    }

    assert (false);
}

// Print the horizontal line
void printHLine(ostream& os, int n)
{
    os << "  ";
    for (int i = 0; i < n; i++)
        os << "+--";
    os << "+" << endl;
}

// Display the field on the out stream os
void displayField(ostream& os, const Field& field, const Grid<bool>& grd)
{
    int height = field.getHeight();
    int width = field.getWidth();

    // Print the x coordinates
    os << "  ";
    for (int i = 0; i < width; i++)
        os << ' ' << setw(2) << i;
    os << endl;

    printHLine(os, width);
    for (int i = 0; i < height; i++) {
        os << setw(2) << i;
        for (int j = 0; j < width; j++) {
            os << '|';
            Unit* u = field.getUnit(i,j);
            Terrain* t = field.getTerrain(i,j);
            if (u != NULL)
                os << u->getSymbol();
            else if ((t != NULL)&&((t->getSymbol_terrain() == "/\\")||(t->getSymbol_terrain() == "~~")))
                os << t->getSymbol_terrain();
            else if (grd.inBounds(i,j) && grd[i][j])
                os << ". ";
            else if ((t != NULL)&&(t->getSymbol_terrain() == "$ "))
                os << t->getSymbol_terrain();
            else {
                os << "  ";
            }
        }
        os << '|' << endl;
        printHLine(os, width);
    }
    os << endl;
}

