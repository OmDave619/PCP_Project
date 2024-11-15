#pragma once 
#include "point.hpp"
#include "GrahamScan.hpp"
#include "Divide_and_Conquer_ConvexHull.hpp"

class MeshMethod {
public:
    MeshMethod(int NumThreads, int mesh_cols); // Constructor initializes with a vector of points
    std::vector<Point> computeHull(std::vector<Point>& points);

private:
    int NumThreads;
    int mesh_cols;
};
