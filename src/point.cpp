#include "point.hpp"

Point::Point(double x, double y) : x(x), y(y) {}

bool Point::operator<(const Point& other) const {
    return x < other.x || (x == other.x && y < other.y);
}

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}