#ifndef HOPTIONS
#define HOPTIONS

#include <vector>

class Options {
    public:
        unsigned getHisId () const ;
        bool setHisId (unsigned hisId);
        bool isIdSet() const;

        bool getListMode() const;
        void setListMode (bool b); 

        bool getInfoMode() const;
        void setInfoMode (bool b);

        bool getListModeZ() const;
        void setListModeZ (bool b);

        bool getZeroSup() const;
        void setZeroSup (bool b);
        
        bool getGx() const;
        bool setGx (bool isGx, unsigned g0, unsigned g1);

        bool getGy() const;
        bool setGy (bool isGy, unsigned g0, unsigned g1);

        bool getBg() const;
        bool setBg (bool isBg, unsigned b0, unsigned b1);
        
        bool getSBg() const;
        bool setSBg (bool isSBg, unsigned b0, unsigned b1,
                                 unsigned b2, unsigned b3);
        
        std::vector<unsigned> getGate();
        std::vector<unsigned> getBgGate();

        Options();

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
        std::vector<unsigned> g_;
        std::vector<unsigned> b_;
};

#endif
