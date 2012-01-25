#ifndef HHISDRRHISTO
#define HHISDRRHISTO

#include <string>
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"

using namespace std;

class HisDrrHisto {
    public:
        HisDrrHisto(Options* options, string baseName) : baseName_(baseName) {
        options_ = options;}
        void process();
    private:
        Options* options_;
        string baseName_;

        void runListMode(HisDrr* h, bool zero);
        void runInfoMode(DrrHisRecordExtended& info);
};

#endif
