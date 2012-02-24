/*
 * Copyright Krzysztof Miernik 2012
 * k.a.miernik@gmail.com 
 *
 * Distributed under GNU General Public Licence v3
 */

#include <sstream> 
#include <cstdlib> 
#include "Polygon.h"
#include "Exceptions.h"
#include "Debug.h"


Polygon::Polygon(const std::vector< Point >& vertices) {
    unsigned sz = vertices.size();
    if (sz < 3)
        throw GenError("Polygon must contain 3 or more vertices.");

    for(unsigned i = 0; i < sz; ++i)
        vertices_.push_back(vertices[i]);
}

Polygon::Polygon(const std::string& polygonFileName) {
    using namespace std;

    ifstream fInput(polygonFileName.c_str());
    if (!fInput.good()) {
        throw IOError("Could not open polygon file.");
    }
    
    string line;
    vector< Point > points;
    while (getline(fInput, line) ) {
        istringstream iss;
        iss.str(line);
        if (line[0] != '#' && line.size() > 1) {
            double xp;
            if (!(iss >> xp))
                throw GenError("Wrong entry in polygon file.");
            double yp;
            if (!(iss >> yp))
                throw GenError("Wrong entry in polygon file.");
            vertices_.push_back(Point(xp, yp));
        }
    }
    fInput.close();
    if (vertices_.size() < 3)
        throw GenError("Polygon must contain 3 or more vertices.");
}

Polygon::Polygon(const std::string& polygonFileName, int banId) {
    using namespace std;

    ifstream banf(polygonFileName.c_str());
    if (!banf.good()) {
        throw IOError("Could not open polygon file.");
    }

    string in;
    unsigned nPoints = 0;
    while (banf >> in) {
        if (in.find("INP") != string::npos) {
            // file name (SKIP)
            banf >> in;

            // his id (SKIP)
            banf >> in;
            
            // Banana id 
            banf >> in;
            int ban = atoi(in.c_str());
            // Check if matches given
            if (ban != banId)
                continue;

            // Axis projection in degrees (SKIP)
            banf >> in;

            // Number of points
            banf >> in;
            nPoints = atoi(in.c_str());
            
            // TIT string (undefined number of tokens)
            banf >> in;
            if (in == "TIT") {
                // Read the rest of the crap until points are present
                while (true) {
                    banf >> in;
                    if (in == "CXY")
                        break;
                }
            } else {
                throw GenError("Wrong BAN file format"); 
            }

            unsigned count = 0;
            while (count <= nPoints) {
                double x;
                double y;
                banf >> in;
                //Points are in pairs X Y, separated by CXY keywords
                if (in != "CXY") {
                   x = atof(in.c_str());
                   banf >> in;
                   y = atof(in.c_str());

                   vertices_.push_back(Point(x, y));
                }
                ++count;
            }
        }
    }
    banf.close();
    if (vertices_.size() < 3)
        throw GenError("BAN Polygon must contain 3 or more vertices.");

}
/**
 * Ray casting algotihm for Point in polygon problem.
 * ray starts from x=0, y=yp and goes to x=xp, y=yp
 */
bool Polygon::pointIn(double xp, double yp) {
    unsigned nEdges = vertices_.size();
    unsigned nIntersec = 0;

    for(unsigned i = 0; i < nEdges; ++i) {

        Point p0 = vertices_[i];
        Point p1(0.0, 0.0);

        if (i == nEdges - 1)
            p1 = vertices_[0];
        else
            p1 = vertices_[i + 1];
          
        // If point is exacly at the vertex it is by definition
        // (in this implementation) outside
        if ( (xp == p0.x && yp == p0.y) ||
             (xp == p0.x && yp == p0.y) )
            return false;
        
        // Edges do not belong to polygon in this implementation
        // so we may exclude case of horizontal line
        if (p0.y == p1.y)
            continue;

        // If yp is up or down given edge it won't cross it
        // for sure
        // Point 0 must be included (ray may go through the vertex)
        // Point 1 must be excluded (otherwise intersection will be
        // counted twice (this and next edge).
        if ( (yp <= p0.y && yp <= p1.y) ||
             (yp > p0.y && yp > p1.y) )
            continue;
        
        // Now find intersection between 
        // edge: f(x) = (y1-y0)/(x1-x0)*(x1-x) + y0
        // x-ray: r(x) = yp
        
        // x position of intersection is:
        double xs = (yp - p0.y) / (p1.y - p0.y) * (p1.x - p0.x) + p0.x;
        
        // If intersection is before we reach the point
        if (xs <= xp)
            ++nIntersec;
    }

    if (nIntersec % 2 == 0)
        return false;
    else
        return true;
}
