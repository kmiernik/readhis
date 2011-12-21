#ifndef HHISTOGRAM
#define HHISTOGRAM

#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "HisDrr.h"
#include "DrrBlock.h"
#include "ExceptionHandlers.h"

using namespace std;

///Histogram structure 
/** 
 *
 */
class Histogram {
    public:
        unsigned getNX();

        double   getXmin();
        void     setXmin (const double xMin);
        
        double   getStepX();
        void     setStepX (const double stepX);
        
        unsigned getBinX();

        double  getX (const unsigned ix);

        void getArray (vector<double>& values);
        void setArray (vector<double>& values);

        void getErrorsArray (vector<double>& values);
        void setErrorsArray (vector<double>& values);

    private:
        const unsigned hisId_;

        unsigned nX_;
        double   xMin_;
        double   stepX_;
        unsigned binX_;
        
        vector<unsigned long> values_;
        vector<double>        errors_;
};

class Histogram1D : public Histogram {
    public:
        void     setBinX (const unsigned binX);

        unsigned long getPoint (const unsigned long x);
        void setPoint (const unsigned long x, const unsigned long value);

        void getArray (vector<double>& values);
        void setArray (vector<double>& values);

        double getError (const unsigned long x);
        void setError (const unsigned long x, const double value);

};

class Histogram2D : public Histogram {
    public:
        void     setBinXY (const unsigned binX, const unsigned binY);
        
        unsigned long getPoint (const unsigned x, const unsigned y);
        void setPoint (const unsigned x, const unsigned y, const unsigned long value);

        double  getY (const unsigned iy);

        double getError (const unsigned x, const unsigned y);
        void setError (const unsigned x, const unsigned y, const double value);

        void gateX (const unsigned x0, const unsigned x1,
                    vector<unsigned logn>& result);
        void gateY (const unsigned y0, const unsigned y1,
                    vector<unsigned logn>& result);

        void gateXbackground (const unsigned x0, const unsigned x1,
                              const unsigned b0, const unsigned b1,
                              vector<unsigned logn>& result);
        void gateYbackground (const unsigned y0, const unsigned y1,
                              const unsigned b0, const unsigned b1,
                              vector<unsigned logn>& result);

    private:
        unsigned nY_;
        double   yMin_;
        double   stepY_;
        unsigned binY_;

};
#endif
