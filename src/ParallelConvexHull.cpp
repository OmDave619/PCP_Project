#include "ParallelConvexHull.hpp"
#include <algorithm>
#include <cmath>

ParallelConvexHull::ParallelConvexHull(int NumThreads) : NumThreads(NumThreads) {}

std::vector<std::vector<Point>> ParallelConvexHull::partitionPoints(std::vector<Point>& points) {
    int chunkSize = std::ceil(points.size() / static_cast<double>(NumThreads));
    std::vector<std::vector<Point>> partitions(NumThreads);
    
    int ind = 0;
    for(int i = 0; i < NumThreads; i++) {
        for(int j = 0; j < chunkSize && ind < points.size(); j++) {
            partitions[i].emplace_back(points[ind++]);
        }
    }
    return partitions;
}

std::vector<Point> ParallelConvexHull::mergeHulls(std::vector<Point>& left, std::vector<Point>& right) {
    // Placeholder implementation for merging two convex hulls
    // Implement the merging logic according to the method of finding upper and lower tangents
    return {}; // Placeholder
}