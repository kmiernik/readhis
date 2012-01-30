/*
 * K. Miernik (k.a.miernik@gmail.com) 2012
 *
 */

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

void show(Histogram2D& h) {
    for (unsigned i = 0; i < h.getnBinX(); ++i) {
        for (unsigned j = 0; j < h.getnBinY(); ++j)
            cout << h.getX(i) << " " << h.getY(j) << " " << h.get(i,j) << endl;
        cout << endl;
    }
    cout << "#SUM: " << h.getSum() << endl;
    cout << "#Under: " << h.getUnder() << endl;
    cout << "#Over: " << h.getOver() << endl;
}

int main(int argc, char** argv) {
    double xMin = 0;
    double xMax = 10;
    
    int nBin = 1;
    if (argc > 1)
        nBin = atoi(argv[1]);
    else {
        cout << "Argument required" << endl;
        exit(1);
    }

    debug::Counter cnt;
    Histogram1D* h11 = new Histogram1D(xMin, xMax, nBin, "A");

    cnt.add();
    vector<int> data;
    for (int i = 0; i < nBin; ++i)
        data.push_back(i);
    
    cnt.add();
    h11->setDataRaw(data);
    Histogram1D* h12 = h11->rebin(-2, 5, 12);
    //show(*h11);
    //show(*h12);
    cnt.add();

    delete h12;
    delete h11;
    
    cnt.add();
    vector<int> data2;
    for (int i = 0; i < nBin; ++i)
    for (int j = 0; j < nBin; ++j)
        data2.push_back(i*j);
//        data2.push_back(1);

    cnt.add();
    Histogram2D* h21 = new Histogram2D(xMin, xMax, xMin, xMax, nBin, nBin, "B");
    cnt.add();
    h21->setDataRaw(data2);
    cnt.add();
    Histogram2D* h22 = h21->rebin(0, 10, 0, 10, 5, 5);
    cnt.add();
    //show(*h21);
    show(*h22);
    cnt.add();

    delete h22;
    delete h21;
    cout << "#OK" << endl;
}
