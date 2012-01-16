#include <iostream>
#include "Histogram.h"
#include "Exceptions.h"

using namespace std;

int main() {
    int xMin = 0;
    int xMax = 10;
    int nBin = 10;
    Histogram1D* h1 = new Histogram1D(xMin, xMax, nBin, "A");
    Histogram1D* h2 = new Histogram1D(xMin, xMax, nBin, "B");
    Histogram1D* h3 = new Histogram1D(xMin, xMax, nBin, "C");

    require(h1->getnBinX() == h2->getnBinX());

    for (int i = 0; i < 11; i++) {
        h1->set(i, i);
        (*h2)[i] = (12 - i) * 5;
    }

//    *h2 = *h1 * 10;
//    for (int i = 0; i < 10; i++)
//        cout << h2->get(i) << endl;

    *h3 = *h1 + *h2;

    for (int i = 0; i < 10; i++) {
        cout << (*h1)[i] << " + " << (*h2)[i] << " = " << (*h3)(i) << endl;
    }

    delete h1;
    delete h2;
    delete h3;
}
