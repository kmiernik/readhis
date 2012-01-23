#include <iostream>
#include <cstdlib>
#include <string>
#include "hisdrr.h"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "createhis input output" << endl;
        cout << "where input is file name with extension, and output WITHOUT extension " << endl;
        exit(1);
    }
    
    string input = argv[1];
    string base = argv[2];
    string drr = base + ".drr";
    string his = base + ".his";
    cout << input << " " << drr << " " << his << endl;

    try {

        HisDrr *h = new HisDrr(drr, his, input);
        delete h;

    } catch (IOError &err) {
        cout << "I/O Error: " << err.show() << endl;
    } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
    }
}
