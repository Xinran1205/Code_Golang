//
// Created by dell on 2023/10/12.
//
#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

class vEBTree {
private:
    int u;
    int min;
    int max;
    vEBTree *summary;
    std::vector<vEBTree*> cluster;

    // return the index of cluster which contains x
    int high(int x) {
        return x / sqrt(u);
    }

    // return the index of x in the cluster
    int low(int x) {
        return x % (int)sqrt(u);
    }

    // This function is the inverse operation of the above two functions.
    // It calculates the index of the element in the entire universe based on the index of
    // the subset(or cluster) and the position of the element in the subset.
    int index(int x, int y) {
        return x * sqrt(u) + y;
    }

public:

    vEBTree(int universe){
        u = universe;
        min = -1;
        max = -1;
        if (u <= 2) {
            summary = nullptr;
            cluster = std::vector<vEBTree*>(0);
        } else {
            int cluster_size = sqrt(u);
            summary = new vEBTree(cluster_size);
            // 一个cluster_size大小的数组，每个元素都是一个vEBTree的指针
            cluster = std::vector<vEBTree*>(cluster_size);
            for (int i = 0; i < cluster_size; i++) {
                cluster[i] = new vEBTree(cluster_size);
            }
        }
    }

    void insert(int x) {
        if (min == INT_MAX) {
            min = max = x;
            return;
        }
        if (x < min) std::swap(x, min);
        if (u > 2) {
            if (cluster[high(x)]->min == INT_MAX) {
                summary->insert(high(x));
                cluster[high(x)]->insert(low(x));
            } else {
                cluster[high(x)]->insert(low(x));
            }
        }
        if (x > max) max = x;
    }

};