#include <iostream>
#include <string>
#include "Histogram.h"
#include "Exceptions.h"

using namespace std;

void show(Histogram1D& h) {
    cout << "#under" << endl;
    cout << h.getX(0) - h.getBinWidthX() << " " << h.getUnder() << endl;
    for (unsigned i = 0; i < h.getnBinX(); ++i)
        cout << h.getX(i) << " " << h[i] << endl;
    cout << "#over" << endl;
    cout << h.getX(h.getnBinX()) << " " << h.getOver() << endl;
    cout << "#SUM: " << h.getSum() << endl;
}

int main() {
    double xMin = 0;
    double xMax = 10;
    int nBin = 100;
    Histogram1D* h1 = new Histogram1D(xMin, xMax, nBin, "A");
    vector<int> data;
    for (int i = 0; i < nBin; ++i)
        data.push_back(i);
    
    h1->setDataRaw(data);
    Histogram1D* h2 = h1->rebin1D(-2, 5, 12);
    show(*h1);

    delete h2;
    delete h1;
}
