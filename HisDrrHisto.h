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
        HisDrrHisto(const Options* options, string& baseName);
        void process();
    private:
        void runListMode(HisDrr* h, bool zero = false);
        const Options* options_;
        string baseName_;
};

#endif
