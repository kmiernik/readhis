#ifndef HOPTIONS
#define HOPTIONS

class Options {
    public:
        unsigned getHisId () const { return hisId_; }
        bool setHisId (unsigned const hisId) {
            if (hisId > 0 && hisId < 10000) {
                hisId_ = hisId;
                isIdSet_ = true;
                return true;
            } else {
                isIdSet_ = false;
                return false;
            }
        }
        bool getisIdSet() const { return isIdSet_; }

        bool getListMode() const { return isListMode_; }
        void setListMode (const bool b) { 
            if (b) {
                isListModeZ_ = false;
                isListMode_ = true;
            } else {
                isListMode_ = false;
            }
            
        }

        bool getInfoMode() const { return isInfoMode_; }
        void setInfoMode (const bool b) { isInfoMode_ = b; }

        bool getListModeZ() const { return isListModeZ_; }
        void setListModeZ (const bool b) { 
            if (b) {
                isListMode_ = false;
                isListModeZ_ = true;
            } else {
                isListModeZ_ = false;
            }
        }

        bool getZeroSup() const { return isZeroSup_; }
        void setZeroSup (const bool b) { isZeroSup_ = b; }
        
        bool getGx() const { return isGx_; }
        bool setGx (const bool isGx, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGx_ = isGx;
            if (isGx) {
                isGy_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            return true;
        }

        bool getGy() const { return isGy_; }
        bool setGy (const bool isGy, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGy_ = isGy;
            if (isGy) {
                isGx_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            g0_ = g0;
            g1_ = g1;
            return true;
        }

        bool getBg() const { return isBg_; }
        bool setBg (const bool isBg, const unsigned b0, const unsigned b1) {
            if (b0 > b1)
                return false;
            isBg_ = isBg;
            if (isBg) {
                isSBg_ = false;
                b0_ = b0;
                b1_ = b1;
            }
            return true;
        }
        
        bool getSBg() const { return isSBg_; }
        bool setSBg (const bool isSBg, const unsigned b0, const unsigned b1,
                                       const unsigned b2, const unsigned b3) {
            if ( !(b3 > b2 && b2 > b1 && b1 > b0) )
                return false;
            isSBg_ = isSBg;
            if (isSBg) {
                isBg_ = false;
                b0_ = b0;
                b1_ = b1;
                b2_ = b2;
                b3_ = b3;
            }
            return true;
        }

        Options() {
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
            g0_ = 0;
            g1_ = 0;
            b0_ = 0;
            b1_ = 0;
            b2_ = 0;
            b3_ = 0;
        }
    private:
        unsigned hisId_;
        bool isIdSet_;
        bool isListMode_;
        bool isListModeZ_;
        bool isInfoMode_;
        bool isZeroSup_;
        bool isGx_;
        bool isGy_;
        bool isBg_;
        bool isSBg_;
        unsigned g0_;
        unsigned g1_;
        unsigned b0_;
        unsigned b1_;
        unsigned b2_;
        unsigned b3_;
};

#endif
