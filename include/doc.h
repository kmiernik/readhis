/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

// Dummy file for doxygen

/**
 * \mainpage readhis program
 *
 *  \brief The purpose of this program is to load histogram data
 *  from HIS/DRR file format (upak library), and convert them to ascii format.
 *
 *  Program parses options given by user (Options class) and creates
 *  HisDrrHisto class which loads the data and puts them into cout.
 *
 *  \section Installation
 *  The provided makefile should compile code and create 'readhis' executable.
 *  To run it simple type 'make'.
 *  Complitation was tested on Linux Fedora 16 and Arch Linux using g++ 4.6.
 *  Program depends only on C++ Standard Library so porting to other
 *  operating systems should be easy.
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
 *    -   --id : short (-i), selects histogram id   
 *              required by other options unless stated otherwise.   
 *     
 *    -  --gx x0,x1 : short (-x), 2D histograms only, requires two   
 *                    integer arguments separated by coma. Sets   
 *                    projection on X axis, starting from column x0 to x1  
 *                    (including both)  
 *     
 *    -   --gy x0,x1 : short (-y), as above, projection on Y axis   
 *     
 *    -   --bg x0,x1 : short (-b), 2D histograms only, only with gx or   
 *                    gy option. Sets gate for background subtraction,   
 *                    from x0 to x1 (including both).   
 *     
 *    -   --sbg x0,x1,x2,x3 : short (-s), 2D histograms only, only with   
 *                           gx or gy option. Same as above exept that    
 *                           gate is split into two parts x0 to x1 and  
 *                           x2 to x3.  
 *
 *    -   --bin bx[,by] : short (-B), defines number of histogram bins   
 *                       to join. For 2D histogram if only one argument   
 *                       is given by=bx is assumed. At least one bin size
 *                       must be > 1.  
 *     
 *    -   --zero : short (-z), suppresses bins with zero counts in output  
 *     
 *    -   --info : short (-I), displays detailed information on histogram  
 *     
 *    -  --list : short (-l), does not requires id. Displays list of  
 *                histograms present in the file.   
 *     
 *    -  --List : short (-L), does not requires id. As above,   
 *                additionally marks empty histograms and histogram dimensions.
 *     
 *    -  --help  : displays help   
 *
 * \section Graph
 * This graph explains the logic of program
 * \image html readhis_graph.png
 *
 * \section Copyrights
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
