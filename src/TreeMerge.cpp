#include "TreeMerge.hpp"
#include "GrahamScan.hpp"
#include "Divide_and_Conquer_ConvexHull.hpp"

#include <bits/stdc++.h>
#include <pthread.h>

struct ThreadArgs1 {
    std::vector<Point> *points;
    GrahamScan *grahamScan;
    std::vector<Point> *hull;
};

struct ThreadArgs2 {
    std::vector<Point> *hull1;
    std::vector<Point> *hull2;
    DnQ_ConvexHull *DnQ;
    std::vector<Point> *hull;
};

void *computeHull(void *args) {
    ThreadArgs1 *threadArgs = (ThreadArgs1 *)args;
    GrahamScan *grahamScan = threadArgs->grahamScan;
    std::vector<Point> *hull = threadArgs->hull;
    hull = &grahamScan->computeHull(*threadArgs->points);
    return NULL;
}

void *mergeHulls(void *args) {
    ThreadArgs2 *threadArgs = (ThreadArgs2 *)args;
    DnQ_ConvexHull *DnQ = threadArgs->DnQ;
    std::vector<Point> *hull = threadArgs->hull;
    hull = &DnQ->mergeHulls(*threadArgs->hull1, *threadArgs->hull2);
    return NULL;
}

std::vector<Point> TreeMerge::ConvexHull(std::vector<Point>& points, int n) {
    GrahamScan grahamScan;
    DnQ_ConvexHull DnQ(n);

    std::vector<std::vector<Point>> partitions = DnQ.partitionPoints(points);

    std::vector<std::vector<Point>> hulls(n);
    std::vector<pthread_t> threads(n);

    for (int i = 0; i < n; i++) {
        ThreadArgs1 *args = new ThreadArgs1();
        args->points = &partitions[i];
        args->grahamScan = &grahamScan;
        args->hull = &hulls[i];

        pthread_create(&threads[i], NULL, computeHull, (void *)args);
    }

    // join threads
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

    while (hulls.size() > 1) {  
    {
        int numThreads = hulls.size()/2;
        std::vector<std::vector<Point>> newHulls(numThreads);
        std::vector<pthread_t> newThreads(numThreads);

        for (int i = 0; i < numThreads; i++) {
            ThreadArgs2 *args = new ThreadArgs2();
            args->hull1 = &hulls[2*i];
            args->hull2 = &hulls[2*i+1];
            args->DnQ = &DnQ;
            args->hull = &newHulls[i];

            pthread_create(&newThreads[i], NULL, mergeHulls, (void *)args);
        }

        // join threads
        for (int i = 0; i < numThreads; i++) {
            pthread_join(newThreads[i], NULL);
        }

        hulls = std::move(newHulls);
    }

    return hulls[0];
}