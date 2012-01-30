/*
* K. Miernik (k.a.miernik@gmail.com)
* 
* Definition file for HisDrr class 
*/

#ifndef HISDRR_H
#define HISDRR_H

#include <vector>
#include <string>
#include <fstream>
#include "DrrBlock.h" 
using namespace std;

/// This structure is used for creating new his and drr files
/**
 * It consist of fields present in a input file and resembles the
 * structure of DrrHisRecord
 */
struct SimpleDrrBlock {
    int hisID;
    short halfWords;
    short scaled[2];
    string title;
};

class HisDrr {
    //! Vector holding all the histogram info read from drr file
    vector<DrrHisRecordExtended> hisList;
    //! .drr file containing information about .his structure
    fstream* drrFile;
    //! .his containg data
    fstream* hisFile;
    //! reads block of data from drr file
    void readBlock(drrBlock *block);
    //! function loading .drr file and filling in spectrum vector
    void loadDrr();

    public:
    //! Constructor taking fstreams 
    HisDrr(fstream* drr, fstream* his);
    //! Constructor taking names and opening fstreams
    HisDrr(const string &drr, const string &his);
    //! Constructor creating and opening new his and drr using definition from input file,
    HisDrr(const string &drr, const string &his, const string &input);

    ~HisDrr() {
        drrFile->close();
        hisFile->close();
        delete drrFile;
        delete hisFile;
    }
    //! returns specified histogram data
      // Return by value version
    //vector<unsigned int> getHistogram(int id);
      // Return by reference version
    void getHistogram(vector<unsigned int> &rtn, int id);
    //! return drr data on specified histogram
    DrrHisRecordExtended getHistogramInfo(int id) const;
    //! return histograms id's list 
    void getHisList(vector<int> &r);
    //! zeroes data for a given histogram
    void zeroHistogram(int id);
    //! replaces in histogram 'id' point 'i' by 'value'
    //** There are two versions for 4 and 2 -bytes long word in file
    void setValue(const int id, unsigned int pos, unsigned int value);
    void setValue(const int id, unsigned int pos, unsigned short value);
    //! replaces histogram id values by ones given in a vector 
    //** There are two versions for 4 and 2  -bytes long word in file
    void setValue(const int id, vector<unsigned int> &value);
    void setValue(const int id, vector<unsigned short> &value);

};

#endif
