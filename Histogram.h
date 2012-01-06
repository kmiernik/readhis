#ifndef HHISTOGRAM
#define HHISTOGRAM

#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "HisDrr.h"
#include "DrrBlock.h"
#include "Exceptions.h"

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
        const char* getHisId(){ return hisId_; }

        /**
         * Retruns Xmin.
         * @see xMin_
         */
        double   getXmin() { return xMin_; }

        /**
         * Retruns Xmax.
         * @see xMin_
         */
        double   getXmax() { return xMax_; }

        /**
         * Returns number of bins in x direction.
         */
        unsigned getnBinX()   { return nBinX_; }

        /**
         * Retruns bin width.
         * @see stepX_
         */
        double getBinWidthX(){ return xMin_ - xMax_ / double(nBinX_) ; }

        /**
         * Returns the center of the bin numbered ix.
         */
        double getX (const unsigned ix) {
            return ( double(ix) - 0.5 ) * getBinWidthX() + xMin_;
        }

        /**
         * Pure virtual function.
         */
        virtual void add () = 0;

        /**
         * Pure virtual function.
         */
        virtual long get () = 0;

        /**
         * Pure virtual function.
         */
        virtual void set () = 0;

        /**
         * Returns by value raw data stored in histogram.
         * @param values a vector of returned values
         */
        void getDataRaw (vector<long>& values);

        /**
         * Sets whole vector of data. Data are 'raw' so user must
         * take care of under and overshooted bins.
         *
         */
        virtual void setDataRaw (vector<long>& values);

        /**
         * Returns by value raw errors (uncertainities) stored in histogram.
         * @param values a vector of returned values
         */
        void getErrorsRaw (vector<double>& errors);

        /**
         * Sets whole vector of errors. Data are 'raw' so user must
         * take care of under and overshooted bins.
         *
         */
        virtual void setErrorsRaw (vector<double>& errors);

        Histogram  (const double xMin,  const double xMax,
                    const unsigned nBinX, const char* hisId)
                    : xMin_(xMin), xMax_(xMax),
                      nBinX_(nBinX), hisId_(hisId) {}

    protected:
        /**
         * Value of lower edge of first bin.
         */
        const double   xMin_;
        
        /**
         * Value of upper edge of last bin.
         */
        const double   xMax_;

        /**
         * Number of bins in X direction.
         */
        const unsigned nBinX_;

        /**
         * hisId is histogram identifier/name.
         */
        const char* hisId_;

        /**
         * Vector storing raw data. Element [0] stores undershoots, 
         * element [nBinX+1] stores overshoots, elements [1-nBinX] store
         * "normal" data, total lenght (for 1D) is nBinX+2.
         * For 2D elements [1-nBinX] stores data for y = 0 (undershoots)
         *        elements [nBinX+2 - 2*(nBinX+2)-1] stores y = 1
         *        e.g nBinX = 4, nBinY = 2
         *        [0  1  2  3  4    5 ]
         *            ------------
         *        [6  |7  8  9  10| 11]
         *        [12 |13 14 15 16| 17]
         *            -------------
         *        [18 19 20 21 22 23]
         * real data is in frame, the rest are overshoot/undershoots bins
         */
        vector<long>   values_;

        /**
         * Vector storing raw data errors (uncertainities).
         */
        vector<double> errors_;
};

class Histogram1D : public Histogram {
    public:
        Histogram1D (const double xMin,  const double xMax,
                     const unsigned nBinX, const char* hisId)
                    : Histogram(xMin, xMax, nBinX, hisId) 
                    { 
                        values_.resize( nBinX_ + 2, 0);
                        errors_.resize( nBinX_ + 2, 0.0);
                    }

        virtual void setDataRaw (vector<long>& values);
        virtual void setErrorsRaw (vector<long>& values);
    
        virtual void add (const double x, const long n = 1);
        virtual long get (const unsigned ix);
        virtual void set (const unsigned ix, const long value);
        
        double getError (const unsigned ix);
        void setError (const unsigned ix, const double error);

        void rebin1D (Histogram1D* rebinned);
};

class Histogram2D : public Histogram {
    public:
        Histogram2D (const double xMin,    const double xMax,
                     const double yMin,    const double yMax,
                     const unsigned nBinX, const unsigned nBinY,
                     const char* hisId)
                    : Histogram(xMin, xMax, nBinX, hisId),
                      yMin_(yMin), yMax_(yMax), nBinY_(nBinY)
                    { 
                        values_.resize( (nBinX_ + 2) * (nBinY_ + 2), 0);
                        errors_.resize( (nBinX_ + 2) * (nBinY_ + 2), 0.0 );
                    }

        double   getYmin()  { return yMin_; }
        double   getYmax()  { return yMax_; }
        unsigned getnBinY() { return nBinY_; }

        double getBinWidthY(){ return yMin_ - yMax_ / double(nBinY_) ; }

        virtual void add (const double x, const double y, const long n = 1);
        virtual long get (const unsigned ix, const unsigned iy);
        virtual void set (const unsigned ix, const unsigned iy,
                          const long value);
        
        double getError (const unsigned ix, const unsigned iy);
        void setError (const unsigned ix, const unsigned iy,
                       const double error);

        virtual void setDataRaw (vector<long>& values);
        virtual void setErrorsRaw (vector<double>& values);

        double getY (const unsigned iy) {
            return ( double(iy) - 0.5 ) * getBinWidthY() + yMin_;
        }

        void gateX (const unsigned x0, const unsigned x1,
                    vector<long>& result);
        void gateY (const unsigned y0, const unsigned y1,
                    vector<long>& result);

        void gateXbackground (const unsigned x0, const unsigned x1,
                              const unsigned b0, const unsigned b1,
                              vector<long>& result,
                              vector<double>& resultErrors);
        void gateYbackground (const unsigned y0, const unsigned y1,
                              const unsigned b0, const unsigned b1,
                              vector<long>& result,
                              vector<double>& resultErrors);
        void rebin2D (Histogram2D* rebinned);

    private:
        const double   yMin_;
        const double   yMax_;
        const unsigned nBinY_;

};
#endif
