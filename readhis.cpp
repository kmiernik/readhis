#include <cstdlib>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;
    
static struct option long_options[] = {
    {"id",    required_argument, 0, 'i'},
    {"gx",    required_argument, 0, 'x'},
    {"gy",    required_argument, 0, 'y'},
    {"bg",    required_argument, 0, 'b'},
    {"sbg",   required_argument, 0, 's'},
    {"info",  no_argument, 0,       'I'},
    {"list",  no_argument, 0,       'l'},
    {"listZ", no_argument, 0,       'L'},
    {"zero",  required_argument, 0, 'z'},
    {0, 0, 0, 0}
};

class Options {
    public:
        unsigned getHisId () { return hisId_; }
        bool setHisId (unsigned const hisId) {
            if (hisId > 0 && hisId < 10000) {
                hisId_ = hisId;
                isIdSet_ = true;
                return true;
            } else {
                isIdSet_ = false;
                return false;
            }
        }
        bool getisIdSet() { return isIdSet_; }

        bool getListMode() { return isListMode_; }
        void setListMode (const bool b) { 
            if (b) {
                isListModeZ_ = false;
                isListMode_ = true;
            } else {
                isListMode_ = false;
            }
            
        }

        bool getListModeZ() { return isListModeZ_; }
        void setListModeZ (const bool b) { 
            if (b) {
                isListMode_ = false;
                isListModeZ_ = true;
            } else {
                isListModeZ_ = false;
            }
        }

        bool getZeroSup() { return isZeroSup_; }
        void setZeroSup (const bool b) { isZeroSup_ = b; }
        
        bool getGx() { return isGx_; }
        bool setGx (const bool isGx, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGx_ = isGx;
            if (isGx) {
                isGy_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            return true;
        }

        bool getGy() { return isGy_; }
        bool setGy (const bool isGy, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGy_ = isGy;
            if (isGy) {
                isGx_ = false;
                g0_ = g0;
                g1_ = g1;
            }
            g0_ = g0;
            g1_ = g1;
            return true;
        }

        bool getBg() { return isBg_; }
        bool setBg (const bool isBg, const unsigned b0, const unsigned b1) {
            if (b0 > b1)
                return false;
            isBg_ = isBg;
            if (isBg) {
                isSBg_ = false;
                b0_ = b0;
                b1_ = b1;
            }
            return true;
        }
        
        bool getSBg() { return isSBg_; }
        bool setSBg (const bool isSBg, const unsigned b0, const unsigned b1,
                                       const unsigned b2, const unsigned b3) {
            if ( !(b3 > b2 && b2 > b1 && b1 > b0) )
                return false;
            isSBg_ = isSBg;
            if (isSBg) {
                isBg_ = false;
                b0_ = b0;
                b1_ = b1;
                b2_ = b2;
                b3_ = b3;
            }
            return true;
        }

        Options() {
            hisId_ = 0;
            isIdSet_ = false;
            isListMode_ = false;
            isListModeZ_ = false;
            isZeroSup_ = false;
            isGx_ = false;
            isGy_ = false;
            isBg_ = false;
            isSBg_ = false;
            g0_ = 0;
            g1_ = 0;
            b0_ = 0;
            b1_ = 0;
            b2_ = 0;
            b3_ = 0;
        }
    private:
        unsigned hisId_;
        bool isIdSet_;
        bool isListMode_;
        bool isListModeZ_;
        bool isZeroSup_;
        bool isGx_;
        bool isGy_;
        bool isBg_;
        bool isSBg_;
        unsigned g0_;
        unsigned g1_;
        unsigned b0_;
        unsigned b1_;
        unsigned b2_;
        unsigned b3_;
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
     
int main (int argc, char* argv[]) {

    int c = 0;

    Options* options = new Options();

    while (true) {
        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "i:x:y:b:s:zIlL",
                        long_options, &option_index);

        /* Detect the end of the options. */
        if (c == -1)
            break;

        switch (c) {

            case 'I': {
                cout << "Option -I" << endl;
                break;
            }

            case 'i': {
                cout << "Option -i " << endl;
                int hisId = atoi(optarg);
                if ( !options->setHisId(hisId) ) {
                    cout << "Wrong or missing hisId " << endl
                    abort();
                }
                cout << "Id: " << options->getHisId() << endl;
                break;
            }

            case 'l': {
                cout << "Option -l " << endl;
                options->setListMode(true);
                break;
            }

            case 'L':
                cout << "Option -L " << endl;
                options->setListModeZ(true);
                break;

            case 'x': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                switch (status){
                    case ok:
                        cout << "Option -x with values " << a[0] << " and " << a[1] << endl;
                        if ( !(options->setGx(true, a[0], a[1])) )
                            status = error;
                        break;
                    case warning:
                        cout << "Warning: option -x with values " << a[0] << " and " << a[1] << endl;
                        if ( !(options->setGx(true, a[0], a[1])) )
                            status = error;
                        break;
                    case error:
                        cout << "Error: option -x with values " << a[0] << " and " << a[1] << endl;
                }
                break;
            }

            case 'y': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                switch (status){
                    case ok:
                        cout << "Option -y with values " << a[0] << " and " << a[1] << endl;
                        if ( !(options->setGy(true, a[0], a[1])) )
                            status = error;
                        break;
                    case warning:
                        cout << "Warning: option -y with values " << optarg << endl;
                        if ( !(options->setGy(true, a[0], a[1])) )
                            status = error;
                        break;
                    case error:
                        cout << "Error: option -y with values " << optarg << endl;
                }
                break;
            }

            case 'b': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 2);
                switch (status){
                    case ok:
                        cout << "Option -b with values " << a[0] << " and " << a[1] << endl;
                        if ( !(options->setBg(true, a[0], a[1])) )
                            status = error;
                        break;
                    case warning:
                        cout << "Warning: option -b with values " << optarg << endl;
                        if ( !(options->setBg(true, a[0], a[1])) )
                            status = error;
                        break;
                    case error:
                        cout << "Error: option -b with values " << optarg << endl;
                }
                break;
            }

            case 's': {
                int a[2] = {0};
                Status status = parseMultiArgs(optarg, a, 4);
                switch (status){
                    case ok:
                        cout << "Option -s with values " << a[0] << " and " << a[1] << endl;
                        if ( !(options->setSBg(true, a[0], a[1], a[2], a[3])) )
                            status = error;
                        break;
                    case warning:
                        cout << "Warning: option -s with values " << optarg << endl;
                        if ( !(options->setSBg(true, a[0], a[1], a[2], a[3])) )
                            status = error;
                        break;
                    case error:
                        cout << "Error: option -s with values " << optarg << endl;
                }
                break;
            }
            
            case 'z': {
                cout << "Option -z " << endl;
                options->setZeroSup(true);
                break;
            }

            case '?':
            /* getopt_long already printed an error message. */
            break;

            default:
            abort ();
            }
        }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc) {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    exit (0);
}
