/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

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
    {"bin",   required_argument, 0, 'B'},
    {"every", required_argument, 0, 'e'},
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

Status parseMultiArgs (char* arguments, int* argsArray,
                       unsigned requiredArgs = 0, unsigned optionalArgs = 0) {
    char* tokens;
    tokens = strtok(arguments, ",");

    unsigned numberOfTokens = 0;
    while (tokens != 0) {
        if (numberOfTokens < requiredArgs + optionalArgs)
            argsArray[numberOfTokens] = atoi(tokens);
        else
            return warning;
        tokens = strtok(0, ",");
        numberOfTokens++;
    }    
    if (numberOfTokens >= requiredArgs)
        return ok;
    else
        return error;
}

/** Displays and formats help output.*/
void helpItem(const string& title, const string& abbrev, const string& desc){
    cout << title << endl;
    if (abbrev != "")
        cout << "\tShort: " << abbrev << endl;
    cout << "\tDescription: " << endl; 
    stringstream ss(desc);
    string token;
    unsigned linel = 0;
    cout << "\t\t";
    while (ss >> token) {
        if (token == "<BR>") {
            cout << endl << "\t\t";
            linel = 0;
            continue;
        }
        linel += token.size() + 1;
        if (linel > 60) {
            cout << endl << "\t\t" << token << " ";
            linel = token.size() + 1;
        } else {
            cout << token << " ";
        }
    }
    cout << endl << endl;
}

void help() {
    cout << "USAGE:" << endl;
    cout << "\treadhis [options] file.his " << endl;
    cout << endl;
    cout << "DESCRIPTION:" << endl;
    cout << "\tProgram reads his file using binary description from drr file." << endl;
    cout << "\tBoth files are required to be present in the specified path " << endl;
    cout << "\tand with the same base name (e.g. t1.his and t1.drr)." << endl;
    cout << "\tResult is send to standard output, use redirection " << endl;
    cout << "\tin case you want to save it to file." << endl;
    cout << endl;

    cout << "OPTIONS:" << endl;
    helpItem("\tOption:\t--id", "-i", "Selects histogram id, required by all other\
 options unless stated otherwise." );

    helpItem("\tOption:\t--gx AND (x0,x1 OR filename OR filename,id)",
             "-x",
             "For 2D histograms only, requires two integer arguments\
 separated by coma. Sets projection on Y axis (gate on X). starting\
 from column x0 to x1 (including both). If filename is given, the file\
 must be a text file defining polygon, using the following\
 format: <BR> <BR> #Comment line <BR> x0 y0 <BR> x1 y1 <BR> (...) <BR> <BR>\
 at least 3 points are required. <BR> If filename,id syntax is used,\
 the file must be a BAN (damm) file, and id is a damm banana id to be used.\
 For the purpose of polygon gates, the edges of polygon (including vertices)\
 are assumed to be outside of the polygon.\
 ");

    helpItem("\tOption:\t--gy AND (y0,y1 OR filename OR filename,id)",
             "-y",
             "As above, exept that projection is made on X axis\
 (gate on Y).\
 If both gx and gy are used the output would be 2D crop of histogram instead of\
 projection.\
 ");

    helpItem("\tOption:\t--bg AND x0,x1",
             "-b",
             "For 2D histograms only, only with --gx or --gy.\
 requires two integer arguments separated by coma. Sets gate for background\
 subtraction from channel containing x0 to x1 (including both).\
 ");

    helpItem("\tOption:\t--sbg AND x0,x1,x2,x3",
             "-b",
             "For 2D histograms only, only with --gx or --gy.\
 Same as above exept that gate is split into two parts x0 to x1 and\
 x2 to x3.\
 ");

    helpItem("\tOption:\t--bin AND (bx OR bx,by)",
             "-B",
             "Defines number of histogram bins to join. For 2D histogram\
 if only one argument is given by = bx is assumed. At least one bin size\
 must be > 1.\
 ");

    helpItem("\tOption:\t--every AND n OR nx,ny",
             "-e",
             "Output gives every n-th (or nx-th in X direction and ny-th in Y)\
 point. At least one of ns should be larger then 1.\
 ");

    helpItem("\tOption:\t--zero",
             "-z",
             "Suppresses bins with zero counts in output. Notice that output\
              format for 2D Histograms changes with -z on and off. Output\
              with -z is no longer suitable for gnuplot pm3d map plotting.\
 ");

    helpItem("\tOption:\t--info",
             "-I",
             "Displays detailed information on histogram.\
 ");

    helpItem("\tOption:\t--list",
             "-l",
             "Does not require histogram id. Displays list of all histograms\
             present in a given file.\
 ");

    helpItem("\tOption:\t--List",
             "-L",
             "Does not require histogram id. As above except that displays\
             number of dimensions for each histogram and marks empty\
             histograms with 'E' letter next to id.\
 ");

    helpItem("\tOption:\t--help",
             "-h",
             "Shows this help.\
 ");
}
     
int main (int argc, char* argv[]) {

    int flag = 0;

    Options* options = new Options();

    while (true) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        flag = getopt_long (argc, argv, "i:x:y:b:s:B:e:zIlLh",
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
                string arg(optarg);
                int coma = arg.find_last_of(",");
                if (coma != (int)string::npos ) {
                    arg = arg.substr(0, coma);
                } 

                ifstream test(arg.c_str());
                if (test) {
                    cout << "# Polygon file: " << optarg << endl;
                    string polfile = string(optarg);
                    options->setPolygon(polfile, true);
                } else {
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
                    if ( !(options->setGx(a[0], a[1])) ) {
                        cout << "Error: wrong arguments for --gx option" << endl;
                        cout << "Run readhis --help for more information" << endl;
                        exit(1);
                    }
                    cout << "# Gate on X from " << a[0] << " to " << a[1] << endl;
                }
                test.close();
                break;
            }

            case 'y': {
                string arg(optarg);
                int coma = arg.find_last_of(",");
                if (coma != (int)string::npos ) {
                    arg = arg.substr(0, coma);
                } 

                ifstream test(arg.c_str());
                if (test) {
                    cout << "# Polygon file: " << optarg << endl;
                    string polfile = string(optarg);
                    options->setPolygon(polfile, false);
                } else {
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
                    if ( !(options->setGy(a[0], a[1])) ) {
                        cout << "Error: wrong arguments for --gy option" << endl;
                        cout << "Run readhis --help for more information" << endl;
                        exit(1);
                    }
                    cout << "# Gate on Y from " << a[0] << " to " << a[1] << endl;
                }
                test.close();
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
                    if ( !(options->setBg(a[0], a[1])) ) {
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
                if ( !(options->setSBg(a[0], a[1], a[2], a[3])) ) {
                    cout << "Error: wrong arguments for --sbg option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Background taken from " << a[0] << " to " << a[1] 
                        << " and " << a[2] << " to " << a[3] << endl;
                break;
            }

            case 'B': {
                int b[2] = {0};
                Status status = parseMultiArgs(optarg, b, 1, 1);
                if (status == warning) {
                    cout << "Warning: option --bin with more then two arguments " << endl;
                }
                if (status == error) {
                    cout << "Error: option --bin requires one or two "  
                         << "arguments separated by coma e.g 2,4 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                if ( !(options->setBin(b[0], b[1])) ) {
                    cout << "Error: wrong arguments for --bin option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Binning, X: " << b[0] << ", Y: " << b[1] << endl;
                break;
            }

            case 'e': {
                int e[2] = {0};
                Status status = parseMultiArgs(optarg, e, 1, 1);
                if (status == warning) {
                    cout << "Warning: option --every with more then two arguments " << endl;
                }
                if (status == error) {
                    cout << "Error: option --every requires one or two "  
                         << "arguments separated by coma e.g 2,4 " << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "#-> " << e[0] << " " << e[1] << endl;
                if ( !(options->setEvery(e[0], e[1])) ) {
                    cout << "Error: wrong arguments for --every option" << endl;
                    cout << "Run readhis --help for more information" << endl;
                    exit(1);
                }
                cout << "# Every, X: " << e[0] << ", Y: " << e[1] << endl;
                break;
            }

            case 'p': {
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
        cout << "# File: " << fileName << endl;
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
