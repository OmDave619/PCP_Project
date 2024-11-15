#pragma once 
#include <vector>   
#include "point.hpp"
#include "ConvexHull.hpp"

class ParallelConvexHull : public ConvexHull {
    public:
        ParallelConvexHull(int NumThreads);
    
    private:
        int NumThreads;
        std::vector<std::vector<Point>> partitionPoints(std::vector<Point>& points);
        std::vector<Point> mergeHulls(std::vector<Point>& left, std::vector<Point>& right);
};

