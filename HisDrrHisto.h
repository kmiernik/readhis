#ifndef HHISDRRHISTO
#define HHISDRRHISTO

#include <string>
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"

using namespace std;

class HisDrrHisto : public HisDrr {
    public:
        HisDrrHisto(const string drr, const string his,
                    const Options* options);
        void process();
        ~HisDrrHisto();
    private:
        const Options* options_;
        string baseName_;
        //current histogram
        Histogram* histogram;
        //current info
        DrrHisRecordExtended info;

        void runListMode(bool zero);
        void runInfoMode();
        void process1D();
        void process2D();
};

#endif
