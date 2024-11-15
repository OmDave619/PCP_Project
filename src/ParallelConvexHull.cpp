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

    int n_left = left.size();
    int n_right = right.size();

    //finding the righmost point in left hull
    Point rightMost = left[0];
    int indl = 0;
    for(int i = 1; i < n_left; i++) {
        if(left[i].x > rightMost.x) {
            rightMost = left[i];
            indl = i;
        }
    }

    //finding the leftmost point in right hull
    Point leftMost = right[0];
    int indr = 0;
    for(int i = 1; i < n_right; i++) {
        if(right[i].x < leftMost.x) {
            leftMost = right[i];
            indr = i;
        }
    }

    //finding the upper tangent 
    Point p1 = rightMost, p2 = leftMost;
    int ind1 = indl, ind2 = indr;   //ind1 iterates in left hull and ind2 iterates in right hull
    bool done = false;
    while(!done) {
        done = true;
        while(orientation(right[ind2], left[ind1], left[(ind1 + 1) % n_left]) == 1) {   //counter clockwise orientation
            ind1 = (ind1 + 1) % n_left;
        }

        while(orientation(left[ind1], right[ind2], right[(ind2 - 1 + n_right) % right.size()]) == -1) { //clockwise orientation
            ind2 = (ind2 - 1 + n_right) % n_right;
            done = false;
        }
    }
    int upper_indl = ind1, upper_indr = ind2;

    //finding the lower tangent
    done = false;
    ind1 = indl, ind2 = indr;
    while(!done) {
        done = true;
        while(orientation(left[ind1], right[ind2], right[(ind2 + 1) % n_right]) == -1) { //clockwise orientation
            ind2 = (ind2 + 1) % n_right;
        }

        while(orientation(right[ind2], left[ind1], left[(ind1 - 1 + n_left) % n_left]) == 1) {   //counter clockwise orientation
            ind1 = (ind1 - 1 + n_left) % n_left;
            done = false;
        }
    }
    int lower_indl = ind1, lower_indr = ind2;

    //merging the two hulls
    std::vector<Point> mergedHull;
    int ind = upper_indl;
    while(ind != lower_indl) {
        mergedHull.emplace_back(left[ind]);
        ind = (ind + 1) % n_left;
    }

    ind = lower_indr;
    while(ind != upper_indr) {
        mergedHull.emplace_back(right[ind]);
        ind = (ind + 1) % n_right;
    }

    return mergedHull; 
}

int ParallelConvexHull::orientation(Point p, Point q, Point r) {   
    int det = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if(det == 0) return 0;   // Collinear
    if(det > 0) return 1;   // Counter-clockwise
    else return -1;         // Clockwise
}