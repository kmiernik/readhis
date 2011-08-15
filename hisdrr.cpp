/**
 * Implementation of Spectrum class
 */

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include "hisdrr.h"
#include "drrblock.h"
using namespace std;

HisDrr::HisDrr(string &drr, string &his) {
    /* test of size of int and short */
    if ( sizeof(unsigned short) != 2 || sizeof(unsigned int) != 4 ) {
        stringstream err;
        err << "This program is intended to run with 'unsigned short' size 2 bytes and"
            << " and 'unsigned int' size 4 bytes. Your machine uses " << sizeof(unsigned short)
            << " and " << sizeof(unsigned int) << " respectively." << endl;
        string msg = err.str();
        throw IOError(msg);
    }

    ifstream testIn(his.c_str());
    if (!testIn.good()) {
        stringstream err;
        err << "Could not open file " << his;
        string msg = err.str();
        throw IOError(msg);
    }
    testIn.close();

    testIn.open(drr.c_str());
    if (!testIn.good()) {
        stringstream err;
        err << "Could not open file " << drr;
        string msg = err.str();
        throw IOError(msg);
    }
    testIn.close();

    drrFile = drr;
    hisFile = his;

    loadDrr();
}

HisDrr::HisDrr(string &drr, string &his, string &input) {
    drrFile = drr;
    hisFile = his;

    /* test of size of int and short */
    if ( sizeof(unsigned short) != 2 || sizeof(unsigned int) != 4 ) {
        stringstream err;
        err << "This program is intended to run with 'unsigned short' size 2 bytes and"
            << " and 'unsigned int' size 4 bytes. Your machine uses " << sizeof(unsigned short)
            << " and " << sizeof(unsigned int) << " respectively." << endl;
        string msg = err.str();
        throw IOError(msg);
    }

    ifstream fileInput(input.c_str());
    if (!fileInput.good()) {
        stringstream err;
        err << "Could not open file " << input;
        string msg = err.str();
        fileInput.close();
        throw IOError(msg);
    }
    
    //This part fills in drrData vector using definition found in input text file
    vector <SimpleDrrBlock> drrData;
    if (fileInput.good()) {
        string line;
        while (getline(fileInput, line) ) {
            istringstream iss;
            iss.str(line);
            if (line[0] != '#') {
                string token;
                SimpleDrrBlock block;
                iss >> block.hisID >> block.halfWords >> block.scaled[0] >> block.scaled[1];
                if ((block.halfWords != 1)&&(block.halfWords != 2)) {
                    stringstream err;
                    err << "Only 2 or 4 bytes long histograms supported";
                    string msg = err.str();
                    fileInput.close();
                    throw GenError(msg);
                }
                while (iss) {
                    string titlePart;
                    iss >> titlePart;
                    block.title += titlePart + " ";
                }
                if (block.title.size() == 1)
                    block.title = "Empty title ";
                drrData.push_back(block);
                iss.clear();
            }
        }
        fileInput.close();
    }


    ofstream drrOut(drrFile.c_str(), ios::binary);
    if (!drrOut.good()) {
        stringstream err;
        err << "Could not create file " << drrFile;
        string msg = err.str();
        drrOut.close();
        throw IOError(msg);
    }
    ofstream hisOut(hisFile.c_str(), ios::binary);
    if (!hisOut.good()) {
        stringstream err;
        err << "Could not create file " << hisFile;
        string msg = err.str();
        hisOut.close();
        throw IOError(msg);
    }

    // Using information from drrData drr header is created
    DrrHeader head;
    int totLength;
    for (unsigned int i = 0; i < drrData.size(); i++) 
        totLength += (drrData[i].scaled[0]+drrData[i].scaled[1])*drrData[i].halfWords; 
    // Magic words (whatever they do...)
    string initial = "HHIRFDIR0001";
    for (unsigned int i = 0; i < initial.size(); i++) {
        head.initial[i] = initial[i];
    }
    head.nHis = drrData.size();
    head.nHWords = totLength;
    time_t clock = time(NULL);
    tm *date = localtime(&clock);
    head.date[0]=0; 
    head.date[1]=date->tm_year+1900; 
    head.date[2]=date->tm_mon; 
    head.date[3]=date->tm_mday; 
    head.date[4]=date->tm_hour; 
    head.date[5]=date->tm_min; 
    char description[40] = {0};
    for (int i = 0; i < 40; i++)
        head.description[i] = description[i];
    // Header is followed by 44 empty bytes to reach 128 bytes long block
    drrOut.write((char *)&head, sizeof(head));
    char garbage[44] = {0};
    drrOut.write(garbage, 44);

    DrrHisRecord record;

    // This part is creating and writing records for each histogram
    // At the same time filling in the his file with '0' values
    int offset = 0;
    for (unsigned int i = 0; i < drrData.size(); i++) {
        int dim = 0;
        for (int j = 0; j < 2; j++)
            if (drrData[i].scaled[j] > 0)
                dim++;
        record.hisDim = dim;
        record.halfWords = drrData[i].halfWords;

        for (int j = 0; j < 4; j++)
            record.params[j] = 0;

        record.raw[0] = drrData[i].scaled[0];
        record.raw[1] = drrData[i].scaled[1];
        for (int j = 2; j < 4; j++)
            record.raw[j] = 0;

        record.scaled[0] = drrData[i].scaled[0];
        record.scaled[1] = drrData[i].scaled[1];
        for (int j = 2; j < 4; j++)
            record.scaled[j] = 0;

        for (int j = 0; j < 4; j++)
            record.minc[j] = 0;

        record.maxc[0] = drrData[i].scaled[0]-1;
        if (drrData[i].scaled[1] > 0)
            record.maxc[1] = drrData[i].scaled[1]-1;
        else
            record.maxc[1] = 0;
        for (int j = 2; j < 4; j++)
            record.maxc[j] = 0;
        
        record.offset = offset;
        char label[12] = {0};
        for (int j = 0; j < 12; j++) {
            record.xlabel[j] = label[j];
            record.ylabel[j] = label[j];
        }
        for (int j = 0; j < 4; j++)
            record.calcon[j] = 0;
        unsigned int k = 0; 
        while ( (k < 39)&&(k < drrData[i].title.size() - 1) ){
            record.title[k] = drrData[i].title[k];
            k++;
        }
        //It is very important to fill unused part of title with 0
        //As well as remove end of string character from the last position
        for (; k < 40; k++)
            record.title[k] = 0; 

        drrOut.write((char *)&record, sizeof(record));
    
        unsigned size = 0;
        if (dim == 1)
            size = drrData[i].scaled[0];
        if (dim == 2)
            size = drrData[i].scaled[0]*drrData[i].scaled[1];

        if (drrData[i].halfWords == 1) {
            unsigned short *his = new unsigned short[size]();
            hisOut.write((char *)his, size*sizeof(unsigned short));
            offset += size*drrData[i].halfWords;
            delete []his;
        }
        if (drrData[i].halfWords == 2) {
            unsigned int *his = new unsigned int[size]();
            hisOut.write((char *)his, size*sizeof(unsigned int));
            offset += size*drrData[i].halfWords;
            delete []his;
        }

    }

    //At the end of file we put a list of histograms in 128 bytes long blocks
    //They are build of 32 records of 4 bytes (int) long histogram Id's
    for (unsigned int i = 0; i < drrData.size()/32 + 1; i++) {
        int hisList[32] = {0};
        unsigned int j = 0;
        while ((j < drrData.size())&&(j < 32)) {
            hisList[j%32] = drrData[i*32+j].hisID;
            j++;
        }
        drrOut.write((char *)hisList, 128);
    }
    drrOut.close();
    hisOut.close();
    
    cout << "# Created files " << drr << " and " << his << " with following histograms: " << endl;
    for (unsigned int i = 0; i < drrData.size(); i++) {
        cout << "# " << drrData[i].hisID << " " << drrData[i].halfWords << " " << drrData[i].scaled[0] << " " << drrData[i].scaled[1] << " \"" << drrData[i].title << "\"" << endl; 
    }

    // And now constructors loads freshly created files
    // so it follows a logic scheme of constructor(drr, his)
    loadDrr();
}

void HisDrr::readBlock(ifstream& file, drrBlock *block) {
    if (file.good())
        file.read((char*)block, sizeof(*block));
    else {
        stringstream err;
        err << "Could not read data from " << file;
        string msg = err.str();
        throw IOError(msg);
    }
}

void HisDrr::loadDrr() {
    ifstream drrIn(drrFile.c_str());

    // Block of data 128 lenght stored in union
    drrBlock block;

    if (drrIn.good()) 
        readBlock(drrIn, &block);

    // Header contains number of histograms in a file
    int nHis = block.header.nHis;

    // We read as many blocks of data as the number of 
    // histogram
    //
    // Inside a loop there is a jump made.
    // We have to jump inside the drr file to get to
    // the histogram IDs which are at the end of file.
    // Jump lenght is equal to:
    // number of histograms plus one (for header) plus
    // current histogram index (position in file, not id)
    
    int hId;
    DrrHisRecordExtended drrRecExt;
    hisList.reserve(nHis);
    for (int i = 0; i < nHis; i++) {
        if (drrIn.good()) {
            readBlock(drrIn, &block);
            int currentPos = drrIn.tellg();
            //jump for ID
            drrIn.seekg( (nHis+1)*sizeof(block)+i*sizeof(hId) );
            //check if we are still in the file
            if (drrIn.good())
                drrIn.read((char*)&hId, sizeof(hId));
            else {
                stringstream err;
                err << "Error reading " << drrIn << " at " << drrIn.tellg();
                string msg = err.str();
                throw IOError(msg);
            }
            //...and jump back
            drrIn.seekg(currentPos);

            drrRecExt.hisID = hId;
            drrRecExt = block.record;
            hisList.push_back(drrRecExt); 
        }
        else {
            stringstream err;
            err << "Error reading " << drrIn << " at " << drrIn.tellg();
            string msg = err.str();
            throw IOError(msg);
        }
    }
}

// Return by value version
//vector<unsigned int> HisDrr::getHistogram(int id) {
// Return by reference version
void HisDrr::getHistogram(vector<unsigned int> &rtn, int id) {
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    vector<unsigned int> r;
    ifstream his(hisFile.c_str());
    if (his.good()) {
        // We jump to location specified by offset (given in units of 2 bytes)
        his.seekg(hisList[index].offset*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
            
        // Check if data exceedes size of unsigned int 
        if ((unsigned short)(hisList[index].halfWords*2) > sizeof(unsigned int) ) {
            stringstream err;
            err << "Histograms with channel size " << hisList[index].halfWords*2
                << " bytes long are not supported ";
            string msg = err.str();
            throw GenError(msg);
        }

        // Since we know now the length of data, we can reserve space
        // for vector avoiding unecessary data movement in memory
        r.reserve(length);
        // We read histogram channels and push them to vector
        // Each channel has a length of equal to halfWords parameter
        // which is given in 2 bytes units
        // We cast this value on unsigned int, which is zeroed first
        // It is a safe cast then.
        for (unsigned int i = 0; i < length; i++) {
            unsigned int u = 0;
            his.read((char*)&u, hisList[index].halfWords*2);
            r.push_back(u);
        }
    }
    // Return by value version
    //return r;
    
    // Return by reference version
    rtn.swap(r);
}

DrrHisRecordExtended HisDrr::getHistogramInfo(int id) {
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    return hisList[index];    
}

void HisDrr::getHisList(vector<int> &r) {
    r.reserve(hisList.size());
    for (unsigned int i = 0; i < hisList.size(); i++)
        r.push_back(hisList[i].hisID);
}

void HisDrr::zeroHistogram(int id) {
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    fstream his(hisFile.c_str());
    if (his.good()) {
        // We jump to location specified by offset (given in units of 2 bytes)
        his.seekp(hisList[index].offset*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
            
        // We put 0 for all data in histogram.
        unsigned int size = hisList[index].halfWords*2*length;
        //Initialization of array to 0
        char *zeroarray = new char[size]();
        cout << "# 0 put from " << his.tellp();
        his.write(zeroarray,size);
        cout << " to " << his.tellp() << ", size = " << size << " bytes" << endl;
        delete []zeroarray;
        his.close();
    }
}

void HisDrr::setValue(const int id, unsigned int pos, unsigned int value){
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    fstream his(hisFile.c_str());
    if (his.good()) {
        if (hisList[index].halfWords*2 != sizeof(value)) {
            stringstream err;
            err << "Channel size " << hisList[index].halfWords*2 << " bytes, mismatches value to put size" << sizeof(value);
            string msg = err.str();
            throw GenError(msg);
        }

        // We jump to location specified by offset (given in units of 2 bytes) plus i
        his.seekp(hisList[index].offset*2 + pos*hisList[index].halfWords*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
        if (pos > length) {
            stringstream err;
            err << "X channel " << pos << " exceedes size of histogram id " << id ;
            string msg = err.str();
            throw GenError(msg);
        }
        // Write value 
        his.write((char *)&value, sizeof(value));
        his.close();
    }

}

void HisDrr::setValue(const int id, unsigned int pos, unsigned short value){
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    fstream his(hisFile.c_str());
    if (his.good()) {
        if (hisList[index].halfWords*2 != sizeof(value)) {
            stringstream err;
            err << "Channel size " << hisList[index].halfWords*2 << " bytes, mismatches value to put size" << sizeof(value);
            string msg = err.str();
            throw GenError(msg);
        }

        // We jump to location specified by offset (given in units of 2 bytes) plus i
        his.seekp(hisList[index].offset*2 + pos*hisList[index].halfWords*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
        if (pos > length) {
            stringstream err;
            err << "X channel " << pos << " exceedes size of histogram id " << id ;
            string msg = err.str();
            throw GenError(msg);
        }
        // Write value 
        his.write((char *)&value, sizeof(value));
        his.close();
    }
}

void HisDrr::setValue(const int id, vector<unsigned int> &value){
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    fstream his(hisFile.c_str());
    if (his.good()) {
        if (hisList[index].halfWords*2 != sizeof(unsigned int)) {
            stringstream err;
            err << "Channel size " << hisList[index].halfWords*2 << " bytes, mismatches requested to put variables of size " << sizeof(unsigned int) << " bytes ";
            string msg = err.str();
            throw GenError(msg);
        }
        // We jump to location specified by offset (given in units of 2 bytes) plus pos
        his.seekp(hisList[index].offset*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
        cout << "length: " << length << " put size: " << value.size() << endl;
        if (value.size() != length) {
            stringstream err;
            err << "Vector length " << value.size() <<" is different than histogram id " << id << " size " << length << endl ;
            string msg = err.str();
            throw GenError(msg);
        }
        // Write value 
        // First cast from vector to array
        unsigned int *newvalue = new unsigned int[length];
        for (unsigned int i = 0; i < length; i++) {
            newvalue[i] = value[i];
        }

        // Now put array into the file
        unsigned int size = hisList[index].halfWords*2*length;
        his.write((char *)newvalue, size);

        delete []newvalue;
        his.close();
    }

}

void HisDrr::setValue(const int id, vector<unsigned short> &value){
    // First we search if histogram id exists
    int index = -1;
    for (unsigned int i = 0; i < hisList.size(); i++)
        if (hisList[i].hisID == id) {
            index = i;
            break;
        }

    if (index < 0) {
        stringstream err;
        err << "Could not find spectrum id = " << id << " in file " << drrFile;
        string msg = err.str();
        throw GenError(msg);
    }
    
    fstream his(hisFile.c_str());
    if (his.good()) {
        if (hisList[index].halfWords*2 != sizeof(unsigned short)) {
            stringstream err;
            err << "Channel size " << hisList[index].halfWords*2 << " bytes, mismatches requested to put variables of size " << sizeof(unsigned short) << " bytes ";
            string msg = err.str();
            throw GenError(msg);
        }
        // We jump to location specified by offset (given in units of 2 bytes) plus pos
        his.seekp(hisList[index].offset*2);
        // Lenght of data is equal to product of all histogram dimensions lengths
        unsigned int length = 1;
        for (int i = 0; i < hisList[index].hisDim; i++)
            length = length * hisList[index].scaled[i];
        if (value.size() != length) {
            stringstream err;
            err << "Vector length " << value.size() <<" is different than histogram id " << id << " size " << length << endl ;
            string msg = err.str();
            throw GenError(msg);
        }
        // Write value 
        // First cast from vector to array
        unsigned short *newvalue = new unsigned short[length];
        for (unsigned int i = 0; i < length; i++) {
            newvalue[i] = value[i];
        }

        // Now put array into the file
        unsigned int size = hisList[index].halfWords*2*length;
        his.write((char *)newvalue, size);
        delete []newvalue;
        his.close();
    }

}

