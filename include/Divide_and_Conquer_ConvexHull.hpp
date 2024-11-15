#pragma once 
#include <vector>   
#include "point.hpp"
#include "ConvexHull.hpp"

class DnQ_ConvexHull {
    public:
        DnQ_ConvexHull(int NumThreads);
        std::vector<Point> mergeHulls(std::vector<Point>& left, std::vector<Point>& right);
        std::vector<std::vector<Point>> partitionPoints(std::vector<Point>& points);
        
    private:
        int NumThreads;
        int orientation(Point p, Point q, Point r);
};

