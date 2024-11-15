#pragma once
#include "../include/point.hpp"
#include <vector>

class ConvexHull {
    public:
        virtual std::vector<Point> computeHull(std::vector<Point> &points) = 0;
};