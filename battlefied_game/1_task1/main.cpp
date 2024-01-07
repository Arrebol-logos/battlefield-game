#include <iostream>
#include <fstream>
#include "field.h"
#include "units.h"
#include "engine.h"
using namespace std;

int main() {
    /*string filename = "field.txt";
    ofstream ofs;
    ofs.open(filename.c_str());
    if (!ofs) {
        cout << "Cannot open the file: " << filename << endl;
        return -1;
    }*/
    Field *field = loadMap(cin);
    play(*field, cin, cout);
    return 0;
}

    //Field f(11, 11);

    // Set units
   /* addFootman(f, true, 2, 0);
    addFootman(f, true, 2, 1);
    addFootman(f, true, 2, 2);
    addFootman(f, true, 2, 7);

    addFootman(f, false, 7, 4);
    addFootman(f, false, 8, 3);
    addFootman(f, false, 7, 3);
    addFootman(f, false, 9, 5);


    // displayField(cout, f);

    play(f, cin, cout);

    // ofs.close();
    return 0;
}

   //所以，写法应当为：1.先生成地图 2.添加地图细节到上面 3.添加单位
   /*int main() {
       Field *field = loadMap(cin);
       play(*field,cin,cout);
       return 0;
   }*/