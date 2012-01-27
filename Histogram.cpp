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
    underflow = 0;
    overflow = 0;
}


void Histogram::getDataRaw (vector<long>& values) const  {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; ++i)
        values.push_back(values_[i]);
}

void Histogram::setDataRaw (vector<long>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = values[i];

    for (; i < sz; ++i)
        values_[i] = 0;
}

void Histogram::setDataRaw (vector<unsigned>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned sz = values_.size();

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = static_cast<long>(values[i]);

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
            ++overflow;
    } else {
        ++underflow;
    }

}

long Histogram1D::get (unsigned ix) {
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
        
void Histogram1D::rebin1D (Histogram1D* rebinned) {
    //To be implemented
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
}

unsigned short Histogram2D::getDim() const {
    return 2;
}

void Histogram2D::add (double x, double y, long n /* = 1*/) {
    unsigned ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() );
    unsigned iy = static_cast<unsigned>( (y - yMin_) / getBinWidthY() );
    
    //Underflow first (if one coordinate is below min, and second over max, count goes to underflow)
    if (x < xMin_ || y < yMin_)
        ++underflow;
    else if (x > xMax_ || y > yMax_)
        ++overflow;
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

/**
    * Gate includes over and undershoots (0 and nbinX+1 respectively). 
    * Gate includes both x0 and x1 bins.
    */
void Histogram2D::gateX (unsigned x0, unsigned x1, vector<long>& result) {

    result.clear();
    result.resize(nBinY_, 0);

    for (unsigned ix = x0; ix < x1 + 1; ++ix) 
        for (unsigned iy = 0; iy < nBinY_ ; ++iy) 
            result[iy] += values_[iy * nBinX_  + ix];

}

void Histogram2D::gateY (unsigned y0, unsigned y1, vector<long>& result) {

    result.clear();
    result.resize(nBinX_, 0);

    for (unsigned iy = y0; iy < y1 + 1; ++iy) 
        for (unsigned ix = 0; ix < nBinX_ ; ++ix) 
            result[ix] += values_[iy * nBinX_  + ix];

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

void Histogram2D::rebin2D (Histogram2D* rebinned) {
    //To be implemented
}

Histogram2D& Histogram2D::operator=(const Histogram2D& right){
    // Self assigment test
    if (this == &right)
        return *this;

    this->xMin_ = right.xMin_;
    this->xMax_ = right.xMax_;
    this->nBinX_= right.nBinX_;

    this->yMin_ = right.xMin_;
    this->yMax_ = right.xMax_;
    this->nBinY_= right.nBinX_;

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

