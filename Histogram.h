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

/**
 *  General purpose base Histogram class.
 *  No instance of base class is intended to be created. 
 *  Look for inheriting classes.
 *  @see Histogram1D
 *  @see Histogram2D
 */
class Histogram {
    public:
        /**
         * Retruns Histogram ID.
         * @see hisId_
         */
        unsigned getHisId(){ return hisId_; }
        /**
         * Retruns Xmin.
         * @see xMin_
         */
        double   getXmin() { return xMin_; }
        /**
         * Retruns stepX.
         * @see stepX_
         */
        double   getStepX(){ return stepX_; }

        unsigned getNBinX()   { return nBinX_ / binX_; }
        unsigned getBinX() { return binX_; }

        /**
         * Pure virtual function.
         */
        virtual unsigned long getValue() = 0;
        /**
         * Pure virtual function.
         */
        virtual void setValue() = 0;
        /**
         * Return the center of the bin numbered x.
         */
        virtual double getX (const unsigned x) { 
            return xMin_ + ( double(x) + 0.5 ) * stepX_ * binX_;
        }
    
        /**
         * Returns by value raw data stored in histogram.
         * @param values a vector of returned values
         */
        void getDataRaw (vector<long>& values);
        /**
         * Pure virtual function.
         */
        virtual void setDataRaw (vector<long>& values) = 0;

        /**
         * Returns by value raw errors (uncertainities) stored in histogram.
         * @param values a vector of returned values
         */
        void getErrorsRaw (vector<double>& errors);
        /**
         * Pure virtual function.
         */
        virtual void setErrorsRaw (vector<double>& errors) = 0;

    private:
        /**
         * hisId is histogram identifier.
         */
        const unsigned hisId_;
        /**
         * Value of lower edge of first bin.
         */
        const double   xMin_;
        /**
         * Distance between lower edges of bins.
         */
        const double   stepX_;
        /**
         * Number of bins.
         */
        const unsigned nBinX_;
        /**
         * Binning value.
         */
        unsigned binX_;
        /**
         * Vector storing raw data.
         */
        vector<long>   values_;
        /**
         * Vector storing raw data errors (uncertainities).
         */
        vector<double> errors_;
};

class Histogram1D : public Histogram {
    public:
        Histogram1D (const unsigned nBinX, const double xMin, const double stepX)
                    : nBinX_(nBinX), xMin_(xMin), stepX_(stepX) {}

        void setBinX (const unsigned binX);

        virtual void setDataRaw (vector<long>& values);
        virtual void setErrorsRaw (vector<long>& values);

        virtual long getValue (const unsigned x);
        virtual void setValue (const unsigned x, const long value);
        
        double getError (const unsigned x);
        void setError (const unsigned x, const double error);
};

class Histogram2D : public Histogram {
    public:
        Histogram2D (const double xMin, const double stepX,
                     const double yMin, const double stepY)
                    : nX_(nX), xMin_(xMin), stepX_(stepX),
                      nY_(nY), yMin_(yMin), stepY_(stepY) {}
        double   getYmin() { return yMin_; }
        double   getStepY(){ return stepY_; }
        unsigned getNBinY()   { return nBinY_ / binY_; }
        unsigned getBinY() { return binY_; }

        void setBinXY (const unsigned binX, const unsigned binY);

        virtual long getValue (const unsigned x, const unsigned y);
        virtual void setValue (const unsigned x, const unsigned y,
                               const long value);

        virtual void setDataRaw (vector<long>& values);
        virtual void setErrorsRaw (vector<double>& values);

        double getY (const unsigned y);
            return yMin_ + ( double(y) + 0.5 ) * stepY_ * binY_;
        }

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
        const unsigned nBinY_;
        const double   yMin_;
        const double   stepY_;
        unsigned binY_;

};
#endif
