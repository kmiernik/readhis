/*
 * K. Miernik (k.a.miernik@gmail.com) 2012
 *
 */

#include <string> 
#include <cmath> 
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"
#include "HisDrrHisto.h"

using namespace std;

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

    if (options_->getBin()) {
        Histogram1D* h1b;
        vector<unsigned> bin;
        options_->getBinning(bin);
        if (bin[0] > 1)
            h1b = h1->rebin(h1->getxMin(), h1->getxMax(), 
                              (unsigned)h1->getnBinX()/bin[0]);
        else
            throw GenError("HisDrrHisto::process1D : Wrong binning size.");
        (*h1) =(*h1b);
        delete h1b;
    }

    unsigned sz = h1->getnBinX();
    if (options_->getZeroSup()) {
        for (unsigned i = 0; i < sz; ++i)
            if ((*h1)[i] == 0 )
                cout << h1->getX(i) << " " << (*h1)[i] << endl;

    } else {

        for (unsigned i = 0; i < sz; ++i)
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

    bool gx = options_->getGx();
    bool gy = options_->getGx();
    bool bg = options_->getBg();
    bool sbg = options_->getSBg();

    if (gx || gy){
        // Resulting projection
        Histogram1D* proj;
        // Uncertainities are going to be stored in a separate histogram
        Histogram1D* projErr;

        vector<unsigned> gate;
        options_->getGate(gate);
        if (gate.size() < 2)
            throw GenError("process2D: Not enough gate points");
       
        if (gx) {
            proj = h2->gateX(gate[0], gate[1]);
            projErr = h2->gateX(gate[0], gate[1]);
        } else {
            proj = h2->gateY(gate[0], gate[1]);
            projErr = h2->gateY(gate[0], gate[1]);
        }

        if (bg || sbg){
            //--gy --bg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);
            Histogram1D* projBg;

            if (bgr.size() >= 2) {
                if (gx)
                    projBg = h2->gateX(bgr[0], bgr[1]);
                else
                    projBg = h2->gateY(bgr[0], bgr[1]);

                *proj -= *projBg;
                *projErr += *projBg;
            } else
                throw GenError("process2D: Not enough background gate points");

            if (sbg) {
                //--gy --sbg
                if (bgr.size() >= 4) {
                    if (gx)
                        projBg = h2->gateX(bgr[2], bgr[3]);
                    else
                        projBg = h2->gateY(bgr[2], bgr[3]);

                    *proj -= *projBg;
                    *projErr += *projBg;
                } else
                    throw GenError("process2D: Not enough split background gate points");
            }

            delete projBg;
        }

        if (options_->getBin()) {
            // proj binned
            Histogram1D* projBin;
            Histogram1D* projErrBin;

            vector<unsigned> bin;
            options_->getBinning(bin);
            if (bin[0] > 1 && bin[1] > 1) {
                if (gx) {
                    projBin = proj->rebin(proj->getxMin(), proj->getxMax(), 
                                           (unsigned)proj->getnBinX()/bin[1]);
                    projErrBin = projErr->rebin(projErr->getxMin(), projErr->getxMax(), 
                                           (unsigned)projErr->getnBinX()/bin[1]);
                } else {
                    projBin = proj->rebin(proj->getxMin(), proj->getxMax(), 
                                           (unsigned)proj->getnBinX()/bin[0]);
                    projErrBin = projErr->rebin(projErr->getxMin(), projErr->getxMax(), 
                                           (unsigned)projErr->getnBinX()/bin[0]);
                }

            } else
                throw GenError("HisDrrHisto::process2D : Wrong binning size.");

            (*proj) =(*projBin);
            (*projErr) =(*projErrBin);
            
            delete projBin;
            delete projErrBin;
        }

        unsigned sz = proj->getnBinX();
        for (unsigned i = 0; i < sz; ++i)
            cout << proj->getX(i) << " " << (*proj)[i] << " " << sqrt((*projErr)[i]) << endl;

        delete projErr;
        delete proj;

    } else {
        // No gates case
        
        // Rebinning (if applicable)
        if (options_->getBin()) {
            Histogram2D* h2b;
            vector<unsigned> bin;
            options_->getBinning(bin);
            if (bin[0] > 1 && bin[1] > 1)
                h2b = h2->rebin( h2->getxMin(), h2->getxMax(), 
                                h2->getyMin(), h2->getyMax(), 
                                (unsigned)h2->getnBinX()/bin[0],
                                (unsigned)h2->getnBinY()/bin[1] );
            else
                throw GenError("HisDrrHisto::process2D : Wrong binning size.");
            (*h2) =(*h2b);
            delete h2b;
        }

        unsigned szX = h2->getnBinX();
        unsigned szY = h2->getnBinY();

        for (unsigned x = 0; x < szX; ++x) {
            for (unsigned y = 0; y < szY; ++y) {
                cout << h2->getX(x) << " " << h2->getY(y)  
                     << " " << (*h2)(x,y) << endl;
            }
            cout << endl;
        }
        //end no gates
    }
    //*******************************************************
/*
    if (options_->getGy()) {
        //--gy
        vector<double> resultX;
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
                Histogram1D* h1g = h2->gateY(gate[0], gate[1]);
                Histogram1D* h1b = h2->gateY(bgr[0], bgr[1]);

                if (options_->getBin()) {
                    Histogram1D* h1gb;
                    Histogram1D* h1bb;

                    vector<unsigned> bin;
                    options_->getBinning(bin);
                    if (bin[0] > 1) {
                        h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                            (unsigned)h1g->getnBinX()/bin[0]);
                        h1bb = h1b->rebin(h1b->getxMin(), h1b->getxMax(), 
                                            (unsigned)h1b->getnBinX()/bin[0]);
                    } else
                        throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                    (*h1g) =(*h1gb);
                    (*h1b) =(*h1bb);
                    delete h1gb;
                    delete h1bb;
                }

                unsigned sz = h1g->getnBinX();
                result.resize(sz, 0);
                resultX.resize(sz, 0.0);
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt( (*h1g)[i] + (*h1b)[i] );
                    result[i] = (*h1g)[i] - (*h1b)[i];
                    resultX[i] = h1g->getX(i);
                }

                delete h1b;
                delete h1g;

            } else {
                throw GenError("--bg: not enough gate points");
            }
            //end --gy --bg
        } else if (options_->getSBg()){
            //--gy --sbg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 4) {
                Histogram1D* h1g  = h2->gateY(gate[0], gate[1]);
                Histogram1D* h1b1 = h2->gateY(bgr[0], bgr[1]);
                Histogram1D* h1b2 = h2->gateY(bgr[2], bgr[3]);

                if (options_->getBin()) {
                    // gate, bckg 1, bckg 2 - binned
                    Histogram1D* h1gb;
                    Histogram1D* h1b1b;
                    Histogram1D* h1b2b;

                    vector<unsigned> bin;
                    options_->getBinning(bin);
                    if (bin[0] > 1) {
                        h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                            (unsigned)h1g->getnBinX()/bin[0]);
                        h1b1b = h1b1->rebin(h1b1->getxMin(), h1b1->getxMax(), 
                                            (unsigned)h1b1->getnBinX()/bin[0]);
                        h1b2b = h1b2->rebin(h1b2->getxMin(), h1b2->getxMax(), 
                                            (unsigned)h1b2->getnBinX()/bin[0]);
                    } else
                        throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                    (*h1g) =(*h1gb);
                    (*h1b1) =(*h1b1b);
                    (*h1b2) =(*h1b2b);
                    delete h1gb;
                    delete h1b1b;
                    delete h1b2b;
                }

                unsigned sz = h1g->getnBinX();
                result.resize(sz, 0);
                resultX.resize(sz, 0.0);
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt( (*h1g)[i] + (*h1b1)[i] + (*h1b2)[i] );
                    result[i] = (*h1g)[i] - (*h1b1)[i] - (*h1b2)[i];
                    resultX[i] = h1g->getX(i);
                }

                delete h1b2;
                delete h1b1;
                delete h1g;
            } else {
                throw GenError("--sbg: not enough gate points");
            }
            //end --gy --sbg
        } else {
            // --gy no background
            Histogram1D* h1g  = h2->gateY(gate[0], gate[1]);

            if (options_->getBin()) {
                // gate binned
                Histogram1D* h1gb;
                vector<unsigned> bin;
                options_->getBinning(bin);
                if (bin[0] > 1) {
                    h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                        (unsigned)h1g->getnBinX()/bin[0]);
                } else
                    throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                (*h1g) =(*h1gb);
                delete h1gb;
            }

            unsigned sz = h1g->getnBinX();
            result.resize(sz, 0);
            error.resize(sz, 0.0);
            resultX.resize(sz, 0.0);
            for (unsigned i = 0; i < sz; ++i) {
                error[i] = sqrt( (*h1g)[i] );
                result[i] = (*h1g)[i];
                resultX[i] = h1g->getX(i);
            }

            delete h1g;
            // end --gx no background
        }

        unsigned sz = result.size();
        for (unsigned i = 0; i < sz; ++i)
            cout << resultX[i] << " " << result[i] << " " << error[i] << endl;
        //end --gx
    } else if (options_->getGx()){
        //--gx
        vector<long> result;
        vector<long> resultX;
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
                Histogram1D* h1g = h2->gateX(gate[0], gate[1]);
                Histogram1D* h1b = h2->gateX(bgr[0], bgr[1]);

                if (options_->getBin()) {
                    Histogram1D* h1gb;
                    Histogram1D* h1bb;

                    vector<unsigned> bin;
                    options_->getBinning(bin);
                    if (bin[0] > 1) {
                        h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                            (unsigned)h1g->getnBinX()/bin[0]);
                        h1bb = h1b->rebin(h1b->getxMin(), h1b->getxMax(), 
                                            (unsigned)h1b->getnBinX()/bin[0]);
                    } else
                        throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                    (*h1g) =(*h1gb);
                    (*h1b) =(*h1bb);
                    delete h1gb;
                    delete h1bb;
                }


                unsigned sz = h1g->getnBinX();
                result.resize(sz, 0);
                resultX.resize(sz, 0.0);
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt( (*h1g)[i] + (*h1b)[i] );
                    result[i] = (*h1g)[i] - (*h1b)[i];
                    resultX[i] = h1g->getX(i);
                }

                delete h1b;
                delete h1g;
            } else {
                throw GenError("--bg: not enough gate points");
            }
            //end --gx --bg
        } else if (options_->getSBg()){
            //--gx --sbg
            vector<unsigned> bgr;
            options_->getBgGate(bgr);

            if (bgr.size() >= 4) {
                // gate, bckg 1, bckg 2
                Histogram1D* h1g  = h2->gateX(gate[0], gate[1]);
                Histogram1D* h1b1 = h2->gateX(bgr[0], bgr[1]);
                Histogram1D* h1b2 = h2->gateX(bgr[2], bgr[3]);

                if (options_->getBin()) {
                    // gate, bckg 1, bckg 2 - binned
                    Histogram1D* h1gb;
                    Histogram1D* h1b1b;
                    Histogram1D* h1b2b;

                    vector<unsigned> bin;
                    options_->getBinning(bin);
                    if (bin[0] > 1) {
                        h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                            (unsigned)h1g->getnBinX()/bin[0]);
                        h1b1b = h1b1->rebin(h1b1->getxMin(), h1b1->getxMax(), 
                                            (unsigned)h1b1->getnBinX()/bin[0]);
                        h1b2b = h1b2->rebin(h1b2->getxMin(), h1b2->getxMax(), 
                                            (unsigned)h1b2->getnBinX()/bin[0]);
                    } else
                        throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                    (*h1g) =(*h1gb);
                    (*h1b1) =(*h1b1b);
                    (*h1b2) =(*h1b2b);
                    delete h1gb;
                    delete h1b1b;
                    delete h1b2b;
                }


                unsigned sz = h1g->getnBinX();
                result.resize(sz, 0);
                resultX.resize(sz, 0.0);
                error.resize(sz, 0.0);
                for (unsigned i = 0; i < sz; ++i) {
                    error[i] = sqrt( (*h1g)[i] + (*h1b1)[i] + (*h1b2)[i] );
                    result[i] = (*h1g)[i] - (*h1b1)[i] - (*h1b2)[i];
                    resultX[i] = h1g->getX(i);
                }

                delete h1b2;
                delete h1b1;
                delete h1g;
            } else {
                throw GenError("--sbg: not enough gate points");
            }
            //end --gx --sbg
        } else {
            // --gx no background
            Histogram1D* h1g  = h2->gateX(gate[0], gate[1]);
            if (options_->getBin()) {
                // gate binned
                Histogram1D* h1gb;
                vector<unsigned> bin;
                options_->getBinning(bin);
                if (bin[0] > 1) {
                    h1gb = h1g->rebin(h1g->getxMin(), h1g->getxMax(), 
                                        (unsigned)h1g->getnBinX()/bin[0]);
                } else
                    throw GenError("HisDrrHisto::process1D : Wrong binning size.");
                (*h1g) =(*h1gb);
                delete h1gb;
            }

            unsigned sz = h1g->getnBinX();
            result.resize(sz, 0);
            resultX.resize(sz, 0.0);
            error.resize(sz, 0.0);
            for (unsigned i = 0; i < sz; ++i) {
                error[i] = sqrt( (*h1g)[i] );
                result[i] = (*h1g)[i];
                resultX[i] = h1g->getX(i);
            }

            delete h1g;
            // end --gx no background
        }

        unsigned sz = result.size();
        for (unsigned i = 0; i < sz; ++i)
            cout << resultX[i] << " " << result[i] << " " << error[i] << endl;
        //end --gx
    } else {
        // No gates case
        
        // Rebinning (if applicable)
        if (options_->getBin()) {
            Histogram2D* h2b;
            vector<unsigned> bin;
            options_->getBinning(bin);
            if (bin[0] > 1 && bin[1] > 1)
                h2b = h2->rebin( h2->getxMin(), h2->getxMax(), 
                                h2->getyMin(), h2->getyMax(), 
                                (unsigned)h2->getnBinX()/bin[0],
                                (unsigned)h2->getnBinY()/bin[1] );
            else
                throw GenError("HisDrrHisto::process1D : Wrong binning size.");
            (*h2) =(*h2b);
            delete h2b;
        }

        unsigned szX = h2->getnBinX();
        unsigned szY = h2->getnBinY();

        for (unsigned x = 0; x < szX; ++x) {
            for (unsigned y = 0; y < szY; ++y) {
                cout << h2->getX(x) << " " << h2->getY(y)  
                     << " " << (*h2)(x,y) << endl;
            }
            cout << endl;
        }
        //end no gates
    } 
*/

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
