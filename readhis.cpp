/* Version 08/16/2011 */
#include "readHisClass.h"

using namespace std;

void help() {
    cout << "SYNOPSIS:" << endl;
    cout << "  readhis file [id] [-gx|-gy g0 g1 [-bg b0 b1] [-sbg b0 b1 b2 b3] [-b n]|[-B n1 n2] [-i] [-l] [-z]" << endl << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "  Program reads .his file using binary description from drr file." << endl;
    cout << "  Both files are required to be present in a current folder " << endl;
    cout << "  and with the same base name (e.g. a.his and a.drr)." << endl;
    cout << "  Result is send to standard output, use redirection " << endl;
    cout << "  in case you want to save it to file." << endl;
    cout << "OPTIONS:" << endl;
    cout << "  file: name of input file with extension (his or drr)" << endl;
    cout << "  id:   id (integer) of histogram to be extracted" << endl;
    cout << "  OPTIONAL: " << endl;
    cout << "  -gy:  flag for 2D histogram " << endl;
    cout << "        makes projection on X axis gated at " << endl;
    cout << "        Y channel g0 (int) to channel g1 (int) " << endl;
    cout << "  -gx:  as -gy except that projection is on Y axis" << endl;
    cout << "  -bg:  sets gate for background (starting at channel b0 ending b1)" << endl; 
    cout << "        subtraction. Works only together with -gx or -gy option" << endl;
    cout << "  -sbg: sets split gates for background subtraction " << endl; 
    cout << "        part I from b0 to b1, part II from b2 to b3 " << endl;
    cout << "  -b:   sets binning mode where 'n' is a number of channels per bin" << endl;
    cout << "  -B:   sets 2D binning mode where 'n1' is a number of X channels per bin" << endl;
    cout << "        and 'n2' is a number of Y channels per bin" << endl;
    cout << "        (works only for 2D spectrum)" << endl;
    cout << "  -i:   displays detailed information about histogram" << endl;
    cout << "  -l:   displays list of histograms id present in a file" << endl;
    cout << "  -L:   displays list of histograms id present in a file and marks empty histograms" << endl;
    cout << "  -z:   removes bins with 0 counts in output (except of 3D histograms)" << endl;
}

/*
 * Main program for extracting histograms out of his and drr files
 */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        help();
        exit(1);
    }
    vector<string> flags;
    for (int i = 1; i < argc; i++)
        flags.push_back(argv[i]);
    try {
        ReadHis *histo = new ReadHis(flags);
        histo->process();
        
        int sz = histo->cSpectrum.size();
        if (histo->hisDim == 1) {
            cout << "#1D spectrum " << histo->options.hisID  << endl;
            cout << "#Ch Counts Err" << endl;
            cout << "#Bin size: " << histo->options.bin << endl;
            for (int x = 0; x < sz; x++)
                if (!(histo->options.zero && histo->cSpectrum[x] == 0))
                    cout << x * histo->options.bin + 0.5 * ((histo->options.bin) - 1) 
                         << " " << histo->cSpectrum[x] << " " << histo->cError[x] << endl;
        }
        else if (histo->hisDim == 2) {
            cout << "#2D spectrum " << histo->options.hisID  << endl;
            if (histo->options.gy || histo->options.gx) {
                if (histo->options.gx) {
                    cout << "#Projection on Y axis, gate on X " << histo->options.g0 << " to " << histo->options.g1 << endl;
                    if (histo->options.bg)
                        cout << "#Background substracted, gate on X " << histo->options.b0 << " to " << histo->options.b1 << endl;
                    if (histo->options.sbg)
                        cout << "#Background substracted, gate on X " 
                             << histo->options.b0 << " to " << histo->options.b1 << " and "
                             << histo->options.b2 << " to " << histo->options.b3 << endl;
                }
                if (histo->options.gy) {
                    cout << "#Projection on X axis, gate on Y " << histo->options.g0 << " to " << histo->options.g1 << endl;
                    if (histo->options.bg)
                        cout << "#Background substracted, gate on Y " << histo->options.b0 << " to " << histo->options.b1 << endl;
                    if (histo->options.sbg)
                        cout << "#Background substracted, gate on Y " 
                             << histo->options.b0 << " to " << histo->options.b1 << " and "
                             << histo->options.b2 << " to " << histo->options.b3 << endl;
                }

                cout << "#Channel Counts Err" << endl;
                for (int x = 0; x < sz; x++)
                    if (!(histo->options.zero && histo->cSpectrum[x] == 0))
                        cout << x * histo->options.bin + 0.5 * ((histo->options.bin) - 1) 
                            << " " << histo->cSpectrum[x] << " " << histo->cError[x] << endl;

            } else {
                    cout << "#XCh YCh Counts" << endl;
                    int ySize = histo->sizeY/histo->options.binY;
                    int xSize = histo->sizeX/histo->options.binX;
                    for (int y = 0; y < ySize ; y++) {
                        for (int x = 0; x < xSize; x++) 
                                cout << x * histo->options.binX + 0.5 * (histo->options.binX - 1) << " "  // x position
                                    << y * histo->options.binY + 0.5 * (histo->options.binY - 1) << " "   // y position
                                    << histo->cSpectrum[x+(histo->sizeX/histo->options.binX)*y] << endl; // position in vector of (x,y) cell
                        cout << endl;
                    }
            }
        }
        delete histo;
    } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
    }
}
