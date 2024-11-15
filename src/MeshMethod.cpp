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

// Thread function to merge a set of hulls
void* mergeHulls(void* arg) {
    MergeTask* task = static_cast<MergeTask*>(arg);
    DnQ_ConvexHull meshMerger(1);   // for merging 2 hulls at a time instance

    //sequential merge for a set of hulls
    std::vector<Point> finalHull = task->hullsToMerge[0];
    for (size_t i = 1; i < task->hullsToMerge.size(); ++i) {
        finalHull = meshMerger.mergeHulls(finalHull, task->hullsToMerge[i]);
    }

    task->resultHull = finalHull;
    return nullptr;
}


MeshMethod::MeshMethod(int NumThreads,int mesh_cols) : NumThreads(NumThreads), mesh_cols(mesh_cols) {}

vector<Point> MeshMethod::computeHull(std::vector<Point>& points) {

    //instance of divide and conquer convex hull to partition the points 
    DnQ_ConvexHull pch(NumThreads);
    vector<vector<Point>> partitions = pch.partitionPoints(points);

    int mesh_rows = NumThreads / mesh_cols;
    vector<pthread_t> merge_threads(mesh_rows);
    vector<vector<vector<Point>>> task_hulls(mesh_rows ,vector<vector<Point>>(mesh_cols));

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

    //create merge threads
    vector<MergeTask> merge_tasks(mesh_rows);
    for(int i = 0; i < mesh_rows; i++) {
        for(int j = 0; j < mesh_cols; j++) {
            merge_tasks[i].hullsToMerge.push_back(task_hulls[i][j]);
        }
    }

    for(int i = 0; i < mesh_rows; i++) {
        if (pthread_create(&merge_threads[i], nullptr, mergeHulls, &merge_tasks[i])) {
            std::cerr << "Failed to create thread" << std::endl;
            return {};
        }
    }

    std::vector<std::vector<Point>> intermediateHulls;
    for (int i = 0; i < mesh_rows; ++i) {
        pthread_join(merge_threads[i], nullptr);
        intermediateHulls.push_back(merge_tasks[i].resultHull);
    }

    // Final merge
    std::vector<Point> finalHull = intermediateHulls[0];
    for (size_t i = 1; i < intermediateHulls.size(); ++i) {
        finalHull = pch.mergeHulls(finalHull, intermediateHulls[i]);
    }
    
    return finalHull;
}