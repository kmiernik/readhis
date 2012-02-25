/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#ifndef POLYGONH
#define POLYGONH

#include <vector>
#include <string>
#include <fstream>

/** Simple point object (x,y) carthesian coordinates.*/
class Point {
    public:
        Point (double a, double b) {
            x = a;
            y = b;
        }
        double x;
        double y;
};

/** Class for polygon gates. */
class Polygon {
    public:
        /** C'tor, polygon is created from given vector of Point objects. */
        Polygon(const std::vector< Point >& vertices);

        /** C'tor, polygon is created from points defined in a text file.
         * The given string should be the name of the file with vertices coords,
         * format is:
         *
         * #Comment
         * x0 y0
         * x1 y1
         * x2 y2
         * (...)
         *
         * At least 3 points are required.
         * */
        Polygon(const std::string& polygonFileName);

        /** C'tor polygon is created from points defined in a BAN (damm) file.*/
        Polygon(const std::string& polygonFileName, int banId);
        
        /** Checks if given point (xp, yp) is inside polygon (returns true).*/
        bool pointIn(double xp, double yp);

        /** Returns rectangle in which polygon is included. Rectangle edges
         * are perpendicular to X and Y axes of carthesian coordinage system. */
        bool rectangle(double& xlow, double& ylow, double& xhigh, double& yhigh);

    private:
        /** Vector of Point objects setting the vertices of polygon. */
        std::vector< Point > vertices_;
};


#endif
