#include <vector>
#include <iostream>
#include <cstdlib>
#include <sys/time.h>
#include <cmath>
#include "hisdrr.h"
#include "drrblock.h"
using namespace std;

void help() {
    cout << "SYNOPSIS:" << endl;
    cout << "  zerotest file id" << endl << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "  Program zeroes single histogram in .his file." << endl;
    cout << "  Both his and drr files are required to be present in a current folder " << endl;
    cout << "  and with the same base name (e.g. a.his and a.drr)." << endl;
    cout << "  Files can also be present in a single gzip tar archive." << endl;
    cout << "OPTIONS:" << endl;
    cout << "  file: name of input file with or without extension" << endl;
    cout << "        or archive (.tar.gz or .tgz). In the latter case" << endl;
    cout << "        program will unzip specified archive but will not" << endl;
    cout << "        delete it after finishing execution." << endl;
    cout << "  id:   id (integer) of histogram to be zeroed" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        help();
        exit(1);
    }

    string fileName(argv[1]);
    unsigned int dot = fileName.find_first_of(".");
    string baseName = fileName.substr(0,dot);
    string extension = fileName.substr(dot+1);

    try {
        if ((extension == "tar.gz")||(extension == "tgz")) {
            cout << "#Archive " << fileName << " found unzipping..." << endl;
            string cmd = "tar xzf " + fileName;
            system(cmd.c_str());
        }
        string drr = baseName + ".drr";
        string his = baseName + ".his";

        HisDrr h(drr, his);
        int id = atoi(argv[2]);
        timeval t_start;
        gettimeofday(&t_start,NULL);
        h.zeroHistogram(id);
      
        /*
         * This part is for testing of setValue only
        DrrHisRecordExtended info = h.getHistogramInfo(id);
        vector<unsigned short> put;
        if (info.hisDim == 1) {
            put.reserve(info.scaled[0]);
            for (int i = 0; i < info.scaled[0]; i++) 
                    put.push_back(100*exp(-pow((i-500),2)/(2*200*200)));
        }
        if (info.hisDim == 2) {
            put.reserve(info.scaled[0]*info.scaled[1]);
            for (int i = 0; i < info.scaled[0]; i++) 
                for (int j = 0; j < info.scaled[1]; j++) 
                    put.push_back(i*j);
        }
         
        h.setValue(id, put);
        */    
        timeval t_end;
        gettimeofday(&t_end,NULL);
        cout << "#T = " << (t_end.tv_sec - t_start.tv_sec)*1e6 + t_end.tv_usec - t_start.tv_usec << " us " << endl;

   } catch (IOError &err) {
        cout << "I/O Error: " << err.show() << endl;
   } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
   }

}
