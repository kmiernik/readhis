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
                    unsigned nBinX, string hisId);

        /**Number of Histogram dimensions is pure virtual.*/
        virtual unsigned short getDim() const = 0;
        
        //These are basic operations, no need of virutal
        double   getxMin() const;
        double   getxMax() const;
        unsigned getnBinX() const;
        string gethisId() const; 

        double getBinWidthX() const;

        /** returns middle of bin number ix */
        double getX (unsigned ix) const;

        /** returns bin number where x is located*/
        unsigned getiX (double x) const;

        /** returns low edge of bin number ix */
        double getXlow (unsigned ix) const;
        
        /** returns high edge of bin number ix */
        double getXhigh (unsigned ix) const;

        virtual long getUnder () const;
        virtual void setUnder (long underflow);
        virtual long getOver () const;
        virtual void setOver (long overflow);

        virtual long getSum () const;

        void getDataRaw (vector<long>& values) const;

        virtual void setDataRaw (const vector<long>& values);
        virtual void setDataRaw (const vector<int>& values);
        virtual void setDataRaw (const vector<unsigned>& values);
        virtual void setDataRaw (const vector<double>& values);

        virtual ~Histogram () {  }

    protected:
        double   xMin_;
        double   xMax_;
        unsigned nBinX_;
        string hisId_;

        long underflow_; 
        long overflow_;

        double binWidthX_;

        vector<long>   values_;
};

inline long     Histogram::getUnder () const { return underflow_; }
inline void     Histogram::setUnder (long underflow) { underflow_ = underflow; }
inline long     Histogram::getOver () const  { return overflow_; }
inline void     Histogram::setOver (long overflow) { overflow_ = overflow; }
inline double   Histogram::getxMin() const  { return xMin_; }
inline double   Histogram::getxMax() const { return xMax_; }
inline unsigned Histogram::getnBinX() const  { return nBinX_; }
inline string   Histogram::gethisId() const { return hisId_; }

inline double   Histogram::getBinWidthX() const { return binWidthX_ ; }

inline double   Histogram::getX (unsigned ix) const { 
    return ( double(ix) + 0.5 ) * binWidthX_ + xMin_;
}
inline double   Histogram::getXlow (unsigned ix) const { 
    return ( double(ix) ) * binWidthX_ + xMin_;
}
inline double   Histogram::getXhigh (unsigned ix) const { 
    return ( double(ix) + 1.0 ) * binWidthX_ + xMin_;
}


class Histogram1D : public Histogram {
    public:
        Histogram1D (double xMin,  double xMax,
                     unsigned nBinX, string hisId);

        unsigned short getDim() const;

        virtual void add (double x, long n = 1);
        virtual long get (unsigned ix) const;
        virtual void set (unsigned ix, long value);
        
        /** Returns rebinned histogram (and ownership to it!).
         *  Number of counts in new histogram is not guaranted to 
         *  be the same, as it calculates new counts via floating
         *  point (number of counts in new bin is proportional to area 
         *  of intersection between new and old bin. Expect +/- 1 count.*/
        Histogram1D* rebin1D (double xMin, double xMax, unsigned nBinX) const;

        virtual Histogram1D& operator=(const Histogram1D&);

        virtual Histogram1D& operator*=(int right); 
        virtual Histogram1D& operator+=(const Histogram1D& right); 
        virtual Histogram1D& operator-=(const Histogram1D& right); 

        virtual const Histogram1D operator*(int right) const;
        virtual const Histogram1D operator+(const Histogram1D& right) const; 
        virtual const Histogram1D operator-(const Histogram1D& right) const; 

        virtual long& operator[] (unsigned ix);
        virtual long  operator[] (unsigned ix) const;
        virtual long& operator() (unsigned ix);
        virtual long  operator() (unsigned ix) const;
};

class Histogram2D : public Histogram {
    public:
        Histogram2D (double xMin,    double xMax,
                     double yMin,    double yMax,
                     unsigned nBinX, unsigned nBinY,
                     string hisId);

        unsigned short getDim() const;

        inline double   getyMin() const;
        inline double   getyMax() const;
        inline unsigned getnBinY() const;
        inline double getBinWidthY() const;
        inline double getY (unsigned iy) const;
        unsigned getiY (double y) const;
        inline double getYlow (unsigned iy) const;
        inline double getYhigh (unsigned iy) const;

        virtual void add (double x, double y, long n = 1);
        virtual long get (unsigned ix, unsigned iy) const;
        virtual void set (unsigned ix, unsigned iy, long value);

        virtual void gateX (unsigned x0, unsigned x1, vector<long>& result);
        virtual void gateY (unsigned y0, unsigned y1, vector<long>& result);

        virtual void transpose ();

        Histogram2D* rebin2D (double xMin, double xMax,
                              double yMin, double yMax,
                              unsigned nBinX, unsigned nBinY) const;

        virtual Histogram2D& operator=(const Histogram2D&);

        virtual Histogram2D& operator*=(int right); 
        virtual Histogram2D& operator+=(const Histogram2D& right); 
        virtual Histogram2D& operator-=(const Histogram2D& right); 

        virtual const Histogram2D operator*(int right) const;
        virtual const Histogram2D operator+(const Histogram2D& right) const; 
        virtual const Histogram2D operator-(const Histogram2D& right) const; 

        virtual long& operator() (unsigned ix, unsigned iy);
        virtual long  operator() (unsigned ix, unsigned iy) const;

    protected:
        double   yMin_;
        double   yMax_;
        unsigned nBinY_;

        double binWidthY_;
};

inline double   Histogram2D::getyMin() const  { return yMin_; }
inline double   Histogram2D::getyMax() const  { return yMax_; }
inline unsigned Histogram2D::getnBinY() const { return nBinY_; }
inline double   Histogram2D::getBinWidthY() const { return binWidthY_ ; }
inline double   Histogram2D::getY (unsigned iy) const {
    return ( double(iy) + 0.5 ) * binWidthY_ + yMin_;
}
inline double   Histogram2D::getYlow (unsigned iy) const {
    return ( double(iy) ) * binWidthY_ + yMin_;
}
inline double   Histogram2D::getYhigh (unsigned iy) const {
    return ( double(iy) + 1.0 ) * binWidthY_ + yMin_;
}


#endif
