/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
#include <string>
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
        void setListMode (bool b = true); 

        /** Returns true if verbose list mode is set.*/
        bool getListModeZ() const;

        /** Sets verbose list mode. Unsets list mode if b = true.*/
        void setListModeZ (bool b = true);

        /** Returns true if info mode is set.*/
        bool getInfoMode() const;

        /** Sets info mode.*/
        void setInfoMode (bool b = true);

        /** Returns true if zero suppresion mode is set.*/
        bool getZeroSup() const;

        /** Sets zero suppresion mode.*/
        void setZeroSup (bool b = true);
        
        /** Returns true if gx flag is set.*/
        bool getGx() const;

        /** Sets gx flag, and gate limits. Makes gates limit check. Unsets gy if succesful. */
        bool setGx (unsigned g0, unsigned g1, bool isGx = true);

        /** Returns true if gy flag is set .*/
        bool getGy() const;

        /** Sets gy flag, and gate limits. Makes gates limit check. Unsets gx if succesful. */
        bool setGy (unsigned g0, unsigned g1, bool isGy = true);

        /** Returns true if bg flag is set.*/
        bool getBg() const;

        /** Sets bg flag, and gate limits. Makes gates limit check. Unsets sbg if succesful. */
        bool setBg (unsigned b0, unsigned b1, bool isBg = true);
        
        /** Returns true if sbg flag is set.*/
        bool getSBg() const;

        /** Sets sbg flag, and gate limits. Makes gates limit check. Unsets bg if succesful. */
        bool setSBg ( unsigned b0, unsigned b1,
                      unsigned b2, unsigned b3, 
                      bool isSBg = true);
        
        /** Returns true if bin flag is set.*/
        bool getBin() const;

        /** Sets binning size. If only b0 is passed, the b1 is equal to b0.*/
        bool setBin (unsigned bx, unsigned by = 0, bool isBin = true);

        /** Returns by reference vector containing binning size.*/
        void getBinning(std::vector<unsigned>& rtn) const;

        /** Returns by reference vector containing X gates.*/
        void getGateX(std::vector<unsigned>& rtn) const;

        /** Returns by reference vector containing Y gates.*/
        void getGateY(std::vector<unsigned>& rtn) const;

        /** Returns by reference vector containing background gates.*/
        void getBgGate(std::vector<unsigned>& rtn) const;

        /** Returns true if every flag is set.*/
        bool getEvery() const;

        /** Sets every size. If only nx is passed, the ny is equal to nx.*/
        bool setEvery (unsigned nx, unsigned ny = 0, bool isEvery = true);

        /** Returns by reference vector containing every vector.*/
        void getEveryN(std::vector<unsigned>& rtn) const;

        /** Sets polygon gate defined in polygonFile.
         * If isGx is set true, projection will be made on Y axis (gate on X)
         * if false projection will be on X axis (gate on Y)
         */
        void setPolygon (std::string polygonFile, bool isGx, bool isPg = true);

        /** Returns true if --pg is set. */
        bool getPg() const;
        
        /** Returns name of polygon definition file. */
        std::string getPolygon() const;

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

        /** --every flag. Every n-th point.*/ 
        bool isEvery_;

        /** For --gx. Stores gates. */
        std::vector<unsigned> gx_;

        /** For --gy. Stores gates. */
        std::vector<unsigned> gy_;

        /** For --bg or --sbg. Stores background gates */
        std::vector<unsigned> b_;

        /** For --bin. Stores binning size.*/
        std::vector<unsigned> bin_;

        /** For --every. Stores every size.*/
        std::vector<unsigned> every_;

        /** For --pol (polygon gate). */
        bool isPg_;

        /** Polygon definition file */
        std::string polygonFile_;
};

#endif
