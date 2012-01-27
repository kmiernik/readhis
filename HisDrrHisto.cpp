#include <string> 
#include <cmath> 
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"
#include "HisDrrHisto.h"

using namespace std;

//inline HisDrrHisto::HisDrrHisto(Options* options, string baseName) {
//    options_ = options;
//    baseName_ = baseName;
//}
HisDrrHisto::HisDrrHisto(const string drr, const string his, 
                         const Options* options)
                        : HisDrr(drr, his) {
    options_ = options;
}

void HisDrrHisto::runListMode(bool more) {
    vector<int> list;
    getHisList(list);
    unsigned numOfHis = list.size();
    for (unsigned i = 0; i < numOfHis; ++i) {
        if (more) {
            vector<unsigned int> d;
            getHistogram(d, list[i]);
            info = getHistogramInfo(list[i]);
            unsigned sz = d.size();
            bool empty = true;
            for (unsigned j = 0; j < sz; ++j)
                if (d[j] > 0) {
                    empty = false;
                    break;
                }
            if (empty)
                cout << "E" << list[i] << "(" << info.hisDim << ")";
            else
                cout << "\033[1;34m" << list[i] << "\033[0m" << "(" << info.hisDim << ")";
        } else {
            cout << list[i];
        }
        
        cout << ", ";
        if ((i+1) % 10 == 0)
            cout << endl;

    }
    cout << "\033[0;30m\033[0m" << endl;
}

void HisDrrHisto::runInfoMode() {
    cout << "#ID: " << info.hisID << endl;
    cout << "#hisDim: " << info.hisDim << endl;
    cout << "#halfWords: " << info.halfWords << endl;
    for (int j = 0; j < 4; ++j)
        cout << "#params[" << j << "]: " << info.params[j] << endl;
    for (int j = 0; j < 4; ++j)
        cout << "#raw[" << j << "]: " << info.raw[j] << endl;
    for (int j = 0; j < 4; ++j)
        cout << "#scaled[" << j << "]: " << info.scaled[j] << endl;
    for (int j = 0; j < 4; ++j)
        cout << "#minc[" << j << "]: " << info.minc[j] << endl;
    for (int j = 0; j < 4; ++j)
        cout << "#maxc[" << j << "]: " << info.maxc[j] << endl;

    cout << "#offset: " << info.offset << endl;
    cout << "#xlabel: " << info.xlabel << endl;
    cout << "#ylabel: " << info.ylabel << endl;

    for (int j = 0; j < 4; ++j)
        cout << "#calcon[" << j << "]: " << info.calcon[j] << endl;

    cout << "#title: " << info.title << endl;
}

void HisDrrHisto::process1D() {
    // maxc + 1 because drr has bins numbered 0 to maxc 
    // but size then is maxc+ 1
    histogram = new Histogram1D(info.minc[0], info.maxc[0] + 1,
                                info.scaled[0], "");

    vector<unsigned> data;
    data.reserve(info.scaled[0]);
    //Load data from his file
    getHistogram(data, info.hisID);
    histogram->setDataRaw(data);

    Histogram1D* h1 = dynamic_cast<Histogram1D*>(histogram);
    if (options_->getZeroSup()) {

        for (int i = 0; i < info.scaled[0]; ++i)
            if ((*h1)[i] == 0 )
                cout << h1->getX(i) << " " << (*h1)[i] << endl;

    } else {

        for (int i = 0; i < info.scaled[0]; ++i)
            cout << h1->getX(i) << " " << (*h1)[i] << endl;

    }
    delete histogram;
}

void HisDrrHisto::process2D() {
    histogram = new Histogram2D(info.minc[0], info.maxc[0] + 1,
                                info.minc[1], info.maxc[1] + 1, 
                                info.scaled[0], info.scaled[1],
                                "");

    vector<unsigned> data;
    data.reserve( info.scaled[0] * info.scaled[1]);

    //Load data from his file
    getHistogram(data, info.hisID);
    histogram->setDataRaw(data);

    Histogram2D* h2 = dynamic_cast<Histogram2D*>(histogram);
    if (options_->getGy()) {
        //--gy
        vector<long> result;
        vector<double> error;

        vector<unsigned> gate;
        options_->getGate(gate);

        if (gate.size() < 2)
            throw GenError("--gx: not enough gate points");

        if (options_->getBg()){
            //--gy --bg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 2) {
                vector<long> temp;
                h2->gateY(gate[0], gate[1], result);
                h2->gateY(bgr[0], bgr[1], temp);
                unsigned sz = result.size();
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt(result[i] + temp[i]);
                    result[i] -= temp[i];
                }
            } else {
                throw GenError("--bg: not enough gate points");
            }
            //end --gy --bg
        } else if (options_->getSBg()){
            //--gy --sbg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 4) {
                vector<long> temp1;
                vector<long> temp2;

                h2->gateY(gate[0], gate[1], result);
                h2->gateY(bgr[0], bgr[1], temp1);
                h2->gateY(bgr[2], bgr[3], temp2);
                unsigned sz = result.size();
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt(result[i] + temp1[i] + temp2[i]);
                    result[i] -= temp1[i] + temp2[i];
                }
            } else {
                throw GenError("--sbg: not enough gate points");
            }
            //end --gy --sbg
        } else {
            // --gy no background
            h2->gateY(gate[0], gate[1], result);
            unsigned sz = result.size();
            error.resize(sz, 0.0);
            for (unsigned i = 0; i < sz; ++i) {
                error[i] = sqrt(result[i]);
            }
            // end --gx no background
        }

        unsigned sz = result.size();
        for (unsigned i = 0; i < sz; ++i)
            cout << h2->getX(i) << " " << result[i] << " " << error[i] << endl;
        //end --gx
    } else if (options_->getGx()){
        //--gx
        vector<long> result;
        vector<double> error;

        vector<unsigned> gate;
        options_->getGate(gate);
        if (gate.size() < 2)
            throw GenError("--gx: not enough gate points");

        if (options_->getBg()){
            //--gx --bg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 2) {
                vector<long> temp;
                h2->gateX(gate[0], gate[1], result);
                h2->gateX(bgr[0], bgr[1], temp);
                unsigned sz = result.size();
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt(result[i] + temp[i]);
                    result[i] -= temp[i];
                }
            } else {
                throw GenError("--bg: not enough gate points");
            }
            //end --gx --bg
        } else if (options_->getSBg()){
            //--gx --sbg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 4) {
                vector<long> temp1;
                vector<long> temp2;

                h2->gateX(gate[0], gate[1], result);
                h2->gateX(bgr[0], bgr[1], temp1);
                h2->gateX(bgr[2], bgr[3], temp2);
                unsigned sz = result.size();
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt(result[i] + temp1[i] + temp2[i]);
                    result[i] -= temp1[i] + temp2[i];
                }
            } else {
                throw GenError("--sbg: not enough gate points");
            }
            //end --gx --sbg
        } else {
            // --gx no background
            h2->gateX(gate[0], gate[1], result);
            unsigned sz = result.size();
            error.resize(sz, 0.0);
            for (unsigned i = 0; i < sz; ++i) {
                error[i] = sqrt(result[i]);
            }
            // end --gx no background
        }

        unsigned sz = result.size();
        for (unsigned i = 0; i < sz; ++i)
            cout << h2->getY(i) << " " << result[i] << " " << error[i] << endl;
        //end --gx
    } else {
        // No gates case
        for (int x = 0; x < info.scaled[0]; ++x) {
            for (int y = 0; y < info.scaled[1]; ++y) {
                cout << h2->getX(x) << " " << h2->getY(y)  
                     << " " << (*h2)(x,y) << endl;
            }
            cout << endl;
        }
        //end no gates
    } 

    delete histogram;
}

void HisDrrHisto::process() {

    try {
        if ( options_->getListMode() )
            runListMode(false);
        else if (options_->getListModeZ())
            runListMode(true);
        else {
            if (!options_->isIdSet()) {
                throw GenError("Histogram id is required");
            }

            int hisId = options_->getHisId();
            info = getHistogramInfo(hisId);

            if (options_->getInfoMode()) { 
                runInfoMode();
            } else if (info.hisDim == 1) {
                process1D();
            } else if (info.hisDim == 2) {
                process2D();
            } else {
                throw GenError("Only 1 and 2 dimensional histograms are supported.");
            }
        }
    } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
        cout << "Run readhis --help for more information" << endl;
    }
}

HisDrrHisto::~HisDrrHisto() {
}
