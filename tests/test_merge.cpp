#include "Divide_and_Conquer_ConvexHull.hpp"
#include "point.hpp"
#include "ConvexHull.hpp"
#include "GrahamScan.hpp"
#include <bits/stdc++.h>
using namespace std;

// make test
std::vector<Point> makeTest(int numPoints,int x_min, int x_max, int y_min, int y_max) {
    std::vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distX(x_min, x_max);
    uniform_real_distribution<double> distY(y_min, y_max);

    for (int i = 0; i < numPoints; ++i) {
        double x = distX(gen);
        double y = distY(gen);
        points.emplace_back(x, y);
    }

    sort(points.begin(), points.end());
    return points;
}

// run graham scan
std::vector<Point> runGrahamScan(std::vector<Point>& points) {
    GrahamScan grahamScan;
    return grahamScan.computeHull(points);
}

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
    vector<Point> input_left = makeTest(100, 0, 50, 0, 30);
    // vector<Point> input_left = {Point(0.322867, 2.353820), Point(1.590296, 7.134989), Point(2.314022, 3.176048), Point(4.227211, 7.397359), Point(4.705691, 3.465594), Point(4.708644, 1.280352), Point(5.095080, 1.206028), Point(5.534333, 2.651887), Point(6.367894, 0.471597), Point(9.635998, 5.565234)};
    vector<Point> leftHull = runGrahamScan(input_left);

    vector<Point> input_right = makeTest(100, 50, 70, 0, 30);
    // vector<Point> input_right = {Point(10.953084, 8.528669), Point(11.329431, 5.806742), Point(12.014740, 7.804263), Point(12.273737, 6.873049), Point(12.452800, 3.697127), Point(14.070159, 3.988598), Point(14.991037, 0.308690), Point(17.988924, 7.688821), Point(18.718463, 7.100990), Point(19.352241, 8.872765)};
    std::vector<Point> rightHull = runGrahamScan(input_right);

    // Perform the merge operation
    std::vector<Point> mergedHull = pch.mergeHulls(leftHull, rightHull);

    // Print the result
    int numPoints = 100;
    FILE *fp;
    fp = fopen("./tests/input.txt", "w");
    for (int i = 0; i < numPoints; ++i) {
        fprintf(fp, "%lf %lf\n", input_left[i].x, input_left[i].y);
    }
    for (int i = 0; i < numPoints; ++i) {
        fprintf(fp, "%lf %lf\n", input_right[i].x, input_right[i].y);
    }
    fp = fopen("./tests/output.txt", "w");
    for (int i = 0; i < mergedHull.size(); ++i) {
        fprintf(fp, "%lf %lf\n", mergedHull[i].x, mergedHull[i].y);
    }
    fclose(fp);

    std::cout << "Merged Hull: ";
    printHull(mergedHull);

    return 0;
}
