/*
* K. Miernik (k.a.miernik@gmail.com)
* 
* Definition file for His_drr class 
*/

#ifndef HHISDRR
#define HHISDRR

#include <vector>
#include <string>
#include "DrrBlock.h" 
using namespace std;

/// This structure is used for creating new his and drr files
/**
 * It consist of fields present in a input file and resembles the
 * structure of DrrHisRecord
 */
struct SimpleDrrBlock {
    int hisId;
    short halfWords;
    short scaled[2];
    string title;
};

class HisDrr {
    public:
        //! Constructor loading data from .drr
        HisDrr(string& drr, string& his);

        //! Constructor creating and opening new his and drr using definition from input file,
        HisDrr(string& drr, string& his, string& input);

        //! returns specified histogram data
        // Return by reference version
        void getHistogram(vector<unsigned int>& data, int id);

        //! return drr data on specified histogram
        DrrHisRecordExtended getHistogramInfo(int id);

        //! return histograms id's list 
        void getHisList(vector<int>& hisList);

        //! zeroes data for a given histogram
        void zeroHistogram(int id);

        //! replaces in histogram 'id' point 'i' by 'value'
        //** There are two versions for 4 and 2 -bytes long word in file
        void setValue(const int id, unsigned int pos, unsigned value);
        void setValue(const int id, unsigned int pos, unsigned short value);

        //! replaces histogram id values by ones given in a vector 
        //** There are two versions for 4 and 2  -bytes long word in file
        void setValue(const int id, vector<unsigned>& value);
        void setValue(const int id, vector<unsigned short>& value);

    private:
        //! Vector holding all the histogram info read from drr file
        vector<DrrHisRecordExtended> hisList;

        //! .drr file containing information about .his structure
        string drrFile;

        //! .his containg data
        string hisFile;

        //! reads block of data from drr file
        void readBlock(ifstream& file, drrBlock* block);

        //! function loading .drr file and filling in spectrum vector
        void loadDrr();
};

#endif
