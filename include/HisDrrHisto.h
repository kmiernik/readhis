/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef HISDRRHISTO_H
#define HISDRRHISTO_H

#include <string>
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"

using namespace std;

/**
 * The main purpose of this class is to translate HisDrr data to Histogram,
 * perform operations accordinhgly to passed Options and print data to cout.
 * This class inherits after HisDrr and adds interface between HisDrr data,
 * and Histogram class. 
 */

class HisDrrHisto : public HisDrr {
    public:
        /** Ctor requires name of drr and his file and list of options (in 
         * form of Options class. */
        HisDrrHisto(const string drr, const string his,
                    const Options* options);
        /** Sets pointer to options.*/
        void setOptions(const Options* options);

        /** Procceses his/drr files accordingly to Options. */
        void process();
        ~HisDrrHisto();
    private:
        /** Pointer to Options */
        const Options* options_;

        /** Current histogram */
        Histogram* histogram;

        /** Current histogram info*/
        DrrHisRecordExtended info;

        /** Lists all histograms in his/drr file.
         *  more option true is more verbose output (marks histogram dimensions)
         *  and empty histograms.*/
        void runListMode(bool more);

        /** Prints all informations on current histogram */
        void runInfoMode();

        /** Sub part of process for 1D histograms */
        void process1D();

        /** Sub part of process for 2D histograms */
        void process2D();
};

inline void HisDrrHisto::setOptions(const Options* options) { options_ = options; }
#endif
