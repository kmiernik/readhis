/*
 * K. Miernik (k.a.miernik@gmail.com) 2012
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
/**
 * Class for storing options for readhis. Options are loaded from
 * command line by readhis and applied here. The HisDrrHisto requires 
 * pointer to Options in ctor.
 */

class Options {
    public:
        /** Ctor. */
        Options();

        /** Returns histogram id.*/
        unsigned getHisId () const ;

        /** Sets histogram id. Returns true if id number is within damm limits.*/
        bool setHisId (unsigned hisId);

        /** Returns true id hisId_ is set correctly.*/
        bool isIdSet() const;

        /** Returns true is list mode is set.*/
        bool getListMode() const;

        /** Sets list mode.*/
        void setListMode (bool b); 

        /** Returns true if verbose list mode is set.*/
        bool getListModeZ() const;

        /** Sets verbose list mode. Unsets list mode if b = true.*/
        void setListModeZ (bool b);


        /** Returns true if info mode is set.*/
        bool getInfoMode() const;

        /** Sets info mode.*/
        void setInfoMode (bool b);

        /** Returns true if zero suppresion mode is set.*/
        bool getZeroSup() const;

        /** Sets zero suppresion mode.*/
        void setZeroSup (bool b);
        
        /** Returns true if gx flag is set.*/
        bool getGx() const;

        /** Sets gx flag, and gate limits. Makes gates limit check. Unsets gy if succesful. */
        bool setGx (bool isGx, unsigned g0, unsigned g1);

        /** Returns true if gy flag is set .*/
        bool getGy() const;

        /** Sets gy flag, and gate limits. Makes gates limit check. Unsets gx if succesful. */
        bool setGy (bool isGy, unsigned g0, unsigned g1);

        /** Returns true if bg flag is set.*/
        bool getBg() const;

        /** Sets bg flag, and gate limits. Makes gates limit check. Unsets sbg if succesful. */
        bool setBg (bool isBg, unsigned b0, unsigned b1);
        
        /** Returns true if sbg flag is set.*/
        bool getSBg() const;

        /** Sets sbg flag, and gate limits. Makes gates limit check. Unsets bg if succesful. */
        bool setSBg (bool isSBg, unsigned b0, unsigned b1,
                                 unsigned b2, unsigned b3);
        
        /** Returns true if bin flag is set.*/
        bool getBin() const;

        /** Sets binning size. If only b0 is passed, the b1 is equal to b0.*/
        bool setBin (bool isBin, unsigned b0, unsigned b1 = 0);

        /** Returns by reference vector containing gates.*/
        void getGate(std::vector<unsigned>& rtn) const;

        /** Returns by reference vector containing background gates.*/
        void getBgGate(std::vector<unsigned>& rtn) const;

        /** Returns by reference vector containing binning size.*/
        void getBinning(std::vector<unsigned>& rtn) const;


    private:
        /** Histogram id.*/
        unsigned hisId_;

        /** Is true when hisId_ is set. */
        bool isIdSet_;

        /** --list flag. List mode outputs list of histogram in a file. */
        bool isListMode_;

        /** --List flag. More verbose version of list mode.  */
        bool isListModeZ_;

        /** Info mode outputs detailed histogram infomation instead of data.*/
        bool isInfoMode_;
        
        /** Zero suppresion flag means that for 1D histogram when number of 
         * count in a bin is 0 it will be skipped in the output.*/
        bool isZeroSup_;

        /** --gx flag. For 2D histograms, sets projection on Y axis gated on
         * X bins. 
         */
        bool isGx_;

        /** --gy flag. For 2D histograms, sets projection on X axis gated on
         * Y bins. 
         */
        bool isGy_;

        /** --bg flag. For --gx or --gy, sets background subtraction gate. */
        bool isBg_;

        /** --sbg flag. For --gx or --gy, sets split (two parts) background
         * subtraction gate
         */
        bool isSBg_;

        /** --bin flag. Binning mode.*/ 
        bool isBin_;

        /** For --gx or --gy. Stores gates. */
        std::vector<unsigned> g_;

        /** For --bg or --sbg. Stores background gates */
        std::vector<unsigned> b_;

        /** For --bin. Stores binning size.*/
        std::vector<unsigned> bin_;
};

#endif
