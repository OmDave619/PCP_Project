#include "Divide_and_Conquer_ConvexHull.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

DnQ_ConvexHull::DnQ_ConvexHull(int NumThreads) : NumThreads(NumThreads) {}

std::vector<std::vector<Point>> DnQ_ConvexHull::partitionPoints(std::vector<Point>& points) {
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

std::vector<Point> DnQ_ConvexHull::mergeHulls(const std::vector<Point>& left, const std::vector<Point>& right) {
    int n_left = left.size();
    int n_right = right.size();

    // Find the rightmost point in left hull
    int indl = 0;
    for(int i = 1; i < n_left; i++) {
        if(left[i].x > left[indl].x) {
            indl = i;
        }
    }

    // Find the leftmost point in right hull
    int indr = 0;
    for(int i = 1; i < n_right; i++) {
        if(right[i].x < right[indr].x) {
            indr = i;
        }
    }

    // Find the upper tangent
    int upper_indl = indl;
    int upper_indr = indr;
    bool done = false;
    while(!done) {
        done = true;
        // Move upper_indl forward while the orientation indicates a counter-clockwise turn
        while(orientation(right[upper_indr], left[upper_indl], left[(upper_indl + 1) % n_left]) >= 0) {
            upper_indl = (upper_indl + 1) % n_left;
        }
        // Move upper_indr backward while the orientation indicates a clockwise turn
        while(orientation(left[upper_indl], right[upper_indr], right[(upper_indr - 1 + n_right) % n_right]) <= 0) {
            upper_indr = (upper_indr - 1 + n_right) % n_right;
            done = false;
        }
    }

    // Find the lower tangent
    int lower_indl = indl;
    int lower_indr = indr;
    done = false;
    while(!done) {
        done = true;
        // Move lower_indr forward while the orientation indicates a clockwise turn
        while(orientation(left[lower_indl], right[lower_indr], right[(lower_indr + 1) % n_right]) >= 0) {
            lower_indr = (lower_indr + 1) % n_right;
        }
        // Move lower_indl backward while the orientation indicates a counter-clockwise turn
        while(orientation(right[lower_indr], left[lower_indl], left[(lower_indl - 1 + n_left) % n_left]) <= 0) {
            lower_indl = (lower_indl - 1 + n_left) % n_left;
            done = false;
        }
    }

    // Merge the hulls
    std::vector<Point> mergedHull;

    // Traverse the left hull from upper_indl to lower_indl (inclusive)
    int ind = upper_indl;
    mergedHull.emplace_back(left[ind]);
    while(ind != lower_indl) {
        ind = (ind + 1) % n_left;
        mergedHull.emplace_back(left[ind]);
    }

    // Traverse the right hull from lower_indr to upper_indr (inclusive)
    ind = lower_indr;
    mergedHull.emplace_back(right[ind]);
    while(ind != upper_indr) {
        ind = (ind + 1) % n_right;
        mergedHull.emplace_back(right[ind]);
    }

    return mergedHull;
}

int DnQ_ConvexHull::orientation(const Point& a, const Point& b, const Point& c) {   
    // long det = static_cast<long>(q.x - p.x) * (r.y - p.y) - static_cast<long>(q.y - p.y) * (r.x - p.x);
    long det = static_cast<long>(c.x - b.x) * (b.y - a.y) - static_cast<long>(c.y - b.y) * (b.x - a.x);
    if(det == 0) return 0;   // Collinear
    if(det > 0) return 1;    // Counter-clockwise
    else return -1;          // Clockwise
}
