#include <cmath>
#include <string>
#include <vector>
#include "Histogram.h"
#include "Exceptions.h"

//
//****************************************************  class  Histogram
//

Histogram::Histogram (double xMin,  double xMax,
                    unsigned nBinX, string hisId)
                    : xMin_(xMin), xMax_(xMax),
                      nBinX_(nBinX), hisId_(hisId) {
    if (nBinX < 1)
        throw GenError("Histogram(): number of bins cannot be less then 1");
    underflow_ = 0;
    overflow_ = 0;
    binWidthX_ = (xMax - xMin) / double(nBinX);
}

unsigned Histogram::getiX (double x) const { 
    if ( x > xMin_ && x < xMax_ ) 
        return (unsigned)( (x - xMin_) / binWidthX_ );
    else if (x < xMin_)
        return 0;
    else
        return nBinX_ - 1;
}

long Histogram::getSum () const {
    long sum = 0;
    unsigned sz = values_.size();

    for (unsigned i = 0; i < sz; ++i)
        sum += values_[i];

    return sum;
}

void Histogram::getDataRaw (vector<long>& values) const  {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; ++i)
        values.push_back(values_[i]);
}

void Histogram::setDataRaw (const vector<long>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = values[i];

    for (; i < sz; ++i)
        values_[i] = 0;
}

void Histogram::setDataRaw (const vector<int>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = static_cast<int>(values[i]);

    for (; i < sz; ++i)
        values_[i] = 0;
}

void Histogram::setDataRaw (const vector<unsigned>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = static_cast<long>(values[i]);

    for (; i < sz; ++i)
        values_[i] = 0;
}

void Histogram::setDataRaw (const vector<double>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    // Proper rounding:
    // if number ends with 5 it's rounded to nearest even
    // e.g 1.5 -> 2
    //     2.5 -> 2
    //     3.5 -> 4
    for (; i < szLow; ++i) {
        //e.g 1.5 + 0.5 = 2.0
        //e.g 2.5 + 0.5 = 3.0
        double val = values[i] + 0.5;
        double ip;
        double fp;
        fp = modf(val, &ip);
        // 2 % 2 = 0 so 1.5 stays as 2.0 and goes to int=2
        // 3 % 2 = 1 so 2.5 is now 2.9 and goes to int=2
        if (fp == 0.0 && int(ip) % 2 == 1)
                val -= 0.1;
        
        values_[i] = static_cast<long>(val);
    }

    for (; i < sz; ++i)
        values_[i] = 0;
}
    
//
//****************************************************  class  Histogram1D
//

Histogram1D::Histogram1D (double xMin,  double xMax,
                          unsigned nBinX, string hisId)
                          : Histogram(xMin, xMax, nBinX, hisId) {
    values_.resize( nBinX_ , 0);
}

unsigned short Histogram1D::getDim() const {
    return 1;
}

void Histogram1D::add (double x, long n /* = 1*/) {
    unsigned ix = 0;
    if ( x > xMin_ ) {
        ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() );
        if ( ix < nBinX_)
            values_[ix] += n;
        else
            ++overflow_;
    } else {
        ++underflow_;
    }

}

long Histogram1D::get (unsigned ix) const {
    if (ix < nBinX_ )
        return values_[ix];
    else
        throw ArrayError("Histogram1D::set Matrix subscript out of bounds"); 
}

void Histogram1D::set (unsigned ix, long value) {
    if (ix < nBinX_ )
        values_[ix] = value;
    else
        throw ArrayError("Histogram1D::set Matrix subscript out of bounds"); 
}
        
Histogram1D* Histogram1D::rebin1D (double xMin, double xMax,
                                   unsigned nBinX) const {
    
    if (nBinX < 1 )
        throw GenError("rebin1D: number of bins cannot be less then 1");

    double binW = (xMax - xMin) / double(nBinX);   
    double underflow = 0.0;
    double overflow = 0.0;
    unsigned sz = values_.size();
    vector<double> values;
    values.resize(nBinX, 0.0);
//    cout << "#BP21: " << sz << endl;
    
    for (unsigned i = 0; i < sz; i++) {
//        cout << "#BP21." << i << endl;
        // We find low and high edge of old bin
        // we find which new bins it goes
        // each new bin will get number of counts proportional
        // to area of old bin fitting into new one
        
        // Auxillary points to calculate area per new bin:
        // xlow, bin0_high, bin1_high, ..., xhigh
        // each bin gets
        // bin0 -> (bin0h - xlow) / dx, 
        // bin1 -> (bin1h - bin0h) / dx,
        // ...
        
        // auxillary points
        vector<double> p;
        p.push_back(getXlow(i));

        int b0 = (getXlow(i) - xMin) / binW;
        int b1 = (getXhigh(i) - xMin) / binW;
        for (int b = b0; b < b1; ++b)
            p.push_back( (b + 1) * binW + xMin );

        p.push_back(getXhigh(i));
//        cout << "#BP21." << i << "b :" << b0 << " " << b1 << endl;

        // p contains at least two points (xlow, xhigh)
        for (unsigned j = 0; j < p.size() - 1; ++j) {
//            cout << "#BP21." << i << "." << j << endl;
            double area = (p[j+1] - p[j]) / binWidthX_ * get(i);
            int ix = b0 + j;
//            cout << "#BP21." << i << "." << j << "b: " << ix << endl;
            if (ix < 0)
                underflow += area;
            else if (ix > (int)(nBinX - 1))
                overflow += area;
            else 
                values[ix] += area;
//            cout << "#BP21." << i << "." << j << "c: " << area << " " << (int)(nBinX-1) << endl;
        }
//            cout << "#BP21.d" << endl;
    }

//    cout << "#BP22" << endl;
    Histogram1D* rebinned = new Histogram1D(xMin, xMax, nBinX, "");
    rebinned->setDataRaw(values);
    rebinned->setUnder(underflow);
    rebinned->setOver(overflow);

    return rebinned;
}

Histogram1D& Histogram1D::operator=(const Histogram1D& right){
    // Self assigment test
    if (this == &right)
        return *this;

    this->xMin_ = right.xMin_;
    this->xMax_ = right.xMax_;
    this->nBinX_= right.nBinX_;
    this->hisId_ = right.hisId_;
    this->values_ = right.values_;
    this->binWidthX_ = right.binWidthX_;

    return *this;
}

Histogram1D& Histogram1D::operator*=(int right) {
    unsigned sz = this->values_.size();
    for (unsigned i = 0; i < sz; ++i)
        this->values_[i] *= right;
    return *this;
}

Histogram1D& Histogram1D::operator+=(const Histogram1D& right) {
    if (this->xMin_ == right.xMin_ &&
        this->xMax_ == right.xMax_ &&
        this->nBinX_ == right.nBinX_) {
       
        unsigned sz = this->values_.size();
        for (unsigned i = 0; i < sz; ++i)
            this->values_[i] += right.values_[i];
        return *this;
    } else {
        throw GenError("Histogram1D::operator +=: histograms of different sizes"); 
    }
}

Histogram1D& Histogram1D::operator-=(const Histogram1D& right) {
    if (this->xMin_ == right.xMin_ &&
        this->xMax_ == right.xMax_ &&
        this->nBinX_ == right.nBinX_) {
       
        unsigned sz = this->values_.size();
        for (unsigned i = 0; i < sz; ++i)
            this->values_[i] -= right.values_[i];
        return *this;
    } else {
        throw GenError("Histogram1D::operator -=: histograms of different sizes"); 
    }
}

const Histogram1D Histogram1D::operator*(int right) const {
    Histogram1D result = *this;
    result *= right;
    return result;
}


const Histogram1D Histogram1D::operator+(const Histogram1D& right) const {
    Histogram1D result = *this;
    result += right;
    return result;
}

const Histogram1D Histogram1D::operator-(const Histogram1D& right) const {
    Histogram1D result = *this;
    result -= right;
    return result;
}

long& Histogram1D::operator[](unsigned ix) {
   if (ix < nBinX_ )
     return values_[ix];
   else
     throw ArrayError("&Histogram1D::operator[]: Matrix subscript out of bounds");
}
 
long Histogram1D::operator[](unsigned ix) const {
   if (ix < nBinX_ )
     return values_[ix];
   else
     throw ArrayError("Histogram1D::operator[]: Matrix subscript out of bounds");
}

long& Histogram1D::operator()(unsigned ix) {
   if (ix < nBinX_)
     return values_[ix];
   else
     throw ArrayError("&Histogram1D::operator(): Matrix subscript out of bounds");
}
 
long Histogram1D::operator()(unsigned ix) const {
   if (ix < nBinX_ )
     return values_[ix];
   else
     throw ArrayError("Histogram1D::operator(): Matrix subscript out of bounds");
}

//
//****************************************************  class  Histogram2D
//

Histogram2D::Histogram2D (double xMin,    double xMax,
                          double yMin,    double yMax,
                          unsigned nBinX, unsigned nBinY,
                          string hisId)
                        : Histogram(xMin, xMax, nBinX, hisId),
                          yMin_(yMin), yMax_(yMax), nBinY_(nBinY) { 
    values_.resize( (nBinX_ ) * (nBinY_ ), 0);
    binWidthY_ = (yMax_ - yMin_) / double(nBinY_) ;
}

unsigned short Histogram2D::getDim() const {
    return 2;
}

void Histogram2D::add (double x, double y, long n /* = 1*/) {
    unsigned ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() );
    unsigned iy = static_cast<unsigned>( (y - yMin_) / getBinWidthY() );
    
    //Underflow first (if one coordinate is below min, and second over max, count goes to underflow)
    if (x < xMin_ || y < yMin_)
        ++underflow_;
    else if (x > xMax_ || y > yMax_)
        ++overflow_;
    else
        values_[iy * nBinX_  + ix] += n;
}

long Histogram2D::get (unsigned ix, unsigned iy) const {
    return values_[iy * (nBinX_ ) + ix];
}

void Histogram2D::set (unsigned ix, unsigned iy, long value) {
    if (ix < nBinX_  && iy < nBinY_ )
        values_[iy * nBinX_ + ix] = value;
    else
        throw ArrayError("Histogram2D::set Matrix subscript out of bounds"); 
}

unsigned Histogram2D::getiY (double y) const { 
    if ( y > yMin_ && y < yMax_ ) 
        return (unsigned)( (y - yMin_) / binWidthY_ );
    else if (y < yMin_)
        return 0;
    else
        return nBinY_ - 1;
}

Histogram1D* Histogram2D::gateX (double xl, double xh) const {
    
    vector<long> result;
    result.resize(nBinY_, 0);

    unsigned x0 = getiX(xl);
    unsigned x1 = getiX(xh);

    for (unsigned ix = x0; ix < x1 + 1; ++ix) 
        for (unsigned iy = 0; iy < nBinY_ ; ++iy) 
            result[iy] += values_[iy * nBinX_  + ix];

    Histogram1D* gate = new Histogram1D(yMin_, yMax_, nBinY_, "");
    gate->setDataRaw(result);
    return gate;

}

Histogram1D* Histogram2D::gateY (double yl, double yh) const {

    vector<long> result;
    result.resize(nBinX_, 0);

    unsigned y0 = getiY(yl);
    unsigned y1 = getiY(yh);

    for (unsigned iy = y0; iy < y1 + 1; ++iy) 
        for (unsigned ix = 0; ix < nBinX_ ; ++ix) 
            result[ix] += values_[iy * nBinX_  + ix];

    Histogram1D* gate = new Histogram1D(xMin_, xMax_, nBinX_, "");
    gate->setDataRaw(result);
    return gate;
}

void Histogram2D::transpose () {
    vector<long> transposed;
    transposed.reserve(values_.size());

    for (unsigned x = 0; x < nBinX_; ++x)
        for (unsigned y = 0; y < nBinY_; ++y)
            transposed.push_back((*this)(x,y));

    values_.swap(transposed);
    double   yMin = yMin_;
    double   yMax = yMax_;
    unsigned nBinY = nBinY_;

    yMin_ = xMin_;
    yMax_ = xMax_;
    nBinY_ = nBinX_;

    xMin_ = yMin;
    xMax_ = yMax;
    nBinX_ = nBinY;
}

Histogram2D* Histogram2D::rebin2D ( double xMin, double xMax,
                                    double yMin, double yMax,
                                    unsigned nBinX, unsigned nBinY) const {
    
    if (nBinX < 1 || nBinY < 1)
        throw GenError("rebin2D: number of bins cannot be less then 1");
    double binWX = (xMax - xMin) / double(nBinX);   
    double binWY = (yMax - yMin) / double(nBinY);   

    double underflow = 0;
    double overflow = 0;

    unsigned sz = values_.size();
    vector<double> values;
    values.resize(nBinX * nBinY, 0.0);

    for (unsigned i = 0; i < sz; ++i) {
        vector<double> px;
        vector<double> py;
        unsigned x = i % nBinX_;
        unsigned y = i / nBinX_;

        px.push_back(getXlow(x));
        py.push_back(getYlow(y));

        int bx0 = (getXlow(x) - xMin) / binWX;
        int by0 = (getYlow(y) - yMin) / binWY;
        int bx1 = (getXhigh(x) - xMin) / binWX;
        int by1 = (getYhigh(y) - yMin) / binWY;
        for (int b = bx0; b < bx1; ++b)
            px.push_back( (b + 1) * binWX + xMin );

        for (int b = by0; b < by1; ++b)
            py.push_back( (b + 1) * binWY + yMin );

        px.push_back(getXhigh(x));
        py.push_back(getYhigh(y));

        for (unsigned j = 0; j < px.size() - 1; ++j) {
            for (unsigned k = 0; k < py.size() - 1; ++k) {
                double area = (px[j+1] - px[j]) / binWidthX_ 
                            * (py[k+1] - py[k]) / binWidthY_ * values_[i];
                
                int ix = bx0 + j;
                int iy = by0 + k;
                if (ix < 0 || iy < 0)
                    underflow += area;
                else if (ix > (int)(nBinX - 1) || iy > (int)(nBinY - 1))
                    overflow += area;
                else
                    values[iy * (nBinX ) + ix] += area;
            }
        }
    }

    Histogram2D* rebinned = new Histogram2D(xMin, xMax, yMin, yMax, nBinX, nBinY, "");
    rebinned->setDataRaw(values);
    rebinned->setUnder(underflow);
    rebinned->setOver(overflow);

//    double sum = 0.0;
//    for (unsigned i = 0; i < values.size(); i++)
//        sum += values[i];
//    cout << "#sum before rounding: " << sum << endl;

    return rebinned;
}

Histogram2D& Histogram2D::operator=(const Histogram2D& right){
    // Self assigment test
    if (this == &right)
        return *this;

    this->xMin_ = right.xMin_;
    this->xMax_ = right.xMax_;
    this->nBinX_= right.nBinX_;
    this->binWidthX_ = right.binWidthX_;

    this->yMin_ = right.xMin_;
    this->yMax_ = right.xMax_;
    this->nBinY_= right.nBinX_;
    this->binWidthY_ = right.binWidthY_;

    this->hisId_ = right.hisId_;
    this->values_ = right.values_;

    return *this;
}

Histogram2D& Histogram2D::operator*=(int right) {
    unsigned sz = this->values_.size();
    for (unsigned i = 0; i < sz; ++i)
        this->values_[i] *= right;
    return *this;
}

Histogram2D& Histogram2D::operator+=(const Histogram2D& right) {
    if (this->xMin_ == right.xMin_ &&
        this->xMax_ == right.xMax_ &&
        this->yMin_ == right.yMin_ &&
        this->yMax_ == right.yMax_ &&
        this->nBinX_ == right.nBinX_&&
        this->nBinY_ == right.nBinY_) {
       
        unsigned sz = this->values_.size();
        for (unsigned i = 0; i < sz; ++i)
            this->values_[i] += right.values_[i];
        return *this;
    } else {
        throw GenError("Histogram2D::operator +=: histograms of different sizes"); 
    }
}

Histogram2D& Histogram2D::operator-=(const Histogram2D& right) {
    if (this->xMin_ == right.xMin_ &&
        this->xMax_ == right.xMax_ &&
        this->yMin_ == right.yMin_ &&
        this->yMax_ == right.yMax_ &&
        this->nBinX_ == right.nBinX_&&
        this->nBinY_ == right.nBinY_) {
       
        unsigned sz = this->values_.size();
        for (unsigned i = 0; i < sz; ++i)
            this->values_[i] -= right.values_[i];
        return *this;
    } else {
        throw GenError("Histogram2D::operator -=: histograms of different sizes"); 
    }
}

const Histogram2D Histogram2D::operator*(int right) const {
    Histogram2D result = *this;
    result *= right;
    return result;
}

const Histogram2D Histogram2D::operator+(const Histogram2D& right) const {
    Histogram2D result = *this;
    result += right;
    return result;
}

const Histogram2D Histogram2D::operator-(const Histogram2D& right) const {
    Histogram2D result = *this;
    result -= right;
    return result;
}


long& Histogram2D::operator()(unsigned ix, unsigned iy) {
   if (ix < nBinX_ && iy < nBinY_ )
     return values_[iy * nBinX_  + ix];
   else
     throw ArrayError("&Histogram2D::operator(): Matrix subscript out of bounds");
}
 
long Histogram2D::operator()(unsigned ix, unsigned iy) const {
   if (ix < nBinX_ && iy < nBinY_ )
     return values_[iy * nBinX_  + ix];
   else
    throw ArrayError("Histogram2D::operator(): Matrix subscript out of bounds");
}

