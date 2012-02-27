/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#include <string> 
#include <cmath> 
#include "DrrBlock.h"
#include "HisDrr.h"
#include "Histogram.h"
#include "Exceptions.h"
#include "Options.h"
#include "HisDrrHisto.h"
#include "Polygon.h"
#include "Debug.h"

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
                cout << "E" << list[i] << "E(" << info.hisDim << ")";
            else
                cout << list[i] << "(" << info.hisDim << ")";
                //Removed because when redirected to file, looks ugly
                //cout << "\033[1;34m" << list[i] << "\033[0m" << "(" << info.hisDim << ")";
        } else {
            cout << list[i];
        }
        
        cout << ", ";
        if ((i+1) % 8 == 0)
            cout << endl;

    }
    cout << endl;
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
        if (bin[0] > 1) {
            unsigned nbX = (unsigned)ceil(h1->getnBinX()/double(bin[0]));
            h1b = h1->rebin(h1->getxMin(), h1->getxMax(), nbX );
            (*h1) =(*h1b);
            delete h1b;
        } else if (bin[0] <= 0)
            throw GenError("HisDrrHisto::process1D : Wrong binning size.");

    }
    
    cout << "#X  N  dN" << endl;
    unsigned sz = h1->getnBinX();
    if (options_->getZeroSup()) {
        for (unsigned i = 0; i < sz; ++i)
            if ((*h1)[i] == 0 )
                cout << h1->getX(i) << " " << (*h1)[i] << " " << sqrt( (*h1)[i] ) << endl;
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
    bool gy = options_->getGy();
    bool bg = options_->getBg();
    bool sbg = options_->getSBg();
    bool pg = options_->getPg();
    
    if ( (gx || gy) && !pg ){
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
            //--gy/gx --bg
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
                //--gy/gx --sbg
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
            if (gx && bin[1] > 1) {
                unsigned nbX = (unsigned)ceil(proj->getnBinX()/double(bin[1]));

                projBin = proj->rebin(proj->getxMin(),
                                      proj->getxMax(),
                                      nbX );
                projErrBin = projErr->rebin(projErr->getxMin(),
                                            projErr->getxMax(), 
                                            nbX );
                (*proj) =(*projBin);
                (*projErr) =(*projErrBin);
                
                delete projBin;
                delete projErrBin;
            } else if (gx && bin[1] <= 0)
                throw GenError("HisDrrHisto::process1D : Wrong binning size.");

            if (gy && bin[0] > 1) {
                unsigned nbX = (unsigned)ceil(proj->getnBinX()/double(bin[0]));
                projBin = proj->rebin(proj->getxMin(),
                                      proj->getxMax(), 
                                      nbX );
                projErrBin = projErr->rebin(projErr->getxMin(),
                                            projErr->getxMax(), 
                                            nbX );
                (*proj) =(*projBin);
                (*projErr) =(*projErrBin);
                
                delete projBin;
                delete projErrBin;
            } else if (gy && bin[0] <= 0)
                throw GenError("HisDrrHisto::process1D : Wrong binning size.");

        }

        unsigned sz = proj->getnBinX();
        //We assume here that 0 counts came from l = 1 Poisson distribution
        for (unsigned i = 0; i < sz; ++i)
            if ((*projErr)[i] == 0)
                (*projErr)[i] = 1;

        cout << "#X  N  dN" << endl;
        for (unsigned i = 0; i < sz; ++i)
            cout << proj->getX(i) << " " << (*proj)[i] << " " << sqrt((*projErr)[i]) << endl;


        delete projErr;
        delete proj;

    } else if ( (gx || gy) && pg ) {
        // Polygon gate
        Polygon* polgate;

        string polFile = options_->getPolygon();
        int coma = polFile.find_last_of(",");
        if (coma != (int)string::npos ) {
            string file = polFile.substr(0, coma);
            string id = polFile.substr(coma + 1);
            cout << "# BAN file " << file << " ban id " << id << endl;
            polgate = new Polygon(file, atoi(id.c_str()));
        } else {
            polgate = new Polygon(polFile);
        }

        unsigned szX = h2->getnBinX();
        unsigned szY = h2->getnBinY();
       
        double min = 0;
        double max = 0;
        unsigned pSz = 0;
        if (gx) {
            min = h2->getyMin();
            max = h2->getyMax();
            pSz = szY;
        } else {
            min = h2->getxMin();
            max = h2->getxMax();
            pSz = szX;
        }
        Histogram1D* proj = new Histogram1D(min, max, pSz, "");

        double xlow, ylow, xhigh, yhigh;
        polgate->rectangle(xlow, ylow, xhigh, yhigh);

        // getiX and getiY safely return 0 or xmax if out of histogram range
        unsigned xmin = h2->getiX(xlow);
        unsigned xmax = h2->getiX(xhigh);
        unsigned ymin = h2->getiY(ylow);
        unsigned ymax = h2->getiY(yhigh);

        for (unsigned x = xmin; x < xmax; ++x)
        for (unsigned y = ymin; y < ymax; ++y) {
            if (polgate->pointIn(h2->getX(x), h2->getY(y)) ) {
                if (gx)
                    proj->add(y, (*h2)(x,y));
                else
                    proj->add(x, (*h2)(x,y));
            }
        }

        cout << "#X  N  dN" << endl;
        for (unsigned i = 0; i < pSz; ++i) {
            cout << proj->getX(i) << " " << (*proj)[i];
            if ((*proj)[i] == 0)
                cout << " " << 1 << endl;
            else
                cout << " " << sqrt((*proj)[i]) << endl;
        }
                    
        delete proj;
        delete polgate;
    } else {
        // No gates case
        
        // Rebinning (if applicable)
        if (options_->getBin()) {
            Histogram2D* h2b;
            vector<unsigned> bin;
            options_->getBinning(bin);

            if ( !(bin[0] <= 1 && bin[1] <= 1) && 
                  (bin[0] > 0  && bin[1] > 0 )      ) {
                unsigned nbX = (unsigned)ceil(h2->getnBinX()/double(bin[0]));
                unsigned nbY = (unsigned)ceil(h2->getnBinY()/double(bin[1]));
                h2b = h2->rebin( h2->getxMin(), h2->getxMax(), 
                                 h2->getyMin(), h2->getyMax(), 
                                 nbX, nbY );
                (*h2) =(*h2b);
                delete h2b;
            } else
                throw GenError("HisDrrHisto::process2D : Wrong binning size.");
        }

        unsigned szX = h2->getnBinX();
        unsigned szY = h2->getnBinY();

        cout << "#X  Y  N" << endl;
        for (unsigned x = 0; x < szX; ++x) {
            for (unsigned y = 0; y < szY; ++y) {
                cout << h2->getX(x) << " " << h2->getY(y)  
                     << " " << (*h2)(x,y) << endl;
            }
            cout << endl;
        }
       
    } //end no gates
    //*******************************************************
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
