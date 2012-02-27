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
 *  Output contains data and calculated uncertainties. The comment lines in
 *  the output is marked by '#' character. These data come in plain text
 *  format, so it should be easy to plot them directly (I recommend gnuplot,
 *  which also uses '#' as comment characters) or import them into spreadsheet
 *  using LibreOffice Calc or similar.
 *  
 *
 *  \section Installation
 *  The provided makefile should compile code and create 'readhis' executable.
 *  To run it simple type 'make', 'make install' copies readhis to
 *  /usr/local/bin path for system wide access.
 *
 *  Compilation was tested on Linux Fedora 16 and Arch Linux using g++ 4.6.
 *  Program depends on C++ Standard Library only, so porting to other
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
 * - Option:	--id
 *
 * 	Short: -i
 *
 * 	Description: 
 * 	
 * 		Selects histogram id, required by all other options unless 
 * 		stated otherwise. 
 *
 * - Option:	--gx AND (x0,x1 OR filename OR filename,id)
 *
 * 	Short: -x
 *
 * 	Description: 
 *
 * 		For 2D histograms only, requires two integer arguments 
 * 		separated by coma. Sets projection on Y axis (gate on X). 
 * 		starting from column x0 to x1 (including both). If filename 
 * 		is given, the file must be a text file defining polygon, 
 * 		using the following format: 
 *
 * 		#Comment line 
 *
 * 		x0 y0 
 *
 * 		x1 y1 
 *
 * 		(...) 
 *
 * 		
 * 		at least 3 points are required. 
 *
 * 		If filename,id syntax is used, the file must be a BAN 
 * 		(damm) file, and id is a damm banana id to be used. 
 * 
 * -	Option:	--gy AND (y0,y1 OR filename OR filename,id)
 *
 * 	Short: -y
 * 	
 * 	Description: 
 *
 * 		As above, exept that projection is made on X axis (gate on 
 * 		Y). 
 * 
 * -	Option:	--bg AND x0,x1
 *
 * 	Short: -b
 *
 * 	Description: 
 *
 * 		For 2D histograms only, only with --gx or --gy. requires 
 * 		two integer arguments separated by coma. Sets gate for 
 * 		background subtraction from channel containing x0 to x1 
 * 		(including both). 
 * 
 * -	Option:	--sbg AND x0,x1,x2,x3
 *
 * 	Short: -b
 *
 * 	Description: 
 *
 * 		For 2D histograms only, only with --gx or --gy. Same as 
 * 		above exept that gate is split into two parts x0 to x1 and 
 * 		x2 to x3. 
 * 
 * -	Option:	--bin AND (bx OR bx,by)
 *
 * 	Short: -B
 *
 * 	Description: 
 *
 * 		Defines number of histogram bins to join. For 2D histogram 
 * 		if only one argument is given by = bx is assumed. At least 
 * 		one bin size must be > 1. 
 * 
 * -	Option:	--zero
 *
 * 	Short: -z
 *
 * 	Description: 
 *
 * 		Suppresses bins with zero counts in output (1D Histograms 
 * 		only) 
 * 
 * -	Option:	--info
 *
 * 	Short: -I
 *
 * 	Description: 
 *
 * 		Displays detailed information on histogram. 
 * 
 * -	Option:	--list
 *
 * 	Short: -l
 *
 * 	Description: 
 *
 * 		Does not require histogram id. Displays list of all 
 * 		histograms present in a given file. 
 * 
 * -	Option:	--List
 *
 * 	Short: -L
 *
 * 	Description: 
 *
 * 		Does not require histogram id. As above except that 
 * 		displays number of dimensions for each histogram and marks 
 * 		empty histograms with 'E' letter next to id. 
 * 
 * -	Option:	--help
 *
 * 	Short: -h
 *
 * 	Description: 
 *
 * 		Shows help. 
 *       
 * \section Examples
 *  Let's suppose that we have data in run01.his and run01.drr files. The
 *  readhis and data are in the same directory. The following examples follow
 *  logic of examining the data.
 *  - Display list of all histograms
 *
 *    $ readhis --list run01.his
 *
 *  - Display list of all histograms, for each show number of dimensions
 *    and mark which are empty
 *
 *    $ readhis --List run01.his
 *
 *  - Display detailed information about histogram 1501
 *
 *    $ readhis --id 1501 --info run01.his
 *
 *  - Extract data for histogram 1501 into text file 1501.txt
 *
 *    $ readhis --id 1501 run01.his > 1501.txt
 *
 *  - As above but bin data, with bin size 3
 *
 *    $ readhis --id 1501 --bin 3 run01.his > 1501.txt
 *
 *  - Display detailed information about 2 dimensional histogram 1734
 *
 *    $ readhis --id 1734 --info run01.his
 *
 *  - Extract data for histogram 1734 into text file 1734.txt
 *
 *    $ readhis --id 1734 run01.his > 1734.txt
 *
 *  - As above but request bin size in X axis 1, and bin size Y - 10
 *
 *    $ readhis --id 1734 --bin 1,10 run01.his > 1734.txt
 *
 *  - Put gate on X axis (makes projection on Y axis) between channels 266 
 *    and 269 in histogram 1734. Put result into gate.txt text file.
 *
 *    $ readhis -- id 1734 --gx 266,269 run01.his > gate.txt
 *
 *  - As above but set gate for background removal between channels 272 
 *    and 275.
 *
 *    $ readhis --id 1734 --gx 266,269 --bg 272,275 run01.his > gate.txt
 *
 *  - As above but set background gate split into two parts (e.g on both sides
 *    of the peak) between channels 262 to 263 and 266 to 267
 *
 *    $ readhis --id 1734 --gx 266,269 --sbg 262,263,266,267 run01.his > gate.txt
 *  - Put gate on Y axis (makes projection on X axis) between channels 100
 *    and 150. Put result on screen.
 *
 *    $ readhis --id 1734 --gy 100,150 run01.his
 *
 *  - Put gate on Y axis (makes projection on X axis) using pol.txt file with
 *    polygon gate definition. Put result to ban01.txt file
 *
 *    $ readhis --id 1734 --gy pol.txt run01.his > ban01.txt
 *
 *  - Put gate on Y axis (makes projection on X axis) using pol.ban file
 *    generated by damm, select banana id 1. Put result to ban02.txt.
 *
 *    $ readhis --id 1734 --gy pol.ban,1 run01.his > ban02.txt
 *
 *  - List all histograms in file run02.his and run02.drr, placed in 
 *    a different directory (relative path is ../RUN02/)
 *
 *    $ readhis --list ../RUN02/run02.his
 *
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
