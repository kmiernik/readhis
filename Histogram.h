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

        //Number of Histogram dimensions is pure virtual
        virtual unsigned short getDim() const = 0;
        
        //These are basic operations, no need of virutal
        inline double   getxMin() const;
        inline double   getxMax() const;
        inline unsigned getnBinX() const;
        inline string gethisId() const; 

        inline double getBinWidthX() const;
        inline double getX (unsigned ix) const;

        inline virtual long getUnder () const;
        inline virtual long getOver () const;

        void getDataRaw (vector<long>& values) const;
        virtual void setDataRaw (vector<long>& values);
        virtual void setDataRaw (vector<unsigned>& values);

        void getErrorsRaw (vector<double>& errors) const;
        virtual void setErrorsRaw (vector<double>& errors);

        virtual ~Histogram () {  }

    protected:
        double   xMin_;
        double   xMax_;
        unsigned nBinX_;
        string hisId_;

        long underflow; 
        long overflow;

        vector<long>   values_;
        vector<double> errors_;
};

inline long Histogram::getUnder () const { return underflow; }
inline long Histogram::getOver () const  { return overflow; }
inline double   Histogram::getxMin() const  { return xMin_; }
inline double   Histogram::getxMax() const { return xMax_; }
inline unsigned Histogram::getnBinX() const  { return nBinX_; }
inline string          Histogram::gethisId() const { return hisId_; }

inline double   Histogram::getBinWidthX() const { 
    return (xMax_ - xMin_) / double(nBinX_) ;
}
inline double   Histogram::getX (unsigned ix) const { 
    return ( double(ix) + 0.5 ) * getBinWidthX() + xMin_;
}


class Histogram1D : public Histogram {
    public:
        Histogram1D (double xMin,  double xMax,
                     unsigned nBinX, string hisId);

        unsigned short getDim() const;

        virtual void add (double x, long n = 1);
        virtual long get (unsigned ix);
        virtual void set (unsigned ix, long value);
        
        virtual double getError (unsigned ix);
        virtual void setError (unsigned ix, double error);

        virtual void rebin1D (Histogram1D* rebinned);

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

        virtual void add (double x, double y, long n = 1);
        virtual long get (unsigned ix, unsigned iy) const;
        virtual void set (unsigned ix, unsigned iy, long value);
        
        virtual double getError (unsigned ix, unsigned iy) const;
        virtual void setError (unsigned ix, unsigned iy, double error);

        virtual void gateX (unsigned x0, unsigned x1, vector<long>& result);
        virtual void gateY (unsigned y0, unsigned y1, vector<long>& result);

        virtual void gateXbackground (unsigned x0, unsigned x1,
                                      unsigned b0, unsigned b1,
                                      vector<long>& result,
                                      vector<double>& resultErrors);
        virtual void gateYbackground (unsigned y0, unsigned y1,
                                      unsigned b0, unsigned b1,
                                      vector<long>& result,
                                      vector<double>& resultErrors);

        virtual void rebin2D (Histogram2D* rebinned);

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

};

inline double   Histogram2D::getyMin() const  { return yMin_; }
inline double   Histogram2D::getyMax() const  { return yMax_; }
inline unsigned Histogram2D::getnBinY() const { return nBinY_; }
inline double   Histogram2D::getBinWidthY() const { 
    return (yMax_ - yMin_) / double(nBinY_) ;
}
inline double Histogram2D::getY (unsigned iy) const {
    return ( double(iy) + 0.5 ) * getBinWidthY() + yMin_;
}


#endif
