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
    unsigned int dot = flags[0].find_first_of(".");
    baseName = flags[0].substr(0,dot);

    int flagL = flagPos(flags, "-l");
    if (flagL >= 1) {
        list = true;
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
    cout << "#Ch Counts Err" << endl;
    cout << "#Bin size: " << options.bin << endl;
    if (options.bin == 1) {
        for (unsigned int x = 0; x < d.size(); x++)
            if (!(options.zero && d[x] == 0))
                cout << x << " " << d[x] << " " << sqrt(d[x]) << endl;
    }
    else {
        long *proj = new long[d.size()/options.bin]();
        for (unsigned int x = 0; x < d.size(); x++)
            proj[x/options.bin] += d[x];
        for (unsigned int x = 0; x < d.size()/options.bin; x++)
            if (!(options.zero && proj[x] == 0))
                cout << x * options.bin + 0.5 * (options.bin - 1) 
                     << " " << proj[x] << " " << sqrt(proj[x]) << endl;
        delete []proj;
    }
}

void ReadHis::gx(long proj[], int sz, int sizeX, vector<unsigned int> &d, int g0, int g1) {
    cout << "#Projection on Y axis, gate on X " << g0 << " to " << g1 << endl;
    for (int x = g0; x < g1+1; x++) 
        for (int y = 0; y < sz*options.bin; y++) 
            proj[y/options.bin] += d[x+sizeX*y];
}

void ReadHis::gy(long proj[], int sz, int sizeX, vector<unsigned int> &d, int g0, int g1) {
    cout << "#Projection on X axis, gate on Y " << g0 << " to " << g1 << endl;
    for (int y = g0; y < g1+1; y++)
        for (int x = 0; x < sizeX; x++) 
            proj[x/options.bin] += d[x+sizeX*y];
}


void ReadHis::gxbg(long proj[], unsigned int projErr[], int sz, int sizeX, vector<unsigned int> &d, int b0, int b1) {
    cout << "#Background substracted, gate on X " << b0 << " to " << b1 << endl;
    for (int x = b0; x < b1+1; x++)
        for (int y = 0; y < sz*options.bin; y++) {
            proj[y/options.bin] -= d[x+sizeX*y];
            projErr[y/options.bin] += d[x+sizeX*y];
        }
}

void ReadHis::gybg(long proj[], unsigned int projErr[], int sz, int sizeX, vector<unsigned int> &d, int b0, int b1) {
    cout << "#Background substracted, gate on Y " << b0 << " to " << b1 << endl;
    for (int y = b0; y < b1+1; y++)
        for (int x = 0; x < sizeX; x++) {
            proj[x/options.bin] -= d[x+sizeX*y];
            projErr[x/options.bin] += d[x+sizeX*y];
        }
}

void ReadHis::bin2D(long proj[], vector<unsigned int> &d, int sX, int sY) {
    for (int y = 0; y < sY/options.binY; y++)
        for (int x = 0; x < sX/options.binX; x++)
            for (int by = 0; by < options.binY; by++)
                for (int bx = 0; bx < options.binX; bx++) {
                        proj[x+y*(sX-sX%options.binX)/options.binX] += d[x*options.binX+bx + (y*options.binY+by)*(sX)];
                        }
}

void ReadHis::process2D(vector<unsigned int> &d, DrrHisRecordExtended &info) {
    int sz;
    if (options.gy)
        sz = info.scaled[0]/options.bin;
    else if (options.gx)
        sz = info.scaled[1]/options.bin;
    else
        sz = info.scaled[0]/options.binX*info.scaled[1]/options.binY;
    cout << "# projection size : " << sz << endl;
    
    // Initialization of dynamic array to 0
    long *proj;
    proj = new long[sz]();
    // for the purpose of calculating projection error bars,
    // since during background subtraction they are no more sqrt(N) but sum of N1 and N2
    // where N1 is peak gate and N2 is background gate
    unsigned int *projErr;
    projErr = new unsigned int[sz]();

    if (options.gy) {
        gy(proj, sz, info.scaled[0], d, options.g0, options.g1);
        for (int i = 0; i < sz; i++) // for "normal" gate errors are calc. on number of counts basis
            projErr[i] = proj[i];
        if (options.bg || options.sbg)
            gybg(proj, projErr, sz, info.scaled[0], d, options.b0, options.b1); // errors are no logner simple number of counts
        // if split background was set, first part was subtracted in previous step, now second part of bg
        if (options.sbg)
            gybg(proj, projErr, sz, info.scaled[0], d, options.b2, options.b3); // errors are no logner simple number of counts

        cout << "#Channel Counts Err" << endl;
        for (int i = 0; i < sz; i++)
            if (!(options.zero && proj[i] == 0))
                cout << i << " " << proj[i] << " " << sqrt(projErr[i]) << endl;
    }       
    else if (options.gx) {
        gx(proj, sz, info.scaled[0], d, options.g0, options.g1);
        for (int i = 0; i < sz; i++)
            projErr[i] = proj[i];
        if (options.bg || options.sbg)
            gxbg(proj, projErr, sz, info.scaled[0], d, options.b0, options.b1);
        if (options.sbg)
            gxbg(proj, projErr, sz, info.scaled[0], d, options.b2, options.b3);
        cout << "#Channel Counts" << endl;
        for (int i = 0; i < sz; i++)
            if (!(options.zero && proj[i] == 0))
                cout << i << " " << proj[i] << " " << sqrt(projErr[i]) << endl;
    } else {
            cout << "#XCh YCh Counts" << endl;
            bin2D(proj, d, info.scaled[0], info.scaled[1]);
            for (int y = 0; y < info.scaled[1]/options.binY; y++) {
                for (int x = 0; x < info.scaled[0]/options.binX; x++) 
                        cout << x * options.binX + 0.5 * (options.binX - 1) << " "
                             << y * options.binY + 0.5 * (options.binY - 1) << " "
                             << proj[x+(info.scaled[0]/options.binX)*y] << endl;
                cout << endl;
            }
     }

    delete []proj;
    delete []projErr;
}

void ReadHis::process() {
    try {
        string drr = options.baseName + ".drr";
        string his = options.baseName + ".his";
        HisDrr *h = new HisDrr(drr, his);
    
        if (options.list) {
            vector<int> list;
            h->getHisList(list);
            for (unsigned int i = 0; i < list.size()-1; i++)
                cout << list[i] << ", ";
            cout << list[list.size()-1] << endl;
        }
        else { 
            DrrHisRecordExtended info = h->getHistogramInfo(options.hisID);
            // Return by value version
            // vector<unsigned int> d = h->getHistogram(options.hisID);
            // Return by reference version
            vector<unsigned int> d;
            h->getHistogram(d, options.hisID);
            cout << "#Histogram ID: " << info.hisID << ", " << info.hisDim << "D" << endl;

            if (options.info)
                showInfo(info);
            else {
                if (info.hisDim == 1)
                    process1D(d);
                else if (info.hisDim == 2)
                    process2D(d, info);
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
