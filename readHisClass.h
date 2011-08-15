#ifndef HREADHIS2
#define HREADHIS2

#include <vector>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include "hisdrr.h"
#include "drrblock.h"

using namespace std;

/// Holds and loads run flags
/*
 * Class for holding and loading options passed from command line as program
 * run flags
 */
struct Flags {
    string baseName; /// file name without extension
    int hisID; /// histogram ID
    bool gx; /// gating on X axis
    bool gy; /// gating on Y axis
    int g0; /// gate start
    int g1; /// gate end
    bool bg; /// background subtraction
    int b0; /// background start
    int b1; /// background end
    bool info; /// information on histogram
    bool list; /// list of histograms
    int bin; /// bin size

    int flagPos(vector<string> &flags, const string &match); /// flags position in vector
    void loadFlags(vector<string> &flags); /// flags processing
    Flags();
};

///Main class processing histogram
/** 
 *
 */
class ReadHis {
    Flags options; /// Command fine flags
    void gx(long proj[], int sz, int sizeX, vector<unsigned int> &d, int g0, int g1);  /// Gate on X channels
    void gy(long proj[], int sz, int sizeX, vector<unsigned int> &d, int g0, int g1);  /// Gate on Y channels
    void gxbg(long proj[], unsigned int projErr[], int sz, int sizeX, vector<unsigned int> &d, int b0, int b1);/// Bg subtraction
    void gybg(long proj[], unsigned int projErr[], int sz, int sizeX, vector<unsigned int> &d, int b0, int b1);/// Bg subtraction
    void process1D(vector<unsigned int> &d); /// Processing 1D histograms
    void process2D(vector<unsigned int> &d, DrrHisRecordExtended &info); ///Processing 2D histograms
    void bin2D(long proj[], vector<unsigned int> &d, int sX, int sY);/// 2D square bins
    void showInfo(DrrHisRecordExtended hisList); /// Displays info on histogram
    public:
    void process(); /// main procedure for processing
    ReadHis(vector<string> &flags){ options.loadFlags(flags);}
};

#endif
