#pragma once 
#include <vector>   
#include "../include/point.hpp"
#include "../include/ConvexHull.hpp"

class ParallelConvexHull : public ConvexHull {
    public:
        ParallelConvexHull(int NumThreads);
    
    private:
        int NumThreads;
        int orientation(Point p, Point q, Point r);
        std::vector<std::vector<Point>> partitionPoints(std::vector<Point>& points);
        std::vector<Point> mergeHulls(std::vector<Point>& left, std::vector<Point>& right);
};

