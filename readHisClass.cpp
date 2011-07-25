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
    b0 = 0;
    b1 = 0;
    info = false;
    list = false;
    bin = 1;
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

    int flagInf = flagPos(flags, "-i");
    if (flagInf > 1) 
        info = true;
    else
        info = false;

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
            cout << x << " " << d[x] << endl;
    }
    else {
        long *proj = new long[d.size()/options.bin]();
        for (unsigned int x = 0; x < d.size(); x++)
            proj[x/options.bin] += d[x];
        for (unsigned int x = 0; x < d.size()/options.bin; x++)
            cout << x << " " << proj[x] << " " << sqrt(proj[x]) << endl;
        delete []proj;
    }
}

void ReadHis::gx(long proj[], int sz, vector<unsigned int> &d, int g0, int g1) {
    cout << "#Projection on Y axis, gate on X " << g0 << " to " << g1 << endl;
    for (int x = g0; x < g1+1; x++)
        for (int y = 0; y < sz*options.bin; y++) 
            proj[y/options.bin] += d[x+sz*options.bin*y];
}

void ReadHis::gy(long proj[], int sz, vector<unsigned int> &d, int g0, int g1) {
    cout << "#Projection on X axis, gate on Y " << g0 << " to " << g1 << endl;
    for (int y = g0; y < g1+1; y++)
        for (int x = 0; x < sz*options.bin; x++) 
            proj[x/options.bin] += d[x+sz*options.bin*y];
}


void ReadHis::gxbg(long proj[], unsigned int projErr[], int sz, vector<unsigned int> &d, int b0, int b1) {
    cout << "#Background substracted, gate on X " << b0 << " to " << b1 << endl;
    for (int x = b0; x < b1+1; x++)
        for (int y = 0; y < sz*options.bin; y++) {
            proj[y/options.bin] -= d[x+sz*options.bin*y];
            projErr[y/options.bin] += d[x+sz*options.bin*y];
        }
}

void ReadHis::gybg(long proj[], unsigned int projErr[], int sz, vector<unsigned int> &d, int b0, int b1) {
    cout << "#Background substracted, gate on Y " << b0 << " to " << b1 << endl;
    for (int y = b0; y < b1+1; y++)
        for (int x = 0; x < sz*options.bin; x++) {
            proj[x/options.bin] -= d[x+sz*options.bin*y];
            projErr[x/options.bin] += d[x+sz*options.bin*y];
        }
}

void ReadHis::bin2D(long proj[], vector<unsigned int> &d, int sX, int sY) {
    for (int y = 0; y < sY/options.bin; y++)
        for (int x = 0; x < sX/options.bin; x++)
            for (int by = 0; by < options.bin; by++)
                for (int bx = 0; bx < options.bin; bx++) 
                        proj[x+y*(sX-sX%options.bin)/options.bin] += d[x*options.bin+bx + (y*options.bin+by)*(sX)];
}

void ReadHis::process2D(vector<unsigned int> &d, DrrHisRecordExtended &info) {
    int sz;
    if (options.gy)
        sz = info.scaled[0]/options.bin;
    else if (options.gx)
        sz = info.scaled[1]/options.bin;
    else
        sz = info.scaled[0]/options.bin*info.scaled[1]/options.bin;
    
    // Initialization of dynamic array to 0
    long *proj;
    proj = new long[sz]();
    // for the purpose of calculating projection error bars,
    // since during background subtraction they are no more sqrt(N) but sum of N1 and N2
    // where N1 is peak gate and N2 is background gate
    unsigned int *projErr;
    projErr = new unsigned int[sz]();

    if (options.gy) {
        gy(proj, sz, d, options.g0, options.g1);
        for (int i = 0; i < sz; i++) // for "normal" gate errors are calc. on number of counts basis
            projErr[i] = proj[i];
        if (options.bg)
            gybg(proj, projErr, sz, d, options.b0, options.b1); // errors are no logner simple number of counts
        cout << "#Channel Counts Err" << endl;
        for (int i = 0; i < sz; i++)
            cout << i << " " << proj[i] << " " << sqrt(projErr[i]) << endl;
    }       
    else if (options.gx) {
        gx(proj, sz, d, options.g0, options.g1);
        for (int i = 0; i < sz; i++)
            projErr[i] = proj[i];
        if (options.bg)
            gxbg(proj, projErr, sz, d, options.b0, options.b1);
        cout << "#Channel Counts" << endl;
        for (int i = 0; i < sz; i++)
            cout << i << " " << proj[i] << " " << sqrt(projErr[i]) << endl;
    } else {
            cout << "#XCh YCh Counts" << endl;
            bin2D(proj, d, info.scaled[0], info.scaled[1]);
            for (int y = 0; y < info.scaled[1]/options.bin; y++) {
                for (int x = 0; x < info.scaled[0]/options.bin; x++) 
                    cout << x <<" "<< y <<" "<< proj[x+(info.scaled[0]/options.bin)*y] << endl;
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
