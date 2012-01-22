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
        const char* getHisId() const { return hisId_; }

        /**
         * Retruns Xmin.
         * @see xMin_
         */
        double   getXmin() const  { return xMin_; }

        /**
         * Retruns Xmax.
         * @see xMin_
         */
        double   getXmax() const { return xMax_; }

        /**
         * Returns number of bins in x direction.
         */
        unsigned getnBinX() const  { return nBinX_; }

        /**
         * Retruns bin width.
         * @see stepX_
         */
        double getBinWidthX() const { return xMin_ - xMax_ / double(nBinX_) ; }

        /**
         * Returns the center of the bin numbered ix.
         */
        double getX (unsigned ix) const { 
            return ( double(ix) - 0.5 ) * getBinWidthX() + xMin_;
        }

        /**
         * Returns by value raw data stored in histogram.
         * @param values a vector of returned values
         */
        virtual void getDataRaw (vector<long>& values) const;

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
        virtual void getErrorsRaw (vector<double>& errors) const;

        /**
         * Sets whole vector of errors. Data are 'raw' so user must
         * take care of under and overshooted bins.
         *
         */
        virtual void setErrorsRaw (vector<double>& errors);

        Histogram  (double xMin,  double xMax,
                    unsigned nBinX, const char* hisId)
                    : xMin_(xMin), xMax_(xMax),
                      nBinX_(nBinX), hisId_(hisId) {}
        virtual ~Histogram () {  }

    protected:
        /**
         * Value of lower edge of first bin.
         */
        double   xMin_;
        
        /**
         * Value of upper edge of last bin.
         */
        double   xMax_;

        /**
         * Number of bins in X direction.
         */
        unsigned nBinX_;

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
        Histogram1D (double xMin,  double xMax,
                     unsigned nBinX, const char* hisId)
                    : Histogram(xMin, xMax, nBinX, hisId) 
                    { 
                        values_.resize( nBinX_ + 2, 0);
                        errors_.resize( nBinX_ + 2, 0.0);
                    }

        virtual void add (double x, long n = 1);
        virtual long get (unsigned ix);
        virtual void set (unsigned ix, long value);
        
        double getError (unsigned ix);
        void setError (unsigned ix, double error);

        void rebin1D (Histogram1D* rebinned);

        friend const Histogram1D operator*(const Histogram1D& left,
                                           int right);
//        friend const Histogram1D operator+(const Histogram1D& left, 
//                                           const Histogram1D& right); 
        Histogram1D operator+(const Histogram1D& right) const; 
        Histogram1D operator-(const Histogram1D& right) const; 
//        friend const Histogram1D operator-(const Histogram1D& left, 
//                                           const Histogram1D& right); 

        Histogram1D& operator+=(const Histogram1D& right); 
        Histogram1D& operator-=(const Histogram1D& right); 

        long& operator[] (unsigned ix);
        long  operator[] (unsigned ix) const;
        long& operator() (unsigned ix);
        long  operator() (unsigned ix) const;
};

class Histogram2D : public Histogram {
    public:
        Histogram2D (double xMin,    double xMax,
                     double yMin,    double yMax,
                     unsigned nBinX, unsigned nBinY,
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

        virtual void add (double x, double y, long n = 1);
        virtual long get (unsigned ix, unsigned iy);
        virtual void set (unsigned ix, unsigned iy, long value);
        
        double getError (unsigned ix, unsigned iy);
        void setError (unsigned ix, unsigned iy, double error);

        double getY (unsigned iy) {
            return ( double(iy) - 0.5 ) * getBinWidthY() + yMin_;
        }

        void gateX (unsigned x0, unsigned x1, vector<long>& result);
        void gateY (unsigned y0, unsigned y1, vector<long>& result);

        void gateXbackground (unsigned x0, unsigned x1,
                              unsigned b0, unsigned b1,
                              vector<long>& result,
                              vector<double>& resultErrors);
        void gateYbackground (unsigned y0, unsigned y1,
                              unsigned b0, unsigned b1,
                              vector<long>& result,
                              vector<double>& resultErrors);
        void rebin2D (Histogram2D* rebinned);

        friend const Histogram2D operator*(const Histogram2D& left,
                                  int right);
        friend const Histogram2D operator+(const Histogram2D& left,
                                           const Histogram2D& right); 
        friend const Histogram2D operator+=(Histogram2D& left,
                                           const Histogram2D& right); 
        friend const Histogram2D operator-=(Histogram2D& left,
                                           const Histogram2D& right); 
        friend const Histogram2D operator-(const Histogram2D& left,
                                           const Histogram2D& right); 
        long& operator() (unsigned ix, unsigned iy);
        long  operator() (unsigned ix, unsigned iy) const;

    private:
        double   yMin_;
        double   yMax_;
        unsigned nBinY_;

};
#endif
