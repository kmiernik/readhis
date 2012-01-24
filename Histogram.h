#ifndef HHISTOGRAM
#define HHISTOGRAM

#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
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
        Histogram  (double xMin,  double xMax,
                    unsigned nBinX, string& hisId)
                    : xMin_(xMin), xMax_(xMax),
                      nBinX_(nBinX), hisId_(hisId) {}

        virtual string gethisId() const = 0; 
        virtual double   getxMin() const  { return xMin_; }
        virtual double   getxMax() const { return xMax_; }
        virtual unsigned getnBinX() const  { return nBinX_; }
        virtual double getBinWidthX() const { return xMin_ - xMax_ / double(nBinX_) ; }

        virtual double getX (unsigned ix) const { 
            return ( double(ix) - 0.5 ) * getBinWidthX() + xMin_;
        }

        virtual void getDataRaw (vector<long>& values) const;
        virtual void setDataRaw (vector<long>& values);
        virtual void setDataRaw (vector<unsigned>& values);

        virtual void getErrorsRaw (vector<double>& errors) const;
        virtual void setErrorsRaw (vector<double>& errors);

        virtual ~Histogram () {  }

    protected:
        double   xMin_;
        double   xMax_;
        unsigned nBinX_;
        string hisId_;
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
        vector<double> errors_;
};

class Histogram1D : public Histogram {
    public:
        Histogram1D (double xMin,  double xMax,
                     unsigned nBinX, string& hisId)
                    : Histogram(xMin, xMax, nBinX, hisId) 
                    { 
                        values_.resize( nBinX_ + 2, 0);
                        errors_.resize( nBinX_ + 2, 0.0);
                    }

        string gethisId() const { return hisId_; }

        virtual void add (double x, long n = 1);
        virtual long get (unsigned ix);
        virtual void set (unsigned ix, long value);
        
        double getError (unsigned ix);
        void setError (unsigned ix, double error);

        void rebin1D (Histogram1D* rebinned);

        Histogram1D& operator=(const Histogram1D&);

        Histogram1D& operator*=(int right); 
        Histogram1D& operator+=(const Histogram1D& right); 
        Histogram1D& operator-=(const Histogram1D& right); 

        const Histogram1D operator*(int right) const;
        const Histogram1D operator+(const Histogram1D& right) const; 
        const Histogram1D operator-(const Histogram1D& right) const; 

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
                     string hisId)
                    : Histogram(xMin, xMax, nBinX, hisId),
                      yMin_(yMin), yMax_(yMax), nBinY_(nBinY)
                    { 
                        values_.resize( (nBinX_ + 2) * (nBinY_ + 2), 0);
                        errors_.resize( (nBinX_ + 2) * (nBinY_ + 2), 0.0 );
                    }

        virtual string gethisId() const { return hisId_; }

        double   getyMin()  { return yMin_; }
        double   getyMax()  { return yMax_; }
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

        Histogram2D& operator=(const Histogram2D&);

        Histogram2D& operator*=(int right); 
        Histogram2D& operator+=(const Histogram2D& right); 
        Histogram2D& operator-=(const Histogram2D& right); 

        const Histogram2D operator*(int right) const;
        const Histogram2D operator+(const Histogram2D& right) const; 
        const Histogram2D operator-(const Histogram2D& right) const; 

        long& operator() (unsigned ix, unsigned iy);
        long  operator() (unsigned ix, unsigned iy) const;

    private:
        double   yMin_;
        double   yMax_;
        unsigned nBinY_;

};
#endif
