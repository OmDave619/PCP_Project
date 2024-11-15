#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include "GrahamScan.hpp"
#include "MeshMethod.hpp"
#include "TreeMerge.hpp"
#include "point.hpp"

using namespace std;

#define MAX_Y 10000
#define MAX_X 10000
#define MIN_Y -10000
#define MIN_X -10000

std::vector<Point> makeTest(int numPoints) {
    std::vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> distX(MIN_X, MAX_X);
    uniform_real_distribution<> distY(MIN_Y, MAX_Y);

    for (int i = 0; i < numPoints; ++i) {
        points.emplace_back(distX(gen), distY(gen));
    }

    sort(points.begin(), points.end());
    return points;
}

std::vector<Point> runGrahamScan(std::vector<Point>& points) {
    GrahamScan grahamScan;
    return grahamScan.computeHull(points);
}

int main() {
    int numPoints = 1e7; // 50 million points
    std::vector<Point> points = makeTest(numPoints);

    // Measure Graham Scan
    auto start = chrono::high_resolution_clock::now();
    std::vector<Point> hullGraham = runGrahamScan(points);
    auto stop = chrono::high_resolution_clock::now();
    auto durationGraham = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Graham Scan took: " << durationGraham.count() << " milliseconds" << endl;

    // Measure Mesh Method
    MeshMethod meshMerge(16, 4); // Assuming 16 threads and 4 columns
    start = chrono::high_resolution_clock::now();
    std::vector<Point> hullMesh = meshMerge.computeHull(points);
    stop = chrono::high_resolution_clock::now();
    auto durationMesh = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Mesh Method took: " << durationMesh.count() << " milliseconds" << endl;

    // Measure Tree Merge
    TreeMerge treeMerge;
    start = chrono::high_resolution_clock::now();
    std::vector<Point> hullTree = treeMerge.ConvexHull(points, 16); // Assuming 16 threads
    stop = chrono::high_resolution_clock::now();
    auto durationTree = chrono::duration_cast<chrono::milliseconds>(stop - start);
    cout << "Tree Merge took: " << durationTree.count() << " milliseconds" << endl;

    return 0;
}
