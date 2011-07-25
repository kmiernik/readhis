/**
 * Implementation of Spectrum class
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "drrblock.h"
using namespace std;

void readBlock(ifstream& file, drrBlock *block) {
    if (file.good())
        file.read((char*)block, sizeof(*block));
}

	
//FileMeasure.append(".txt");
//char *fileName = (char*)FileMeasure.c_str();

int main(int argc, char* argv[]) {
    ifstream drrIn(argv[1]);

    drrBlock block;

    if (drrIn.good()) 
        readBlock(drrIn, &block);

    int nHis = block.header.nHis;

    for (int i = 0; i < nHis; i++) 
        if (drrIn.good()) 
            readBlock(drrIn, &block);
    int i = 1;
    cout << "N = " << nHis << endl;
    while (drrIn.good()) {
        int cos;
        drrIn.read((char*)&cos, sizeof(cos));
        if (i%32 == 0)
            cout << i << ":" << cos << endl;
        else
            cout << i << ":" << cos << ", ";
        i++;
    }
    cout << endl;
}

