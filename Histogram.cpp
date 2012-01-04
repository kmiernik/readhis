#include "Histogram.h"

void Histogram::getDataRaw (vector<unsigned long>& values) {
    unsigned sz = values_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; i++)
        values.push_back(values_[i]);
}


void Histogram::getErrorsRaw (vector<double>& errors) {
    unsigned sz = errors_.size();
    values.clear();
    values.reserve(sz);
    for (unsigned i = 0; i < sz; i++)
        values.push_back(values_[i]);
}

void Histogram1D::setDataRaw (vector<long>& values) {
    unsigned sz = min( values.size(), nBinX_ );
    unsigned i = 0;
    for (; i < sz; i++)
        values_[i] = values[i];
    for (; i < nBinX_; i++)
        values_[i] = 0;
}

void Histogram1D::setErrorsRaw (vector<long>& errors) {
    unsigned sz = min( errors.size(), nBinX_ );
    unsigned i = 0;
    for (; i < sz; i++)
        errors_[i] = errors[i];
    for (; i < nBinX_; i++)
        errors_[i] = 0;
}

Histogram1D (const unsigned nX, const double xMin, const double stepX)
                    : nX_(nX), xMin_(xMin), stepX_(stepX) {}
void setBinX (const unsigned binX);
virtual unsigned long getValue (const unsigned x);
virtual void setValue (const unsigned x, const unsigned value);
double getError (const unsigned x);
void setError (const unsigned x, const double value);




Histogram2D (const unsigned nX, const double xMin, const double stepX,
                     const unsigned nY, const double yMin, const double stepY)
                    : nX_(nX), xMin_(xMin), stepX_(stepX),
                      nY_(nY), yMin_(yMin), stepY_(stepY) {}

void Histogram2D::setDataRaw (vector<long>& values) {
    unsigned sz = min( values.size(), nBinX_ * nBinY_ );
    unsigned i = 0;
    for (; i < sz; i++)
        values_[i] = values[i];
    for (; i < nBinX_ * nBinY_; i++)
        values_[i] = 0;
}

void Histogram1D::setErrorsRaw (vector<long>& errors) {
    unsigned sz = min( errors.size(), nBinX_ * nBinY_ );
    unsigned i = 0;
    for (; i < sz; i++)
        errors_[i] = errors[i];
    for (; i < nBinX_ * nBinY_; i++)
        errors_[i] = 0;
}
void setBinXY (const unsigned binX, const unsigned binY) {
    if ( binX > 0 && binX < nBinX_ && binY > 0 && binY < nBinY_ ) {
        binX_ = binX;
        binY_ = binY;
    } else {
    // Exception
    }
}
        
virtual unsigned long getValue (const unsigned x, const unsigned y) {
    if ( x < nBinX_ / binX_ && y < nBinY_ / binY_ )
        return ;
    else {
        // Exception
        return 0;
    }
}
virtual void setValue (const unsigned x, const unsigned y, const unsigned long value) {
    if ( x < nBinX_ / binX_ && y < nBinY_ / binY_ )
        ;
    else
        //Exception
        ;
}

double getError (const unsigned x, const unsigned y);
void setError (const unsigned x, const unsigned y, const double value);

void gateX (const unsigned x0, const unsigned x1,
                    vector<unsigned logn>& result);
void gateY (const unsigned y0, const unsigned y1,
                    vector<unsigned logn>& result);

void gateXbackground (const unsigned x0, const unsigned x1,
                              const unsigned b0, const unsigned b1,
                              vector<unsigned logn>& result);
void gateYbackground (const unsigned y0, const unsigned y1,
                              const unsigned b0, const unsigned b1,
                              vector<unsigned logn>& result);

