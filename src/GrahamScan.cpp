#include "GrahamScan.hpp"
#include <algorithm>
#include <stack>
#include <iostream>

bool GrahamScan::compare(Point p1, Point p2, Point pivot) {
    int o = orientation(pivot, p1, p2);
    if (o == 0)
        return p1(pivot) < p2(pivot); // Keep the closer point
    return o == 2; // Counterclockwise points come first
}

std::vector<Point> GrahamScan::computeHull(std::vector<Point> points) {
    int n = points.size();

    // to decide
    // if (n < 3) {
    //     throw std::invalid_argument("Convex hull cannot be formed with fewer than 3 points");
    // }

    // Find the point with the lowest y-coordinate, break ties by x-coordinate
    int minYIndex = 0;
    for (int i = 1; i < n; ++i)
        if ((points[i].y < points[minYIndex].y) || (points[i].y == points[minYIndex].y && points[i].x < points[minYIndex].x))
            minYIndex = i;

    // Place the bottom-most point at the first position
    std::swap(points[0], points[minYIndex]);
    Point pivot = points[0];

    // Sort the remaining points based on polar angle with respect to pivot
    sortPoints(points);

    // Remove collinear points at the end of the sorted array
    int m = 1; // Initial size of modified array
    for (int i = 1; i < n; ++i) {
        while (i < n - 1 && orientation(pivot, points[i], points[i + 1]) == 0)
            ++i;
        points[m++] = points[i];
    }

    // to decide
    // if (m < 3) {
    //     throw std::invalid_argument("Convex hull cannot be formed after removing collinear points");
    // }

    // Create a stack and push first three points
    std::stack<Point> hull;
    hull.push(points[0]);
    hull.push(points[1]);
    hull.push(points[2]);

    // Process remaining points
    for (int i = 3; i < m; ++i) {
        // Remove points that do not form a counterclockwise turn
        while (hull.size() > 1) {
            Point top = hull.top(); hull.pop();
            Point secondTop = hull.top();
            if (orientation(secondTop, top, points[i]) != 2) {
                continue;
            }
            hull.push(top);
            break;
        }
        hull.push(points[i]);
    }

    // Transfer stack content to vector for output
    std::vector<Point> result;
    while (!hull.empty()) {
        result.push_back(hull.top());
        hull.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}

// assumes 1st point in pivot
void GrahamScan::sortPoints(std::vector<Point>& points) {
    Point pivot = points[0];
    std::sort(points.begin() + 1, points.end(), [this, pivot](Point p1, Point p2) { return this->compare(p1, p2, pivot); });
}

int GrahamScan::orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;            // collinear
    return (val > 0) ? 1 : 2;          // clockwise or counterclockwise
}
