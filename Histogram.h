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
        double   getStepX();
        unsigned getBinX();

        virtual unsigned long getValue() = 0;
        virtual void setValue() = 0;

        void getDataRaw (vector<double>& values);
        void setDataRaw (vector<double>& values);

        void getErrorsRaw (vector<double>& values);
        void setErrorsRaw (vector<double>& values);

    private:
        const unsigned hisId_;

        const unsigned nX_;
        const double   xMin_;
        const double   stepX_;
        unsigned binX_;
        
        vector<unsigned long> values_;
        vector<double>        errors_;
};

class Histogram1D : public Histogram {
    public:
        Histogram1D (const unsigned nX, const double xMin, const double stepX)
                    : nX_(nX), xMin_(xMin), stepX_(stepX) {}

        void setBinX (const unsigned binX);

        virtual unsigned long getValue (const unsigned x);
        virtual void setValue (const unsigned x, const unsigned value);

        double getError (const unsigned x);
        void setError (const unsigned x, const double value);
};

class Histogram2D : public Histogram {
    public:
        Histogram2D (const unsigned nX, const double xMin, const double stepX,
                     const unsigned nY, const double yMin, const double stepY)
                    : nX_(nX), xMin_(xMin), stepX_(stepX),
                      nY_(nY), yMin_(yMin), stepY_(stepY) {}

        void     setBinXY (const unsigned binX, const unsigned binY);
        
        virtual unsigned long getValue (const unsigned x, const unsinged y);
        virtual void setValue (const unsigned x, const unsinged y, const unsigned long value);

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
        const unsigned nY_;
        const double   yMin_;
        const double   stepY_;
        unsigned binY_;

};
#endif
