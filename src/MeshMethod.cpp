#include "MeshMethod.hpp"
#include "Divide_and_Conquer_ConvexHull.hpp"

#include<bits/stdc++.h>
#include<pthread.h>
using namespace std;

struct ThreadData {
    int threadId;
    std::vector<Point> points;
    std::vector<Point> resultHull;
};

// Thread function signature
void* computeHullPart(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    GrahamScan gs;
    data->resultHull = gs.computeHull(data->points);
    return nullptr;
}
struct MergeTask {
    std::vector<std::vector<Point>> hullsToMerge; // Hulls that this task needs to merge
    std::vector<Point> resultHull;                // Resulting hull from this merge task
};

MeshMethod::MeshMethod(int NumThreads,int mesh_cols) : NumThreads(NumThreads), mesh_cols(mesh_cols) {}

vector<Point> MeshMethod::computeHull(std::vector<Point>& points) {

    //instance of divide and conquer convex hull to partition the points 
    DnQ_ConvexHull pch(NumThreads);
    vector<vector<Point>> partitions = pch.partitionPoints(points);

    int merge_threads = NumThreads / mesh_cols;
    vector<pthread_t> merge_threads(merge_threads);
    vector<vector<vector<Point>>> task_hulls(merge_threads,vector<vector<Point>>(mesh_cols));

    //create threads
    vector<pthread_t> threads(NumThreads);
    vector<ThreadData> threadData(NumThreads);
    for(int i = 0; i < NumThreads; i++) {
        //create threads       
        threadData[i].threadId = i;
        threadData[i].points = partitions[i];

        // Create each thread passing it its own data
        if (pthread_create(&threads[i], nullptr, computeHullPart, &threadData[i])) {
            cerr << "Error creating thread " << i << endl;
            return {};
        }
    }

    //join all threads
    for(int i = 0; i < NumThreads; i++) {
        pthread_join(threads[i], nullptr);
        task_hulls[i/mesh_cols][i%mesh_cols] = threadData[i].resultHull;
    }



}