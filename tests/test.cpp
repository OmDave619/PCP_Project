#include "GrahamScan.hpp"
#include "point.hpp"
#include <bits/stdc++.h>
using namespace std;

#define MAX_Y 5
#define MAX_X 10
#define MIN_Y 0
#define MIN_X 5

// make test
std::vector<Point> makeTest(int numPoints) {
    std::vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distX(MIN_X, MAX_X);
    uniform_real_distribution<double> distY(MIN_Y, MAX_Y);

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

int main() {
    int numPoints = 10;
    std::vector<Point> points = makeTest(numPoints);
    std::vector<Point> hull = runGrahamScan(points);
    FILE *fp;
    fp = fopen("./tests/input.txt", "w");
    for (int i = 0; i < numPoints; ++i) {
        fprintf(fp, "%lf %lf\n", points[i].x, points[i].y);
    }
    fp = fopen("./tests/output.txt", "w");
    for (int i = 0; i < hull.size(); ++i) {
        fprintf(fp, "%lf %lf\n", hull[i].x, hull[i].y);
    }
    fclose(fp);
    return 0;
}