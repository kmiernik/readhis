#include <iostream>
#include "Histogram.h"

using namespace std;

int main() {
    int xMin = 0;
    int xMax = 10;
    int nBin = 10;
    Histogram1D* h1 = new Histogram1D(xMin, xMax, nBin, "A");
    Histogram1D* h2 = new Histogram1D(xMin, xMax, 5, "B");
    Histogram1D* h3 = new Histogram1D(xMin, xMax, nBin, "B");

    for (int i = 0; i < 10; i++)
        h1->set(i, i);

//    *h2 = *h1 * 10;
//    for (int i = 0; i < 10; i++)
//        cout << h2->get(i) << endl;

    *h3 = *h1 + *h2;
    for (int i = 0; i < 10; i++) {
        long a = h3[i];
        cout << a << endl;

    delete h1;
    delete h2;
    delete h3;
}
