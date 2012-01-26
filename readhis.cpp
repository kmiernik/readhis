#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include "HisDrr.h"
#include "HisDrrHisto.h"
#include "Exceptions.h"

using namespace std;
    
static struct option long_options[] = {
    {"id",    required_argument, 0, 'i'},
    {"gx",    required_argument, 0, 'x'},
    {"gy",    required_argument, 0, 'y'},
    {"bg",    required_argument, 0, 'b'},
    {"sbg",   required_argument, 0, 's'},
    {"zero",  no_argument, 0,       'z'},
    {"info",  no_argument, 0,       'I'},
    {"list",  no_argument, 0,       'l'},
    {"List",  no_argument, 0,       'L'},
    {"help",  no_argument, 0,       'h'},
    {0, 0, 0, 0}
};

enum Status {
    ok,
    warning,
    error
};

Status parseMultiArgs (char* arguments, int* argsArray, unsigned requiredArgs = 0) {
    char* tokens;
    tokens = strtok(arguments, ",");

    unsigned numberOfTokens = 0;
    while (tokens != 0) {
        if (numberOfTokens < requiredArgs)
            argsArray[numberOfTokens] = atoi(tokens);
        else
            return warning;
        tokens = strtok(0, ",");
        numberOfTokens++;
    }    
    if (numberOfTokens == requiredArgs)
        return ok;
    else
        return error;
}

void help() {
    cout << "USAGE:" << endl;
    cout << "  readhis [options] file.his " << endl;
    cout << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "  Program reads his file using binary description from drr file." << endl;
    cout << "  Both files are required to be present in the specified path " << endl;
    cout << "  and with the same base name (e.g. t1.his and t1.drr)." << endl;
    cout << "  Result is send to standard output, use redirection " << endl;
    cout << "  in case you want to save it to file." << endl;
    cout << endl;
    cout << "OPTIONS:" << endl;
    cout << "  --id : short (-i), selects histogram id " << endl;
    cout << "         required by other options unless stated otherwise. " << endl;
    cout << endl;
    cout << "  --gx x0,x1 : short (-x), 2D histograms only, requires two " << endl;
    cout << "               integer arguments separated by coma. Sets " << endl;
    cout << "               projection on X axis, starting from column x0 to x1" << endl;
    cout << "               (including both)" << endl;
    cout << endl;
    cout << "  --gy x0,x1 : short (-y), as above, projection on Y axis " << endl;
    cout << endl;
    cout << "  --bg x0,x1 : short (-b), 2D histograms only, only with gx or " << endl;
    cout << "               gy option. Sets gate for background subtraction, " << endl;
    cout << "               from x0 to x1 (including both). " << endl;
    cout << endl;
    cout << "  --sbg x0,x1,x2,x3 : short (-s), 2D histograms only, only with " << endl;
    cout << "                      gx or gy option. Same as above exept that  " << endl;
    cout << "                      gate is split into two parts x0 to x1 and "<< endl;
    cout << "                      x2 to x3." << endl;
    cout << endl;
    cout << "  --zero : short (-z), suppresses bins with zero counts in output" << endl;
    cout << endl;
    cout << "  --info : short (-I), displays detailed information on histogram" << endl;
    cout << endl;
    cout << "  --list : short (-l), does not requires id. Displays list of" << endl;
    cout << "           histograms present in the file. " << endl;
    cout << endl;
    cout << "  --List : short (-L), does not requires id. As above," << endl; 
    cout << "            additionally marks empty histograms and histogram dimensions. " << endl;
    cout << endl;

    cout << "  --help  : displays this help " << endl;
    cout << endl;
}
     
int main (int argc, char* argv[]) {

    int flag = 0;

    Options* options = new Options();

    while (true) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        flag = getopt_long (argc, argv, "i:x:y:b:s:zIlLh",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (flag == -1)
            break;

        switch (flag) {

            case 'i': {
                int hisId = atoi(optarg);
                if ( !options->setHisId(hisId) ) {
                    cout << "Wrong or missing histogram id " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Histogram: " << options->getHisId() << endl;
                break;
            }

            case 'x': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --gx with more then two arguments " << endl;
                }
                if (status == error) {
                    cout << "Error: option --gx requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setGx(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --gx option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Gate on X from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 'y': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --gy with more then two arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --gy requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setGy(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --gy option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Gate on Y from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 'b': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                if (status == warning) {
                    cout << "Warning: option --bg with more then two arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --bg requires two "  
                         << "arguments separated by coma e.g 10,20 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setBg(true, a[0], a[1])) ) {
                    cout << "Error: wrong arguments for --bg option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Background taken from " << a[0] << " to " << a[1] << endl;
                break;
            }

            case 's': {
                int a[4] = {0};
                Status status = parseMultiArgs(optarg, a, 4);
                if (status == warning) {
                    cout << "Warning: option --sbg with more then four arguments" << endl;
                }
                if (status == error) {
                    cout << "Error: option --sbg requires four "  
                         << "arguments separated by coma e.g 10,20,30,40 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setSBg(true, a[0], a[1], a[2], a[3])) ) {
                    cout << "Error: wrong arguments for --sbg option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Background taken from " << a[0] << " to " << a[1] 
                        << " and " << a[2] << " to " << a[3] << endl;
                break;
            }

            case 'z': {
                options->setZeroSup(true);
                break;
            }

            case 'I': {
                options->setInfoMode(true);
                break;
            }

            case 'l': {
                options->setListMode(true);
                break;
            }

            case 'L':
                options->setListModeZ(true);
                break;

            case 'h':
                help();
                exit(0);
                break;
                
            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                help();
                exit(1);
            }
        }

    string fileName;
    if (optind < argc) {
        fileName = argv[optind];
    } else {
        cout << "Error: missing histogram file name" << endl;
        cout << "Run readhis --help for more information" << endl;
        exit(1);
    }
    
    unsigned int dot = fileName.find_last_of(".");
    string baseName = fileName.substr(0,dot);
    const string drr = baseName + ".drr";
    const string his = baseName + ".his";

    try {
        HisDrrHisto h(drr, his, options);
        h.process();
    } catch (GenError &err) {
        cout << "Error: " << err.show() << endl;
    }
  
    delete options;
    exit(0);
}
