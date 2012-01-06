#include <cmath>
#include <vector>
#include "Histogram.h"

void Histogram::getDataRaw (vector<long>& values) {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; i++)
        values.push_back(values_[i]);
}


void Histogram::getErrorsRaw (vector<double>& errors) {
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

void Histogram1D::add (const double x, const long n /* = 1*/) {
    unsigned ix = 0;
    if ( x > xMin_ )
        ix = static_cast<unsigned>( (x - xMin_) / getBinWidthX() + 1 );
    if ( ix > nBinX_)
        ix = nBinX_ + 1;

    values_[ix] += n;
}

long Histogram1D::get (const unsigned ix) {
    if (ix < nBinX_ + 1)
        return values_[ix];
    else
        // exception
        return 0;
}

void Histogram1D::set (const unsigned ix, const long value) {
    if (ix < nBinX_ + 1)
        values_[ix] = value;
    else
        // exception
        ;
}
        
double Histogram1D::getError (const unsigned ix) {
    if (ix < nBinX_ + 1)
        return errors_[ix];
    else
        // exception
        return 0;
}

void Histogram1D::setError (const unsigned ix, const double error) {
    if (ix < nBinX_ + 1)
        errors_[ix] = error;
    else
        // exception
        ;
}

//***************************          Histogram2D

void Histogram2D::add (const double x, const double y,
                                const long n /* = 1*/) {
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

long Histogram2D::get (const unsigned ix, const unsigned iy){
    return values_[iy * (nBinX_ + 2) + ix];
}

void Histogram2D::set (const unsigned ix, const unsigned iy,
                                const long value) {
    if (ix < nBinX_ + 1 && iy < nBinY_ + 1)
        values_[iy * (nBinX_ + 2) + ix] = value;
    else
        // exception
        ;
}

double Histogram2D::getError (const unsigned ix, const unsigned iy){
    return errors_[iy * (nBinX_ + 2) + ix];
}

void Histogram2D::setError (const unsigned ix, const unsigned iy,
                const double error) {
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
void Histogram2D::gateX (const unsigned x0, const unsigned x1, vector<long>& result) {

    result.clear();
    result.resize(nBinY_ + 2, 0);

    for (unsigned ix = x0; ix < x1 + 1; ix++) 
        for (unsigned iy = 0; iy < nBinY_ + 2; iy++) 
            result[iy] += values_[iy * (nBinX_ + 2) + ix];

}

void Histogram2D::gateY (const unsigned y0, const unsigned y1, vector<long>& result) {

    result.clear();
    result.resize(nBinX_ + 2, 0);

    for (unsigned iy = y0; iy < y1 + 1; iy++) 
        for (unsigned ix = 0; ix < nBinX_ + 2; iy++) 
            result[ix] += values_[iy * (nBinX_ + 2) + ix];

}

void Histogram2D::gateXbackground (const unsigned x0, const unsigned x1,
                        const unsigned b0, const unsigned b1,
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

void Histogram2D::gateYbackground (const unsigned y0, const unsigned y1,
                        const unsigned b0, const unsigned b1,
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
