#pragma once
#include "ConvexHull.hpp"
#include <algorithm>

class GrahamScan : public ConvexHull {
public:
    std::vector<Point> computeHull(std::vector<Point>& points) override;
private:
    void sortPoints(std::vector<Point>& points);
    bool isCounterClockwise(const Point& p1, const Point& p2, const Point& p3);
};
