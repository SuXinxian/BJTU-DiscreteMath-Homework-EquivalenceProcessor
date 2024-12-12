//
// Created by Chypre on 12/8/2024.
//

#ifndef RELATION_PROCESSOR_H
#define RELATION_PROCESSOR_H

#include <vector>
#include <bitset>
#include <map>
#include <stdexcept>

const int MAX_N = 100;

class RelationProcessor {
public:
    RelationProcessor(int size);
    void readRelation();
    bool isEquivalence() const;
    std::vector<std::vector<int>> getPartition();
    void computeMinimalEquivalence();
    void printMatrix() const;
    static long long countEquivalenceRelations(int n);

private:
    int n;
    std::vector<std::bitset<MAX_N>> matrix;
    std::vector<int> parent;
    std::vector<int> rank;

    bool isReflexive() const;
    bool isSymmetric() const;
    bool isTransitive() const;
    void initUnionFind();
    int find(int x);
    void unionSets(int x, int y);
    void readMatrixRow(int row);  // 添加这个声明
};

#endif