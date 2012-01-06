#include <cstdlib>
#include <getopt.h>
    
static struct option long_options[] = {
    {"list",  no_argument, 0,       'l'},
    {"listZ", no_argument, 0,       'L'},
    {"gx",    required_argument, 0, 'x'},
    {"gy",    required_argument, 0, 'y'},
    {"bg",    required_argument, 0, 'b'},
    {"sbg",   required_argument, 0, 's'},
    {"zero",  required_argument, 0, 'z'},
    {0, 0, 0, 0}
};

class Flags {
    public:
        unsigned getHisId () { return hisId_; }
        void setHisId (unsigned const hisId) { hisId_ = hisId; }

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
            g0_ = g0;
            g1_ = g1;
        }

        bool getGy() { return isGy_; }
        bool setGy (const bool isGy, const unsigned g0, const unsigned g1) {
            if (g0 > g1)
                return false;
            isGy_ = isGy;
            g0_ = g0;
            g1_ = g1;
        }
        
        Flags() {
            hisId_ = 0;
            isListMode_ = false;
            isListModeZ_ = false;
            isZeroSup_ = false;
            isGx_ = false;
            isGy_ = false;
            isBg_ = false;
            isSbg_ = false;
            g0_ = 0;
            g1_ = 0;
            b0_ = 0;
            b1_ = 0;
            b2_ = 0;
            b3_ = 0;
        }
    private:
        unsigned hisId_;
        bool isListMode_;
        bool isListModeZ_;
        bool isZeroSup_;
        bool isGx_;
        bool isGy_;
        bool isBg_;
        bool isSbg_;
        unsigned g0_;
        unsigned g1_;
        unsigned b0_;
        unsigned b1_;
        unsigned b2_;
        unsigned b3_;
};
     
int main (int argc, char* argv[]) {
    int c;
       
     
       while (true)
         {
           /* getopt_long stores the option index here. */
           int option_index = 0;
     
           c = getopt_long (argc, argv, "abc:d:f:",
                            long_options, &option_index);
     
           /* Detect the end of the options. */
           if (c == -1)
             break;
     
           switch (c)
             {
             case 0:
               /* If this option set a flag, do nothing else now. */
               if (long_options[option_index].flag != 0)
                 break;
               printf ("option %s", long_options[option_index].name);
               if (optarg)
                 printf (" with arg %s", optarg);
               printf ("\n");
               break;
     
             case 'a':
               puts ("option -a\n");
               break;
     
             case 'b':
               puts ("option -b\n");
               break;
     
             case 'c':
               printf ("option -c with value `%s'\n", optarg);
               break;
     
             case 'd':
               printf ("option -d with value `%s'\n", optarg);
               break;
     
             case 'f':
               printf ("option -f with value `%s'\n", optarg);
               break;
     
             case '?':
               /* getopt_long already printed an error message. */
               break;
     
             default:
               abort ();
             }
         }
     
       /* Instead of reporting ‘--verbose’
          and ‘--brief’ as they are encountered,
          we report the final status resulting from them. */
       if (verbose_flag)
         puts ("verbose flag is set");
     
       /* Print any remaining command line arguments (not options). */
       if (optind < argc)
         {
           printf ("non-option ARGV-elements: ");
           while (optind < argc)
             printf ("%s ", argv[optind++]);
           putchar ('\n');
         }
     
       exit (0);
}
