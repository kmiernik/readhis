#include <iostream>
#include "Histogram.h"
#include "Exceptions.h"

using namespace std;

void show(Histogram1D& h) {
    cout << "Histogram " << h.gethisId() << endl;
    for (int i = 0; i < h.getnBinX(); ++i)
        cout << "[" << i << "] = " << h[i] << endl;
}

int main() {
    int xMin = 0;
    int xMax = 10;
    int nBin = 10;
    Histogram1D* h1 = new Histogram1D(xMin, xMax, nBin, "A");
    Histogram1D* h2 = new Histogram1D(xMin, xMax, nBin, "B");
    Histogram1D* h3 = new Histogram1D(xMin, xMax, nBin, "C");
    Histogram1D* h4 = new Histogram1D(xMin, xMax, nBin, "D");

    require(h1->getnBinX() == h2->getnBinX());
    
    cout << "Testing set(), [] and ()" << endl;
    for (int i = 0; i < 11; ++i) {
        h1->set(i, i);
        (*h2)[i] = -1;
        (*h3)[i] = 2;
    }
    *h4 = *h3; 

    show(*h4);

    for (int i = 0; i < 10; ++i) {
        cout << (*h1)[i] << " " << (*h2)[i] << " " << (*h3)(i) << " " << (*h4)[i] << endl;
    }
    *h4 = *h1 + *h2 - *h3 - *h4;
    cout << "*h4 = *h1 + *h2 - *h3 - *h4" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << (*h1)[i] << " " << (*h2)[i] << " " << (*h3)(i) << " " << (*h4)[i] << endl;
    }

    *h4 += *h2 + *h3*(-1);
    cout << "*h4 += *h2 + *h3*(-3)" << endl;
    for (int i = 0; i < 10; ++i) {
        cout << (*h1)[i] << " " << (*h2)[i] << " " << (*h3)(i) << " " << (*h4)[i] << endl;
    }

    delete h1;
    delete h2;
    delete h3;
    delete h4;
}
