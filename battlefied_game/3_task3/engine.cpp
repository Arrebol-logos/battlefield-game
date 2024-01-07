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

bool addArcher(Field* field, bool sd, int row, int col)
{
    if (field->getUnit(row, col) != NULL)
        return false;

    Unit* ar = new Unit(ARCHER, sd, row, col);
    return field->setUnit(row, col, ar);
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
bool queryRowCol(istream& is, ostream& os, int& row, int& col, int& flag);
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

        else if (kind == "AR") {
            if (side == "A") {
                addArcher(field,true,stoi(row),stoi(column));
            }
            else if (side == "B") {
                addArcher(field, false,stoi(row),stoi(column));
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
        // Print the new map
        int count_c = 0;
        int count_d = 0;
        for(int i=0;i<field.getHeight();i++){
            for (int j=0;j<field.getWidth();j++) {
                if (field.getUnit(i,j) != 0) {
                    if ((field.getUnit(i,j)->getSymbol() == "KN")||(field.getUnit(i,j)->getSymbol() == "FT")||(field.getUnit(i,j)->getSymbol() == "AR")){
                        count_c+=1;
                    } else{
                        count_d+=1;
                    }
                }
            }
        }
        if((count_c>0)&&(count_d==0)){
            winner = "A";
            break;
        }
        else if ((count_c==0)&&(count_d>0)){
            winner = "B";
            break;
        }

        int flag = 0;
        Unit *s;
        // Perform actions
        while (flag == 0){
            os << endl;
            displayField(os, field);
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
            if (!queryRowCol(is, os, srow, scol,flag))
                continue;
            if (!checkBounds(os, field, srow, scol))
                continue;

            // Select a unit
            Unit *u;
            SelectUResult sres = selectUnit(field, side, srow, scol, u);
            s = u;
            if (sres == SELECTU_NO_UNIT) {
                os << "No unit at (" << srow << ", " << scol << ")!" << endl;
                continue;
            } else if (sres == SELECTU_WRONG_SIDE) {
                os << "Unit at (" << srow << ", " << scol << ") is an enemy!" << endl;
                continue;
            }

            // Perform actions
            if (performActions(os, is, field, u ,flag_1,flag)) {
                if (flag_1 == 1) {
                    flag = 1;//跳到stage 2
                } else if ((flag_1 == 2) || (flag_1 == 3)) {
                    flag = 2;//跳到结尾
                }
            }

        }
        while (flag == 1) {
            int flag_1 = 1;
            if (performActions(os,is,field,s,flag_1,flag)) {
                flag = 2;
            }
        }
        side = !side;
        numTurns++;
        int count_A = 0;
        int count_B = 0;
        for(int i=0;i<field.getHeight();i++){
            for (int j=0;j<field.getWidth();j++) {
                if (field.getUnit(i,j) != 0) {
                    if ((field.getUnit(i,j)->getSymbol() == "KN")||(field.getUnit(i,j)->getSymbol() == "FT")||(field.getUnit(i,j)->getSymbol() == "AR")){
                        count_A+=1;
                    } else{
                        count_B+=1;
                    }
                }
            }
        }
        if((count_A>0)&&(count_B==0)){
            winner = "A";
            break;
        }
        else if ((count_A==0)&&(count_B>0)){
            winner = "B";
            break;
        }
        if (flag==3)
            break;
    }
    if (winner != " ") {
        os << endl;
        displayField(os, field);
        os << "Congratulations! Player " << winner << " has won!" << endl;
    }
    else {
        os << "Read line failed!" << endl;
    }
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
bool queryRowCol(istream& is, ostream& os, int& row, int& col,int& flag)//尝试一下，加个break
{
    string line;
    if (!getline(is, line)) {
        flag = 3;
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
        case ATTACK:
            os << "Attack ";
            break;
        case SHOOT:
            os << "Shoot ";
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
bool queryAction(ostream& os, istream& is, Vector<Action>& acts, Action& res,int& flag)//两个关键
{
    int i;
    string line;
    getline(is, line);
    stringstream ss(line);
    ss >> i;
    if (!ss) {
        flag = 3;
        return false;}
    i--;
    if (i < 0 || i >= acts.size())
        return false;
    else {
        res = acts[i];
        return true;
    }
}

// Collection actions that can be performed by a unit
Vector<Action> collectActions(Unit* u, int flag)//两个关键
{
    Vector<Action> acts = getActions(u, flag);

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
bool performMove(ostream& os, istream& is, Field& field, Unit* u,int& flag)
{
    // Display the reachable points
    Grid<bool> grd =
        searchReachable(getFieldCosts(field), u->getRow(), u->getCol(), u->getMovPoints());
    os << endl;
    displayField(os, field, grd);

    // Choose the target coordinate
    os << "Please enter your destination: ";
    int trow, tcol;
    if (!queryRowCol(is, os, trow, tcol,flag)){//这里可能也要break
        return false;}
    if (!checkBounds(os,field,trow,tcol)){//直接套用了
        return false;}
    if (!grd[trow][tcol]){
        os << "Cannot reach (" << trow << ", " << tcol << ")!"<< endl;//检查是否这个地方移动力不够
        return false;}
    // Move the target
    moveUnit(field, u, trow, tcol,grd);
    os << endl;
    displayField(os, field);
    return true;
}
bool performAttack(ostream& os, istream& is, Field& field, Unit* u,int& flag){//开始写attack
    os << "1.UP 2.DOWN 3.LEFT 4.RIGHT" << endl;
    os << "Please enter the direction: ";
    string line;
    if(getline(is, line)) {
        if (line == "1") {
            if (checkBounds(os, field, u->getRow() - 1, u->getCol())) {
                if (field.getUnit(u->getRow() - 1, u->getCol()) != NULL) {
                    field.deleteUnit(u->getRow() - 1, u->getCol());//删除单位
                    return true;
                } else {
                    os << "No unit to attack at (" << u->getRow() - 1 << ", " << u->getCol() << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow()-1 << ", " << u->getCol() << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "2") {
            if (checkBounds(os, field, u->getRow() + 1, u->getCol())) {
                if (field.getUnit(u->getRow() + 1, u->getCol()) != NULL) {
                    field.deleteUnit(u->getRow() + 1, u->getCol());//删除单位
                    return true;
                } else {
                    os << "No unit to attack at (" << u->getRow() + 1 << ", " << u->getCol() << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow()+1 << ", " << u->getCol() << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "3") {
            if (checkBounds(os, field, u->getRow(), u->getCol() - 1)) {
                if (field.getUnit(u->getRow(), u->getCol() - 1) != NULL) {
                    field.deleteUnit(u->getRow(), u->getCol() - 1);
                    return true;//删除单位
                } else {
                    os << "No unit to attack at (" << u->getRow() << ", " << u->getCol() - 1 << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow() << ", " << u->getCol()-1 << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "4") {
            if (checkBounds(os, field, u->getRow(), u->getCol() + 1)) {
                if (field.getUnit(u->getRow(), u->getCol() + 1) != NULL) {
                    field.deleteUnit(u->getRow(), u->getCol() + 1);
                    return true;//删除单位
                } else {
                    os << "No unit to attack at (" << u->getRow() << ", " << u->getCol() + 1 << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow() << ", " << u->getCol()+1 << ") is out of bound" << endl;
                return false;
            }
        }
        else {
            os << "Incorrect input!" << endl;
            return false;
        }
    }
    else {
        flag = 3;
        return false;
    }
}
bool performShoot(ostream& os, istream& is, Field& field, Unit* u, int& flag){//开始写shoot
    os << "1.UP 2.DOWN 3.LEFT 4.RIGHT" << endl;
    os << "Please enter the direction: ";
    string line;
    if(getline(is, line)) {
        if (line == "1") {
            if (checkBounds(os, field, u->getRow() - 2, u->getCol())) {
                if (field.getUnit(u->getRow() - 2, u->getCol()) != NULL) {
                    field.deleteUnit(u->getRow() - 2, u->getCol());//删除单位
                    return true;
                } else {
                    os << "No unit to attack at (" << u->getRow() - 2 << ", " << u->getCol() << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow()-2 << ", " << u->getCol() << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "2") {
            if (checkBounds(os, field, u->getRow() + 2, u->getCol())) {
                if (field.getUnit(u->getRow() + 2, u->getCol()) != NULL) {
                    field.deleteUnit(u->getRow() + 2, u->getCol());//删除单位
                    return true;
                } else {
                    os << "No unit to attack at (" << u->getRow() + 1 << ", " << u->getCol() << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow()+2 << ", " << u->getCol() << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "3") {
            if (checkBounds(os, field, u->getRow(), u->getCol() - 2)) {
                if (field.getUnit(u->getRow(), u->getCol() - 2) != NULL) {
                    field.deleteUnit(u->getRow(), u->getCol() - 2);
                    return true;//删除单位
                } else {
                    os << "No unit to attack at (" << u->getRow() << ", " << u->getCol() - 2 << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow() << ", " << u->getCol()-2 << ") is out of bound" << endl;
                return false;
            }
        } else if (line == "4") {
            if (checkBounds(os, field, u->getRow(), u->getCol() + 2)) {
                if (field.getUnit(u->getRow(), u->getCol() + 2) != NULL) {
                    field.deleteUnit(u->getRow(), u->getCol() + 2);
                    return true;//删除单位
                } else {
                    os << "No unit to attack at (" << u->getRow() << ", " << u->getCol() + 2 << ")" << endl;
                    return false;
                }
            } else {
                //os << "(" << u->getRow() << ", " << u->getCol()+2 << ") is out of bound" << endl;
                return false;
            }
        }
        else {
            os << "Incorrect input!" << endl;
            return false;
        }
    }
    else {
        flag = 3;
        return false;
    }

}
// Perform actions
bool performActions(ostream& os, istream&is, Field& field, Unit* u, int& flag_1, int& flag)//加了个flag
{
    Vector<Action> acts = collectActions(u,flag);
    if (isAllSkip(acts))
        return true;

    displayActions(os, acts);
    os << "Select your action: ";
    Action act;
    if (!queryAction(os, is, acts, act,flag)) {
        os << "Invalid action!" << endl;
        return false;
    }

    switch(act) {
    case MOVE:
        flag_1 = 1;
        return performMove(os, is, field, u,flag);
    case ATTACK:
        flag_1 = 2;
        return performAttack(os,is,field,u,flag);
    case SHOOT:
        flag_1 = 2;
        return performShoot(os,is,field,u,flag);
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