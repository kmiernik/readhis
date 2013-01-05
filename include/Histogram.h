/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef HISTOGRAM_H
#define HISTOGRAM_H

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
 *  @see Histogram1D
 *  @see Histogram2D
 */
class Histogram {
    public:
        /** Ctor. */
        Histogram  (double xMin,  double xMax,
                    unsigned nBinX, string hisId);

        /**Number of Histogram dimensions is pure virtual.*/
        virtual unsigned short getDim() const = 0;
        
        //These are basic operations, no need of virutal
        /** Returns xMin_ .*/
        double   getxMin() const;

        /** Returns xMax_ .*/
        double   getxMax() const;

        /** Returns nBinX_ .*/
        unsigned getnBinX() const;

        /** Returns hisId_ .*/
        string gethisId() const; 

        /** Returns binWidthX_ */
        double getBinWidthX() const;

        /** returns middle of bin number ix */
        virtual double getX (unsigned ix) const;

        /** Returns bin number where x is located. 
         * If x is outside upper bound last bin is returned.
         * If x is outside lower bound 0 bin is returned.
         */
        virtual unsigned getiX (double x) const;

        /** Returns low edge of bin number ix */
        virtual double getXlow (unsigned ix) const;
        
        /** Returns high edge of bin number ix */
        virtual double getXhigh (unsigned ix) const;

        /** Returns underflow_ */
        virtual long getUnder () const;

        /** Returns overflow_ */
        virtual long getOver () const;

        /** Returns total number of counts in histogram */
        virtual long getSum () const;

        /** Returns raw data in form of vector. */
        void getDataRaw (vector<long>& values) const;

        /** Sets vector of raw data to passed vector */
        virtual void setDataRaw (const vector<long>& values);

        /** Sets vector of raw data to passed vector. Casts int to long. */
        virtual void setDataRaw (const vector<int>& values);
        
        /** Sets vector of raw data to passed vector. Cast unsigned to long. */
        virtual void setDataRaw (const vector<unsigned>& values);

        /** Sets vector of raw data to passed vector. Cast double to long 
         * using more sophisticated rounding:
         * * number with floating point part smaller then .5 is rounded down
         * * number with floating point part larger then .5 is rounded down
         * * number with floating point equal to .5 is rounded to
         *                                          nearest even integer
         * */
        virtual void setDataRaw (const vector<double>& values);

        virtual ~Histogram () {  }

    protected:
        /** Lower edge of first bin. */
        double   xMin_;

        /** Upper edge of last bin. */
        double   xMax_;
        
        /** Number of bins in X direction.*/
        unsigned nBinX_;

        /** Histogram name. */
        string hisId_;

        /** Any count that would to go bin lower then lowest goes here.
         * In case of 2 and more dimensions if at least in one dimension count 
         * would go to lower bin underflow is incremented only. */
        long underflow_; 

        /** Any count that would go to bin higher then the last one goes
         * here. @see underflow_ .
         */
        long overflow_;

        /** Bin width in X dimension. */
        double binWidthX_;

        /** Raw data for any number of dimensions. */
        vector<long>   values_;
};

inline long     Histogram::getUnder () const { return underflow_; }
inline long     Histogram::getOver () const  { return overflow_; }
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

/**
 * One dimenstional histogram holding 'long' per bin.
 */
class Histogram1D : public Histogram {
    public:
        /** Ctor */
        Histogram1D (double xMin,  double xMax,
                     unsigned nBinX, string hisId);

        /** Overloaded pure virtual from base class. Returns 1.*/
        unsigned short getDim() const;

        /** Adds n counts to bin where x is located. Over- and
         * underflows goes to overflow_ and underflow_ fields.*/
        virtual void add (double x, long n = 1);

        /** Returns number of counts in bin number ix */
        virtual long get (unsigned ix) const;

        /** Sets number of counts in bin number ix to value.*/
        virtual void set (unsigned ix, long value);
        
        /** Returns rebinned histogram (and ownership to it!).
         *  Number of counts in a new histogram is not guaranted to 
         *  be the same, as it calculates new number of counts via floating
         *  point (number of counts in new bin is proportional to area 
         *  of intersection between new and old bin and then rounded).
         *  Expect +/- 1 count.
         *  Expect bad results especially when increasing number of bin
         *  when small number of counts are present (why would you increase
         *  number of bins then anyway). */
        Histogram1D* rebin (double xMin, double xMax, unsigned nBinX) const;

        /** Returns rebinned histogram as above, however with the new 
         * bin width specified instead of number of bins. As a result the xMax
         * of a new histograms may be different (last bin is enlarged to keep
         * its size equal to the other bins).
         * Other comments found above apply here as well.
         */
        Histogram1D* rebin (double xMin, double xMax, double binW) const;

        /** lhs lstogram will be overwritten by rhs. */
        virtual Histogram1D& operator=(const Histogram1D&);

        /** All elements of histogram will be multiplied by right integer. */
        virtual Histogram1D& operator*=(int right); 

        /** All elements of histogram will be incremented by value of rhs
         * histogram elements. Histograms nBinX, xMin and xMax must be the same. */
        virtual Histogram1D& operator+=(const Histogram1D& right); 

        /** All elements of histogram will be decremented by value of rhs
         * histogram elements. Histograms nBinX, xMin and xMax must be the same. */
        virtual Histogram1D& operator-=(const Histogram1D& right); 

        /** Returns histogram where all elements of histogram are multiplied by right. */
        virtual const Histogram1D operator*(int right) const;

        /** Returns histogram where all elements are sum of elements of
         * two histograms. Histogram nBinX, xMin, xMax must be the same. */
        virtual const Histogram1D operator+(const Histogram1D& right) const; 

        /** Returns histogram where all elements are difference of elements of
         * two histograms. Histogram nBinX, xMin, xMax must be the same. */
        virtual const Histogram1D operator-(const Histogram1D& right) const; 

        /** Access to elements by their index.*/
        virtual long& operator[] (unsigned ix);

        /** Access to elements by their index.*/
        virtual long  operator[] (unsigned ix) const;

        /** Access to elements by their index.*/
        virtual long& operator() (unsigned ix);

        /** Access to elements by their index.*/
        virtual long  operator() (unsigned ix) const;
};

/** Two dimensional histogram holding 'long' per bin.*/
class Histogram2D : public Histogram {
    public:
        /** Ctor.*/
        Histogram2D (double xMin,    double xMax,
                     double yMin,    double yMax,
                     unsigned nBinX, unsigned nBinY,
                     string hisId);

        /** Overloaded pure virutal from base class. Returns 2.*/
        unsigned short getDim() const;

        /** Returns yMin_.*/
        inline double   getyMin() const;

        /** Returns yMax_.*/
        inline double   getyMax() const;

        /** Returns nBinY_.*/
        inline unsigned getnBinY() const;

        /** Returns binWidthY_.*/
        inline double getBinWidthY() const;

        /** Returns middle of bin number iy.*/
        inline double getY (unsigned iy) const;

        /** Returns bin number where y is located. 
         * If y is outside upper bound last bin is returned.
         * If y is outside lower bound 0 bin is returned.
         */
        unsigned getiY (double y) const;

        /** Returns low edge of bin number iy */
        inline double getYlow (unsigned iy) const;

        /** Returns high edge of bin number iy */
        inline double getYhigh (unsigned iy) const;

        /** Add n counts to bin where (x,y) is located. */
        virtual void add (double x, double y, long n = 1);

        /** Returns value of bin (ix,iy). @see Histogram2D::operator() */
        virtual long get (unsigned ix, unsigned iy) const;

        /** Sets value of bin (ix,iy) to value. */
        virtual void set (unsigned ix, unsigned iy, long value);

        /** Returns projection on Y axis gated on X axis.
         * Return also ownership to projected histogram (1D).
         * Gate starts at bin containing
         * xl end on bin containing xh (including both).
         */
        virtual Histogram1D* gateX (double xl, double xh) const;

        /** Return projection on X axis. @see gateX .*/
        virtual Histogram1D* gateY (double yl, double yh) const;

        /** 10 points for guessing what this function does.
         * Yes! It transposes the 2D histogram as if it was a matrix. 
         */
        virtual void transpose ();

        /** See Histogram1D::rebin comment */
        Histogram2D* rebin (double xMin, double xMax,
                            double yMin, double yMax,
                            unsigned nBinX, unsigned nBinY) const;

        /** "Width" rebinning version, see Histogram1D::rebin comment */
        Histogram2D* rebin (double xMin, double xMax,
                            double yMin, double yMax,
                            double binWX, double binWY) const;

        /** lhs lstogram will be overwritten by rhs. */
        virtual Histogram2D& operator=(const Histogram2D&);

        /** All elements of histogram will be multiplied by right integer. */
        virtual Histogram2D& operator*=(int right); 

        /** All elements of histogram will be incremented by value of rhs
         * histogram elements.
         * Histograms nBinX, nBinY, xMin, xMax, yMin and yMax must be the same. */
        virtual Histogram2D& operator+=(const Histogram2D& right); 
        
        /** All elements of histogram will be decremented by value of rhs
         * histogram elements.
         * Histograms nBinX, nBinY, xMin, xMax, yMin and yMax must be the same. */
        virtual Histogram2D& operator-=(const Histogram2D& right); 

        /** Returns histogram where all elements of histogram are multiplied by right. */
        virtual const Histogram2D operator*(int right) const;

        /** Returns histogram where all elements are sum of elements of
         * two histograms.
         * Histograms nBinX, nBinY, xMin, xMax, yMin and yMax must be the same. */
        virtual const Histogram2D operator+(const Histogram2D& right) const; 

        /** Returns histogram where all elements are difference of elements of
         * two histograms.
         * Histograms nBinX, nBinY, xMin, xMax, yMin and yMax must be the same. */
        virtual const Histogram2D operator-(const Histogram2D& right) const; 

        /** Access to elements by their index.*/
        virtual long& operator() (unsigned ix, unsigned iy);

        /** Access to elements by their index.*/
        virtual long  operator() (unsigned ix, unsigned iy) const;

    protected:
        /** Lower edge of lowest bin in Y direction. */
        double   yMin_;

        /** High edge of highest bin in Y direction. */
        double   yMax_;

        /** Number of bins in Y direction.*/
        unsigned nBinY_;
    
        /** Bin width in Y direction.*/
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
