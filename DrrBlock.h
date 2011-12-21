/*
* K. Miernik (k.a.miernik@gmail.com)
* 
* Definition file for His_drr class 
*/

#ifndef HDRRBLOCK
#define HDRRBLOCK

using namespace std;

//// drr file header structure
/**
 * drr files start with a header the struct itself is of 84 bytes lenght
 * but it is followed by 44 bytes of garbage (leftovers from previous file?)
 * to make it round 128 bytes
 */
struct DrrHeader {
    //! string HHIRFDIR0001
    char initial[12];
    //! Number of histograms in file
    int nHis;
    //! Number of half-words (2 bytes) in file
    int nHWords;
    //! Date 0 YY MM DD HR MN
    int date[6];
    //! Field for text description 
    char description[40];
};

/// drr block data on histogram
/**  
 * Structure holding information on specific histgram in .his file
 * it is 128 bytes long and follows structure of drr file
 */
struct DrrHisRecord {
    //! Number of dimensions
    short hisDim;
    //! Number of half-words (2 bytes) per channel
    short halfWords;
    //! Parameter id numbers, for each dimension (up to 4)
    short params[4];
    //! Raw length
    short raw[4];
    //! Scaled length
    short scaled[4];
    //! Min channel number
    short minc[4];
    //! Max channel number
    short maxc[4];
    //! Location in his file (in 2-bytes units)
    int offset;
    //! X axis label
    char xlabel[12];
    //! Y axis label
    char ylabel[12];
    //! Calibration for X axis
    float calcon[4];
    //! Title
    char title[40];
};

/// Drr block on histogram plus hisID 
/**
 * Structure extending DrrHisRecord by hisID which is found at the end of
 * drr file (block of 128 contains 32 record -> 4 bytes long id's)
 */
struct DrrHisRecordExtended : DrrHisRecord {
    //! Histogram ID (unique)
    int hisID;
    //! Overloaded operator= in case when we want to do DrrHisRecordExt = DrrHisRecord
    // (which is very convenient)
    DrrHisRecordExtended& operator=(const DrrHisRecord& right) {
        hisDim = right.hisDim;
        halfWords = right.halfWords;
        for (int i = 0; i < 4; i++)
            params[i] = right.params[i];
        for (int i = 0; i < 4; i++)
            raw[i] = right.raw[i];
        for (int i = 0; i < 4; i++)
            scaled[i] = right.scaled[i];
        for (int i = 0; i < 4; i++)
            minc[i] = right.minc[i];
        for (int i = 0; i < 4; i++)
            maxc[i] = right.maxc[i];
        offset = right.offset;
        for (int i = 0; i < 12; i++)
            xlabel[i] = right.xlabel[i];
        for (int i = 0; i < 12; i++)
            ylabel[i] = right.ylabel[i];
        for (int i = 0; i < 4; i++)
            calcon[i] = right.calcon[i];
        for (int i = 0; i < 40; i++)
            title[i] = right.title[i];
        return *this;
  }

};

/// Union to read block of data from drr file
/**
 * This union defines block of data to be read from drr file.
 * Drr file is build using block of 128 bytes.
 * Compiler will reserve space for the larger of two structs (DrrHisRecord)
 * which is 128 bytes long. This way we can always read a block of data and
 * not worry about 44 bytes garbage found after header.
 */
union drrBlock {
    //! drr file header 128 bytes long but in reality only 84 bytes contain information
    DrrHeader header;
    //! drr histogram record 128 bytes long
    DrrHisRecord record; 
};

#endif
