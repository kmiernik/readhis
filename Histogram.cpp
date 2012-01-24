#include <cmath>
#include <vector>
#include "Histogram.h"
#include "Exceptions.h"

void Histogram::getDataRaw (vector<long>& values) const  {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; ++i)
        values.push_back(values_[i]);
}


void Histogram::getErrorsRaw (vector<double>& errors) const  {
    unsigned sz = errors_.size();
    errors.clear();
    errors.reserve(sz);
    for (unsigned i = 0; i < sz; ++i)
        errors.push_back(errors_[i]);
}

void Histogram::setDataRaw (vector<long>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned szHigh = max( values.size(), values_.size() );

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = values[i];

    for (; i < szHigh; ++i)
        values_[i] = 0;
}

void Histogram::setDataRaw (vector<unsigned>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned szHigh = max( values.size(), values_.size() );

    unsigned i = 0;

    for (; i < szLow; ++i)
        values_[i] = static_cast<long>(values[i]);

    for (; i < szHigh; ++i)
        values_[i] = 0;
}

void Histogram::setErrorsRaw (vector<double>& errors) {
    unsigned szLow  = min( errors.size(), errors_.size() );
    unsigned szHigh = max( errors.size(), errors_.size() );

    unsigned i = 0;

    for (; i < szLow; ++i)
        errors_[i] = errors[i];

    for (; i < szHigh; ++i)
        errors_[i] = 0;
}
    
//************************ Histogram1D

void Histogram1D::add (double x, long n /* = 1*/) {
    unsigned ix = 0;
    if ( x > xMin_ )
        ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() + 1 );
    if ( ix > nBinX_)
        ix = nBinX_ + 1;

    values_[ix] += n;
}

long Histogram1D::get (unsigned ix) {
    if (ix < nBinX_ + 1)
        return values_[ix];
    else
        throw ArrayError("Histogram1D::set Matrix subscript out of bounds"); 
}

void Histogram1D::set (unsigned ix, long value) {
    if (ix < nBinX_ + 1)
        values_[ix] = value;
    else
        throw ArrayError("Histogram1D::set Matrix subscript out of bounds"); 
}
        
double Histogram1D::getError (unsigned ix) {
    if (ix < nBinX_ + 1)
        return errors_[ix];
    else
        throw ArrayError("Histogram1D::getError Matrix subscript out of bounds"); 
}

void Histogram1D::setError (unsigned ix, double error) {
    if (ix < nBinX_ + 1)
        errors_[ix] = error;
    else
        throw ArrayError("Histogram1D::setError Matrix subscript out of bounds"); 
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
    this->errors_ = right.errors_;

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
   if (ix > nBinX_ )
     throw ArrayError("&Histogram1D::operator[]: Matrix subscript out of bounds");
   return values_[ix];
}
 
long Histogram1D::operator[](unsigned ix) const {
   if (ix > nBinX_ + 1)
     throw ArrayError("Histogram1D::operator[]: Matrix subscript out of bounds");
   return values_[ix];
}

long& Histogram1D::operator()(unsigned ix) {
   if (ix > nBinX_ + 1)
     throw ArrayError("&Histogram1D::operator(): Matrix subscript out of bounds");
   return values_[ix];
}
 
long Histogram1D::operator()(unsigned ix) const {
   if (ix > nBinX_ + 1)
     throw ArrayError("Histogram1D::operator(): Matrix subscript out of bounds");
   return values_[ix];
}

//***************************          Histogram2D

void Histogram2D::add (double x, double y, long n /* = 1*/) {
    unsigned ix = 0;
    unsigned iy = 0;

    if ( x > xMin_ )
        ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() + 1 );
    if ( ix > nBinX_)
        ix = nBinX_ + 1;

    if ( y > yMin_ )
        iy = static_cast<unsigned>( (y - yMin_) / getBinWidthY() + 1 );
    if ( iy > nBinY_)
        iy = nBinY_ + 1;
    
    values_[iy * (nBinX_ + 2) + ix] += n;
}

long Histogram2D::get (unsigned ix, unsigned iy){
    return values_[iy * (nBinX_ + 2) + ix];
}

void Histogram2D::set (unsigned ix, unsigned iy, long value) {
    if (ix < nBinX_ + 1 && iy < nBinY_ + 1)
        values_[iy * (nBinX_ + 2) + ix] = value;
    else
        throw ArrayError("Histogram2D::set Matrix subscript out of bounds"); 
}

double Histogram2D::getError (unsigned ix, unsigned iy){
    return errors_[iy * (nBinX_ + 2) + ix];
}

void Histogram2D::setError (unsigned ix, unsigned iy, double error) {
    if (ix < nBinX_ + 1 && iy < nBinY_ + 1)
        errors_[iy * (nBinX_ + 2) + ix] = error;
    else
        throw ArrayError("Histogram2D::setError Matrix subscript out of bounds"); 
}

/**
    * Gate includes over and undershoots (0 and nbinX+1 respectively). 
    * Gate includes both x0 and x1 bins.
    */
void Histogram2D::gateX (unsigned x0, unsigned x1, vector<long>& result) {

    result.clear();
    result.resize(nBinY_ + 2, 0);

    for (unsigned ix = x0; ix < x1 + 1; ++ix) 
        for (unsigned iy = 0; iy < nBinY_ + 2; ++iy) 
            result[iy] += values_[iy * (nBinX_ + 2) + ix];

}

void Histogram2D::gateY (unsigned y0, unsigned y1, vector<long>& result) {

    result.clear();
    result.resize(nBinX_ + 2, 0);

    for (unsigned iy = y0; iy < y1 + 1; ++iy) 
        for (unsigned ix = 0; ix < nBinX_ + 2; ++iy) 
            result[ix] += values_[iy * (nBinX_ + 2) + ix];

}

void Histogram2D::gateXbackground (unsigned x0, unsigned x1,
                                   unsigned b0, unsigned b1,
                        vector<long>& result,
                        vector<double>& resultErrors) {

    result.clear();
    result.resize(nBinY_ + 2, 0);
    resultErrors.clear();
    resultErrors.resize(nBinY_ + 2, 0);

    for (unsigned ix = x0; ix < x1 + 1; ++ix) 
        for (unsigned iy = 0; iy < nBinY_ + 2; ++iy) {
            result[iy] += values_[iy * (nBinX_ + 2) + ix];
            resultErrors[iy] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned ix = b0; ix < b1 + 1; ++ix) 
        for (unsigned iy = 0; iy < nBinY_ + 2; ++iy) {
            result[iy] -= values_[iy * (nBinX_ + 2) + ix];
            resultErrors[iy] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned iy = 0; iy < nBinY_ + 2; ++iy)
        resultErrors[iy] = sqrt(resultErrors[iy]);

}

void Histogram2D::gateYbackground (unsigned y0, unsigned y1,
                                   unsigned b0, unsigned b1,
                                   vector<long>& result,
                                   vector<double>& resultErrors) {

    result.clear();
    result.resize(nBinX_ + 2, 0);
    resultErrors.clear();
    resultErrors.resize(nBinX_ + 2, 0);

    for (unsigned iy = y0; iy < y1 + 1; ++iy) 
        for (unsigned ix = 0; ix < nBinX_ + 2; ++ix) {
            result[ix] += values_[iy * (nBinX_ + 2) + ix];
            resultErrors[ix] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned iy = b0; iy < b1 + 1; ++iy) 
        for (unsigned ix = 0; ix < nBinX_ + 2; ++ix) {
            result[ix] -= values_[iy * (nBinX_ + 2) + ix];
            resultErrors[ix] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned ix = 0; ix < nBinY_ + 2; ++ix)
        resultErrors[ix] = sqrt(resultErrors[ix]);

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
    this->errors_ = right.errors_;

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
   if (ix > nBinX_ + 1 || iy > nBinY_ + 1)
     throw ArrayError("&Histogram2D::operator(): Matrix subscript out of bounds");
   return values_[iy * (nBinX_ + 2) + ix];
}
 
long Histogram2D::operator()(unsigned ix, unsigned iy) const {
   if (ix > nBinX_ + 1 || iy > nBinY_ + 1)
     throw ArrayError("Histogram2D::operator(): Matrix subscript out of bounds");
   return values_[iy * (nBinX_ + 2) + ix];
}

