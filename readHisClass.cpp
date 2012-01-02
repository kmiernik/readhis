#include "readHisClass.h"
#include <cmath>
#include <sys/time.h>


Flags::Flags() {
    baseName = "";
    hisID = -1;
    gx = false;
    gy = false;
    g0 = 0;
    g1 = 0;
    bg = false;
    sbg = false;
    b0 = 0;
    b1 = 0;
    b2 = 0;
    b3 = 0;
    info = false;
    list = false;
    listEmpty = false;
    bin = 1;
    binX = 1;
    binY = 1;
    zero = false;
}

int Flags::flagPos(vector<string> &flags, const string &match) {
    for (unsigned int i = 0; i < flags.size(); i++)
        if (flags[i] == match)
            return i;
    return -1;
}


void Flags::loadFlags(vector<string> &flags) {
    unsigned int dot = flags[0].find_last_of(".");
    baseName = flags[0].substr(0,dot);

    int flagL = flagPos(flags, "-l");
    int flagLE = flagPos(flags, "-L");
    if (flagL >= 1) {
        list = true;
    }
    else if (flagLE >= 1) {
        listEmpty = true;
    }
    else {
        hisID = atoi(flags[1].c_str());
    }
    
    int flagGx = flagPos(flags, "-gx");
    if (flagGx > 1) {
        if (flagGx+2 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process Y projection";
            string msg = err.str();
            throw GenError(msg);
        }
        gx = true;
        g0 = atoi(flags[flagGx+1].c_str());
        g1 = atoi(flags[flagGx+2].c_str());
    }

    int flagGy = flagPos(flags, "-gy");
    if ((!gx)&&(flagGy > 1)) {
        if (flagGy+2 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process X projection";
            string msg = err.str();
            throw GenError(msg);
        }
        gy = true;
        g0 = atoi(flags[flagGy+1].c_str());
        g1 = atoi(flags[flagGy+2].c_str());
    }

    int flagBg = flagPos(flags, "-bg");
    if ( (flagGx||flagGy)&&(flagBg > 1)) {
        if (flagBg+2 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process background subtraction";
            string msg = err.str();
            throw GenError(msg);
        }
        bg = true;
        b0 = atoi(flags[flagBg+1].c_str());
        b1 = atoi(flags[flagBg+2].c_str());
    }

    int flagSbg = flagPos(flags, "-sbg");
    if ( (flagGx||flagGy)&&(flagSbg > 1)) {
        if (flagSbg+4 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process background subtraction";
            string msg = err.str();
            throw GenError(msg);
        }
        sbg = true;
        b0 = atoi(flags[flagSbg+1].c_str());
        b1 = atoi(flags[flagSbg+2].c_str());
        b2 = atoi(flags[flagSbg+3].c_str());
        b3 = atoi(flags[flagSbg+4].c_str());
    }

    int flagBin = flagPos(flags, "-b");
    if (flagBin > 1) {
        if (flagBin+1 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process binning";
            string msg = err.str();
            throw GenError(msg);
        }
        bin = atoi(flags[flagBin+1].c_str());
        if (bin < 1) {
            stringstream err;
            err << "Wrong bin size value";
            string msg = err.str();
            throw GenError(msg);
        }
    }

    int flagBin2d = flagPos(flags, "-B");
    if (flagBin2d > 1) {
        if (flagBin2d+2 > (int)(flags.size()-1)) {
            stringstream err;
            err << "Not enough flags to process binning";
            string msg = err.str();
            throw GenError(msg);
        }
        binX = atoi(flags[flagBin2d+1].c_str());
        binY = atoi(flags[flagBin2d+2].c_str());
        if (binX < 1 || binY < 1) {
            stringstream err;
            err << "Wrong bin size value";
            string msg = err.str();
            throw GenError(msg);
        }
    }

    int flagInf = flagPos(flags, "-i");
    if (flagInf > 1) 
        info = true;
    else
        info = false;

    int flagZ = flagPos(flags, "-z");
    if (flagZ >= 1) 
        zero = true;
    else
        zero = false;
}

void ReadHis::showInfo(DrrHisRecordExtended hisList) {
    cout << "#ID: " << hisList.hisID << endl;
    cout << "#hisDim: " << hisList.hisDim << endl;
    cout << "#halfWords: " << hisList.halfWords << endl;
    for (int j = 0; j < 4; j++)
        cout << "#params[" << j << "]: " << hisList.params[j] << endl;
    for (int j = 0; j < 4; j++)
        cout << "#raw[" << j << "]: " << hisList.raw[j] << endl;
    for (int j = 0; j < 4; j++)
        cout << "#scaled[" << j << "]: " << hisList.scaled[j] << endl;
    for (int j = 0; j < 4; j++)
        cout << "#minc[" << j << "]: " << hisList.minc[j] << endl;
    for (int j = 0; j < 4; j++)
        cout << "#maxc[" << j << "]: " << hisList.maxc[j] << endl;

    cout << "#offset: " << hisList.offset << endl;
    cout << "#xlabel: " << hisList.xlabel << endl;
    cout << "#ylabel: " << hisList.ylabel << endl;

    for (int j = 0; j < 4; j++)
        cout << "#calcon[" << j << "]: " << hisList.calcon[j] << endl;

    cout << "#title: " << hisList.title << endl;
}

void ReadHis::process1D(vector<unsigned int> &d) {
    if (options.bin == 1) {

        cSpectrum.clear();
        cSpectrum.reserve(d.size()/options.bin);
        cSpectrum.resize(d.size()/options.bin, 0);

        cError.clear();
        cError.reserve(d.size()/options.bin);
        cError.resize(d.size()/options.bin, 0);

        unsigned sz = d.size();

        for (unsigned int x = 0; x < sz; x++) {
            cSpectrum[x] = d[x];
            cError[x] = sqrt(d[x]);
        }
    }
    else {

        cSpectrum.clear();
        cSpectrum.reserve(d.size());
        cSpectrum.resize(d.size(), 0);

        cError.clear();
        cError.reserve(d.size());
        cError.resize(d.size(), 0);

        unsigned sz = d.size();

        for (unsigned int x = 0; x < sz; x++) {
            cSpectrum[x/options.bin] += d[x];
            cError[x] += cSpectrum[x];
        }
        for (unsigned i = 0; i < sz; i++)
            cError[i] = sqrt(cError[i]);

    }
}

void ReadHis::gx(vector<unsigned int> &d, int g0, int g1) {
    int sz = cSpectrum.size();
    for (int x = g0; x < g1+1; x++) 
        for (int y = 0; y < sz*options.bin; y++) {
            cSpectrum[y/options.bin] += d[x+sizeX*y];
            cError[y/options.bin] += d[x+sizeX*y];
        }
}

void ReadHis::gy(vector<unsigned int> &d, int g0, int g1) {
    for (int y = g0; y < g1+1; y++)
        for (unsigned x = 0; x < sizeX; x++) {
            cSpectrum[x/options.bin] += d[x+sizeX*y];
            cError[x/options.bin] += d[x+sizeX*y];
        }
}


void ReadHis::gxbg(vector<unsigned int> &d, int b0, int b1) {
    for (int x = b0; x < b1+1; x++)
        for (unsigned y = 0; y < sizeY; y++) {
            cSpectrum[y/options.bin] -= d[x+sizeX*y];
            cError[y/options.bin] += d[x+sizeX*y];
        }
}

void ReadHis::gybg(vector<unsigned int> &d, int b0, int b1) {
    for (int y = b0; y < b1+1; y++)
        for (unsigned x = 0; x < sizeX; x++) {
            cSpectrum[x/options.bin] -= d[x+sizeX*y];
            cError[x/options.bin] += d[x+sizeX*y];
        }
}

void ReadHis::bin2D(vector<unsigned int> &d) {
    for (unsigned y = 0; y < sizeY/options.binY; y++)
        for (unsigned x = 0; x < sizeX/options.binX; x++)
            for (int by = 0; by < options.binY; by++)
                for (int bx = 0; bx < options.binX; bx++) {
                        cSpectrum[x+y*(sizeX-sizeX%options.binX)/options.binX] 
                                    += d[x*options.binX+bx + (y*options.binY+by)*(sizeX)];
                        cError[x+y*(sizeX-sizeX%options.binX)/options.binX] 
                                    += d[x*options.binX+bx + (y*options.binY+by)*(sizeX)];
                }
}

void ReadHis::process2D(vector<unsigned int> &d, DrrHisRecordExtended &info) {
    int sz;
    if (options.gy)
        sz = sizeX/options.bin;
    else if (options.gx)
        sz = sizeY/options.bin;
    else
        sz = sizeX/options.binX*sizeY/options.binY;
    
    cSpectrum.clear();
    cSpectrum.reserve(sz);
    cSpectrum.resize(sz, 0);
    cError.clear();
    cError.reserve(sz);
    cError.resize(sz, 0);

    if (options.gy) {
        gy(d, options.g0, options.g1);

        if (options.bg || options.sbg)
            gybg(d, options.b0, options.b1); 
        // if split background was set, first part was subtracted in previous step, now second part of bg
        if (options.sbg)
            gybg(d, options.b2, options.b3);
    }       
    else if (options.gx) {
        gx(d, options.g0, options.g1);
        if (options.bg || options.sbg)
            gxbg(d, options.b0, options.b1);
        if (options.sbg)
            gxbg(d, options.b2, options.b3);
    } else {
        bin2D(d);
    }
    for (int i = 0; i < sz; i++)
        cError[i] = sqrt(cError[i]);
}

void ReadHis::process() {
    try {
        string drr = options.baseName + ".drr";
        string his = options.baseName + ".his";
        HisDrr *h = new HisDrr(drr, his);
    
        if (options.list) {
            vector<int> list;
            h->getHisList(list);
            unsigned sz = list.size();
            for (unsigned int i = 0; i < sz; i++) {
                cout << list[i] << ", ";
                if ((i+1) % 10 == 0)
                    cout << endl;

            }
            cout << endl;
        } else if (options.listEmpty) {
            vector<int> list;
            vector<unsigned int> d;
            h->getHisList(list);
            for (unsigned int i = 0; i < list.size(); i++) {
                h->getHistogram(d, list[i]);
                unsigned int sz = d.size();
                bool empty = true;
                for (unsigned int j = 0; j < sz; j++)
                    if (d[j] > 0) {
                        empty = false;
                        break;
                    }
                if (empty)
                    cout << "E" << list[i];
                else
                    cout << "\033[1;34m" << list[i] << "\033[0m";
                cout << ", ";
                if ((i+1) % 10 == 0)
                    cout << endl;

            }
            cout << "\033[0;30m\033[0m" << endl;
        }
        else { 
            DrrHisRecordExtended info = h->getHistogramInfo(options.hisID);
            hisDim = info.hisDim;

            if (options.info)
                showInfo(info);
            else {
                vector<unsigned int> d;
                h->getHistogram(d, options.hisID);
                //cout << "#Histogram ID: " << info.hisID << ", " << info.hisDim << "D" << endl;
                sizeX = info.scaled[0];
                sizeY = info.scaled[1];

                if (info.hisDim == 1)
                    process1D(d);
                else if (info.hisDim == 2) {
                    process2D(d, info);
                }
                else {
                    stringstream err;
                    err << "Histograms of dimension larger then 2 not supported";
                    string msg = err.str();
                    throw GenError(msg);
                }
            }
       }
       delete h;
   } catch (IOError &err) {
        cout << "I/O Error: " << err.show() << endl;
   } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
   }
}
