#include <vector>
#include "Options.h"

unsigned Options::getHisId () const { return hisId_; }
bool Options::setHisId (unsigned hisId) {
    if (hisId > 0 && hisId < 10000) {
        hisId_ = hisId;
        isIdSet_ = true;
        return true;
    } else {
        isIdSet_ = false;
        return false;
    }
}
bool Options::isIdSet() const { return isIdSet_; }

bool Options::getListMode() const { return isListMode_; }
void Options::setListMode (bool b) { 
    if (b) {
        isListModeZ_ = false;
        isListMode_ = true;
    } else {
        isListMode_ = false;
    }
    
}

bool Options::getInfoMode() const { return isInfoMode_; }
void Options::setInfoMode (bool b) { isInfoMode_ = b; }

bool Options::getListModeZ() const { return isListModeZ_; }
void Options::setListModeZ (bool b) { 
    if (b) {
        isListMode_ = false;
        isListModeZ_ = true;
    } else {
        isListModeZ_ = false;
    }
}

bool Options::getZeroSup() const { return isZeroSup_; }
void Options::setZeroSup (bool b) { isZeroSup_ = b; }

bool Options::getGx() const { return isGx_; }
bool Options::setGx (bool isGx, unsigned g0, unsigned g1) {
    if (g0 > g1)
        return false;
    g_.clear();
    isGx_ = isGx;
    if (isGx) {
        isGy_ = false;
        g_.resize(2, 0);
        g_[0] = g0;
        g_[1] = g1;
    }
    return true;
}

bool Options::getGy() const { return isGy_; }
bool Options::setGy (bool isGy, unsigned g0, unsigned g1) {
    if (g0 > g1)
        return false;
    g_.clear();
    isGy_ = isGy;
    if (isGy) {
        isGx_ = false;
        g_.resize(2, 0);
        g_[0] = g0;
        g_[1] = g1;
    }
    g_[0] = g0;
    g_[1] = g1;
    return true;
}

bool Options::getBg() const { return isBg_; }
bool Options::setBg (bool isBg, unsigned b0, unsigned b1) {
    if (b0 > b1)
        return false;

    b_.clear();
    isBg_ = isBg;
    if (isBg) {
        isSBg_ = false;
        b_.resize(2, 0);
        b_[0] = b0;
        b_[1] = b1;
    }
    return true;
}

bool Options::getSBg() const { return isSBg_; }
bool Options::setSBg (bool isSBg, unsigned b0, unsigned b1,
                                unsigned b2, unsigned b3) {
    if ( !(b3 > b2 && b2 > b1 && b1 > b0) )
        return false;

    b_.clear();
    isSBg_ = isSBg;
    if (isSBg) {
        isBg_ = false;
        b_.resize(4, 0);
        b_[0] = b0;
        b_[1] = b1;
        b_[2] = b2;
        b_[3] = b3;
    }
    return true;
}

std::vector<unsigned> Options::getGate() {
    return g_;
}
std::vector<unsigned> Options::getBgGate() {
    return b_;
}


Options::Options() {
    hisId_ = 0;
    isIdSet_ = false;
    isListMode_ = false;
    isListModeZ_ = false;
    isInfoMode_ = false;
    isZeroSup_ = false;
    isGx_ = false;
    isGy_ = false;
    isBg_ = false;
    isSBg_ = false;
}
