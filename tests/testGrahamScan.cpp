// #include "/home/priyesh/Personal/PCP_Project/include/GrahamScan.hpp"
#include "/home/priyesh/Personal/PCP_Project/include/point.hpp"
#include <bits/stdc++.h>
using namespace std;

#define MAX_Y 1000
#define MAX_X 1000
#define MIN_Y -1000
#define MIN_X -1000

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
// std::vector<Point> runGrahamScan(std::vector<Point>& points) {
//     GrahamScan grahamScan;
//     return grahamScan.computeHull(points);
// }

int main() {
    int numPoints = 1000;
    std::vector<Point> points = makeTest(numPoints);
    // std::vector<Point> hull = runGrahamScan(points);
    FILE *fp;
    fp = fopen("input.txt", "w");
    for (int i = 0; i < numPoints; ++i) {
        fprintf(fp, "%lf %lf\n", points[i].x, points[i].y);
    }
    // fp = fopen("output.txt", "w");
    // for (int i = 0; i < hull.size(); ++i) {
    //     fprintf(fp, "%lf %lf\n", hull[i].x, hull[i].y);
    // }
    fclose(fp);
    return 0;
}

