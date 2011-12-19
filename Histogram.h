#ifndef HHISTOGRAM
#define HHISTOGRAM

#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "hisdrr.h"
#include "drrblock.h"

using namespace std;

///Histogram structure 
/** 
 *
 */
class Histogram {
    private:
        unsigned hisDim_;
        unsigned hisId_;

        unsigned binX_;
        unsigned binY_;
        unsigned sizeX_;
        unsigned sizeY_;

    public:
        bool valid;

        vector<unsigned> x;
        vector<unsigned long> y;
        vector<double> dy;
        vector<unsigned long> z;
        vector<double> dz;

        void checkSanity();

        void setBinX(const unsigned binX);
        const unsigned getBinX();

        void setBinY(const unsigned binY);
        const unsigned getBinY();

        void setSizeX(const unsigned sizeX);
        const unsigned getSizeX();

        void setSizeY(const unsigned sizeY);
        const unsigned getSizeY();

        const unsigned getHisDim();
        const unsigned getHisId();
};

#endif
