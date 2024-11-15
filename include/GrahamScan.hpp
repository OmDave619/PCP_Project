#pragma once
#include "ConvexHull.hpp"
#include <algorithm>

class GrahamScan : public ConvexHull {
public:
    std::vector<Point> computeHull(std::vector<Point> points) override;
private:
    void sortPoints(std::vector<Point>& points);
    int orientation(Point p, Point q, Point r);
    bool compare(Point p1, Point p2, Point pivot);
};
