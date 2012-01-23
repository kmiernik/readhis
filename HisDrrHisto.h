#ifndef HHISDRRHISTO
#define HHISDRRHISTO

#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"

class HisDrrHisto1D : public Histogram1D {
    public:
        HisDrrHisto1D(DrrHisRecordExtended& info) : Histogram1D(info.minc[0], info.maxc[0], info.scaled[0], "" )
        {info_ = info;}

        void Process();
    private:
        DrrHisRecordExtended info_;
};

class HisDrrHisto2D : public Histogram2D {
    public:
        HisDrrHisto2D(DrrHisRecordExtended& info) : Histogram2D(info.minc[0], info.maxc[0], info.scaled[0],
                                                                info.minc[1], info.maxc[1], info.scaled[1],
                                                                "" )
        {info_ = info;}
        void Process();
    private:
        DrrHisRecordExtended info_;
};

#endif
