#include "TreeMerge.hpp"
#include "GrahamScan.hpp"
#include "Divide_and_Conquer_ConvexHull.hpp"

#include <bits/stdc++.h>
#include <pthread.h>

struct ThreadArgs1 {
    std::vector<Point> points;
    std::vector<Point> hull;
};

struct ThreadArgs2 {
    std::vector<Point> hull1;
    std::vector<Point> hull2;
    std::vector<Point> hull;
};

void *computeHull(void *args) {
    ThreadArgs1 *threadArgs = (ThreadArgs1 *)args;
    GrahamScan grahamScan;
    threadArgs->hull = grahamScan.computeHull(threadArgs->points);
    return NULL;
}

void *thread_mergeHulls(void *args) {
    ThreadArgs2 *threadArgs = (ThreadArgs2 *)args;
    DnQ_ConvexHull DnQ(1);
    threadArgs->hull = DnQ.mergeHulls(threadArgs->hull1, threadArgs->hull2);
    return NULL;
}

std::vector<Point> TreeMerge::ConvexHull(std::vector<Point> &points, int n) {
    DnQ_ConvexHull DnQ(n);

    // Partition points into subsets
    std::vector<std::vector<Point>> partitions = DnQ.partitionPoints(points);

    // Validate partitions
    for (size_t i = 0; i < partitions.size(); ++i) {
        if (partitions[i].empty()) {
            throw std::runtime_error("Partition contains no points!");
        }
    }

    std::vector<std::vector<Point>> hulls(n);
    std::vector<pthread_t> threads(n);
    std::vector<ThreadArgs1 *> threadArgs(n);

    // Compute hulls for partitions in parallel
    for (int i = 0; i < n; i++) {
        ThreadArgs1 *args = new ThreadArgs1();
        threadArgs[i] = args;
        args->points = partitions[i];

        pthread_create(&threads[i], NULL, computeHull, (void *)args);
    }

    // Join threads and collect results
    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
        hulls[i] = threadArgs[i]->hull;
        delete threadArgs[i]; // Clean up allocated memory
    }

    // Merge hulls iteratively
    while (hulls.size() > 1) {
        int numThreads = hulls.size() / 2;
        std::vector<std::vector<Point>> newHulls(numThreads);
        std::vector<pthread_t> newThreads(numThreads);
        std::vector<ThreadArgs2*> mergeArgs(numThreads);

        for (int i = 0; i < numThreads; i++) {
            ThreadArgs2 *args = new ThreadArgs2();
            mergeArgs[i] = args;
            args->hull1 = hulls[2 * i];
            args->hull2 = hulls[2 * i + 1];

            pthread_create(&newThreads[i], NULL, thread_mergeHulls, (void *)args);
        }

        // Join threads and collect results
        for (int i = 0; i < numThreads; i++) {
            pthread_join(newThreads[i], NULL);
            newHulls[i] = mergeArgs[i]->hull;
            delete mergeArgs[i]; // Clean up allocated memory
        }

        // if (hulls.size() % 2 != 0) { // Handle leftover hull if odd number
        //     newHulls.push_back(hulls.back());
        // }

        hulls = std::move(newHulls);
    }

    return hulls[0];
}