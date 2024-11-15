#include "Divide_and_Conquer_ConvexHull.hpp"
#include "point.hpp"
#include "ConvexHull.hpp"
#include "GrahamScan.hpp"
#include <iostream>
#include <vector>

// Helper function to print points of a hull
void printHull(const std::vector<Point>& hull) {
    for (const auto& point : hull) {
        std::cout << "(" << point.x << ", " << point.y << ") ";
    }
    std::cout << std::endl;
}

int main() {
    // Initialize the ParallelConvexHull object
    DnQ_ConvexHull pch(1);  // The number of threads is irrelevant for this test

    // Create two test convex hulls
    std::vector<Point> leftHull = {{0, 0}, {1, 1}, {0, 2}};
    std::vector<Point> rightHull = {{2, 0}, {3, 1}, {2, 2}};

    // Perform the merge operation
    std::vector<Point> mergedHull = pch.mergeHulls(leftHull, rightHull);

    // Print the result
    std::cout << "Merged Hull: ";
    printHull(mergedHull);

    return 0;
}
