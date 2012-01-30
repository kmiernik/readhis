/*
 * K. Miernik (k.a.miernik@gmail.com) 2012
 *
 */

// Dummy file for doxygen

/**
 * \mainpage readhis program
 *
 *  \brief The purpose of this program is to load data histogram data 
 *  from .his format (ORNL / DAMM).
 *  Program parses options given by user (Options class) and creates
 *  HisDrrHisto class which loads the data and puts them into cout.
 *
 * \section Usage
 *       readhis [options] file.his   
 *     
 * \section Description  
 *       Program reads his file using binary description from drr file.  
 *       Both files are required to be present in the specified path   
 *       and with the same base name (e.g. t1.his and t1.drr).  
 *       Result is send to standard output, use redirection   
 *       in case you want to save it to file.  
 *     
 * \section Options  
 *       --id : short (-i), selects histogram id   
 *              required by other options unless stated otherwise.   
 *     
 *       --gx x0,x1 : short (-x), 2D histograms only, requires two   
 *                    integer arguments separated by coma. Sets   
 *                    projection on X axis, starting from column x0 to x1  
 *                    (including both)  
 *     
 *       --gy x0,x1 : short (-y), as above, projection on Y axis   
 *     
 *       --bg x0,x1 : short (-b), 2D histograms only, only with gx or   
 *                    gy option. Sets gate for background subtraction,   
 *                    from x0 to x1 (including both).   
 *     
 *       --sbg x0,x1,x2,x3 : short (-s), 2D histograms only, only with   
 *                           gx or gy option. Same as above exept that    
 *                           gate is split into two parts x0 to x1 and  
 *                           x2 to x3.  
 *       --bin bx[,by] : short (-B), defines number of histogram bins   
 *                       to join. For 2D histogram if only one argument   
 *                       is given by=bx is assumed. Bin size must be > 1.  
 *     
 *       --zero : short (-z), suppresses bins with zero counts in output  
 *     
 *       --info : short (-I), displays detailed information on histogram  
 *     
 *       --list : short (-l), does not requires id. Displays list of  
 *                histograms present in the file.   
 *     
 *       --List : short (-L), does not requires id. As above,   
 *                additionally marks empty histograms and histogram dimensions.
 *     
 *       --help  : displays this help   
 *
 * \section Logic graph
 * This graph explains the logic of program
 * \image html readhis_graph.svg
 */
