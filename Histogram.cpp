#include <cmath>
#include <vector>
#include "Histogram.h"
#include "Exceptions.h"

void Histogram::getDataRaw (vector<long>& values) const  {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; i++)
        values.push_back(values_[i]);
}


void Histogram::getErrorsRaw (vector<double>& errors) const  {
    unsigned sz = errors_.size();
    errors.clear();
    errors.reserve(sz);
    for (unsigned i = 0; i < sz; i++)
        errors.push_back(errors_[i]);
}

void Histogram::setDataRaw (vector<long>& values) {
    unsigned szLow  = min( values.size(), values_.size() );
    unsigned szHigh = max( values.size(), values_.size() );

    unsigned i = 0;

    for (; i < szLow; i++)
        values_[i] = values[i];

    for (; i < szHigh; i++)
        values_[i] = 0;
}

void Histogram::setErrorsRaw (vector<double>& errors) {
    unsigned szLow  = min( errors.size(), errors_.size() );
    unsigned szHigh = max( errors.size(), errors_.size() );

    unsigned i = 0;

    for (; i < szLow; i++)
        errors_[i] = errors[i];

    for (; i < szHigh; i++)
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
        throw GenError("Outside of histogram range"); 
        return 0;
}

void Histogram1D::set (unsigned ix, long value) {
    if (ix < nBinX_ + 1)
        values_[ix] = value;
    else
        throw GenError("Outside of histogram range"); 
}
        
double Histogram1D::getError (unsigned ix) {
    if (ix < nBinX_ + 1)
        return errors_[ix];
    else
        throw GenError("Outside of histogram range"); 
        return 0;
}

void Histogram1D::setError (unsigned ix, double error) {
    if (ix < nBinX_ + 1)
        errors_[ix] = error;
    else
        throw GenError("Outside of histogram range"); 
}

const Histogram1D operator*(const Histogram1D& left,
                            const int right) {
    Histogram1D multi(left.xMin_, left.xMax_, left.nBinX_, left.hisId_);
    unsigned sz = left.values_.size();
    vector<long> values;
    left.getDataRaw(values);
    for (unsigned i = 0; i < sz; i++)
        values[i] *= right;
    multi.setDataRaw(values);
    return multi;
}

const Histogram1D operator+(const Histogram1D& left,
                            const Histogram1D& right) {

    Histogram1D sum(left.xMin_, left.xMax_, left.nBinX_, left.hisId_);
    if (left.xMin_ == right.xMin_ &&
        left.xMax_ == right.xMax_ &&
        left.nBinX_ == right.nBinX_) {
    
        unsigned sz = left.values_.size();
        vector<long> values(sz, 0);
        for (unsigned i = 0; i < sz; i++)
            values[i] = left.values_[i] + right.values_[i];

        sum.setDataRaw(values);
        return sum;
    } else {
        throw GenError("Operator +, but histogram of different sizes"); 
        return left;
    }
}
//const Histogram1D operator-(const Histogram1D& left,
//                            const Histogram1D& right) {
//}

long& Histogram1D::operator[](unsigned ix) {
   if (ix > nBinX_ )
     throw BadIndex("Matrix subscript out of bounds");
   return values_[ix];
}
 
long Histogram1D::operator[](unsigned ix) const {
   if (ix > nBinX_ + 1)
     throw BadIndex("Matrix subscript out of bounds");
   return values_[ix];
}

long& Histogram1D::operator()(unsigned ix) {
   if (ix > nBinX_ + 1)
     throw BadIndex("Matrix subscript out of bounds");
   return values_[ix];
}
 
long Histogram1D::operator()(unsigned ix) const {
   if (ix > nBinX_ + 1)
     throw BadIndex("Matrix subscript out of bounds");
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
        // exception
        ;
}

double Histogram2D::getError (unsigned ix, unsigned iy){
    return errors_[iy * (nBinX_ + 2) + ix];
}

void Histogram2D::setError (unsigned ix, unsigned iy, double error) {
    if (ix < nBinX_ + 1 && iy < nBinY_ + 1)
        errors_[iy * (nBinX_ + 2) + ix] = error;
    else
        // exception
        ;
}

/**
    * Gate includes over and undershoots (0 and nbinX+1 respectively). 
    * Gate includes both x0 and x1 bins.
    */
void Histogram2D::gateX (unsigned x0, unsigned x1, vector<long>& result) {

    result.clear();
    result.resize(nBinY_ + 2, 0);

    for (unsigned ix = x0; ix < x1 + 1; ix++) 
        for (unsigned iy = 0; iy < nBinY_ + 2; iy++) 
            result[iy] += values_[iy * (nBinX_ + 2) + ix];

}

void Histogram2D::gateY (unsigned y0, unsigned y1, vector<long>& result) {

    result.clear();
    result.resize(nBinX_ + 2, 0);

    for (unsigned iy = y0; iy < y1 + 1; iy++) 
        for (unsigned ix = 0; ix < nBinX_ + 2; iy++) 
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

    for (unsigned ix = x0; ix < x1 + 1; ix++) 
        for (unsigned iy = 0; iy < nBinY_ + 2; iy++) {
            result[iy] += values_[iy * (nBinX_ + 2) + ix];
            resultErrors[iy] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned ix = b0; ix < b1 + 1; ix++) 
        for (unsigned iy = 0; iy < nBinY_ + 2; iy++) {
            result[iy] -= values_[iy * (nBinX_ + 2) + ix];
            resultErrors[iy] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned iy = 0; iy < nBinY_ + 2; iy++)
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

    for (unsigned iy = y0; iy < y1 + 1; iy++) 
        for (unsigned ix = 0; ix < nBinX_ + 2; ix++) {
            result[ix] += values_[iy * (nBinX_ + 2) + ix];
            resultErrors[ix] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned iy = b0; iy < b1 + 1; iy++) 
        for (unsigned ix = 0; ix < nBinX_ + 2; ix++) {
            result[ix] -= values_[iy * (nBinX_ + 2) + ix];
            resultErrors[ix] += values_[iy * (nBinX_ + 2) + ix];
        }

    for (unsigned ix = 0; ix < nBinY_ + 2; ix++)
        resultErrors[ix] = sqrt(resultErrors[ix]);

}

long& Histogram2D::operator()(unsigned ix, unsigned iy) {
   if (ix > nBinX_ + 1 || iy > nBinY_ + 1)
    throw BadIndex("Matrix subscript out of bounds");
   return values_[iy * (nBinX_ + 2) + ix];
}
 
long Histogram2D::operator()(unsigned ix, unsigned iy) const {
   if (ix > nBinX_ + 1 || iy > nBinY_ + 1)
     throw BadIndex("Matrix subscript out of bounds");
   return values_[iy * (nBinX_ + 2) + ix];
}
