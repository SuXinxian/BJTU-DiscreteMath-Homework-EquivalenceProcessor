//
// Created by Chypre on 12/8/2024.
//

#include "relation_processor.h"
#include <iostream>
#include <limits>

//---------------------- 构造函数与初始化 ----------------------

RelationProcessor::RelationProcessor(int size) : n(size) {
    if (size <= 0 || size > MAX_N) {
        throw std::invalid_argument("集合大小必须在1到MAX_N之间");
    }
    matrix.resize(n);
    initUnionFind();
}

//---------------------- 矩阵输入输出 ----------------------

void RelationProcessor::readRelation() {
    std::cout << "\n请选择输入方式：\n"
              << "1. 逐行输入矩阵 (每行输入 " << n << " 个数字)\n"
              << "2. 一次性输入整个 " << n << "x" << n << " 矩阵\n"
              << "请选择 (1/2): ";

    int choice;
    if (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
        throw std::invalid_argument("无效的选择");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n请输入关系矩阵 (只能包含0和1)：\n";

    if (choice == 1) {  // 逐行输入
        for (int i = 0; i < n; i++) {
            std::cout << "[第 " << i + 1 << "/" << n << " 行] ";
            readMatrixRow(i);
        }
    }
    else {  // 一次性输入整个矩阵
        for (int i = 0; i < n; i++) {
            readMatrixRow(i);
        }
    }
}

// 读取矩阵的一行
void RelationProcessor::readMatrixRow(int row) {
    for (int j = 0; j < n; j++) {
        int value;
        if (!(std::cin >> value) || (value != 0 && value != 1)) {
            throw std::invalid_argument("输入必须是0或1");
        }
        matrix[row][j] = value;
    }
}

void RelationProcessor::printMatrix() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

//---------------------- 关系性质判断 ----------------------

bool RelationProcessor::isEquivalence() const {
    return isReflexive() && isSymmetric() && isTransitive();
}

bool RelationProcessor::isReflexive() const {
    for (int i = 0; i < n; i++) {
        if (!matrix[i][i]) return false;
    }
    return true;
}

bool RelationProcessor::isSymmetric() const {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != matrix[j][i]) return false;
        }
    }
    return true;
}

bool RelationProcessor::isTransitive() const {
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (matrix[i][k]) {
                for (int j = 0; j < n; j++) {
                    if (matrix[k][j] && !matrix[i][j]) return false;
                }
            }
        }
    }
    return true;
}

//---------------------- 并查集操作 ----------------------

void RelationProcessor::initUnionFind() {
    parent.resize(n);
    rank.resize(n, 0);
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int RelationProcessor::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);  // 路径压缩
    }
    return parent[x];
}

void RelationProcessor::unionSets(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx != ry) {
        if (rank[rx] < rank[ry]) {
            std::swap(rx, ry);  // 按秩合并
        }
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) {
            rank[rx]++;
        }
    }
}

//---------------------- 等价类计算 ----------------------

std::vector<std::vector<int>> RelationProcessor::getPartition() {
    if (!isEquivalence()) {
        return std::vector<std::vector<int>>();
    }

    initUnionFind();
    // 根据关系矩阵构建等价类
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j]) {
                unionSets(i, j);
            }
        }
    }

    // 收集等价类
    std::map<int, std::vector<int>> classes;
    for (int i = 0; i < n; i++) {
        classes[find(i)].push_back(i);
    }

    // 转换为向量格式
    std::vector<std::vector<int>> result;
    for (const auto& [root, members] : classes) {
        result.push_back(members);
    }
    return result;
}

void RelationProcessor::computeMinimalEquivalence() {
    // 自反闭包
    for (int i = 0; i < n; i++) {
        matrix[i].set(i);
    }

    // 对称闭包
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j]) matrix[j].set(i);
            if (matrix[j][i]) matrix[i].set(j);
        }
    }

    // 传递闭包 (Warshall算法)
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            if (matrix[i][k]) {
                matrix[i] |= matrix[k];
            }
        }
    }
}

//---------------------- 静态方法 ----------------------

long long RelationProcessor::countEquivalenceRelations(int n) {
    if (n <= 0) return 0;
    if (n == 1) return 1;


    std::vector<long long> curr(n + 1), prev(n + 1, 0);
    prev[0] = 1;

    for (int i = 1; i <= n; i++) {
        curr[0] = prev[i-1];
        for (int j = 1; j <= i; j++) {
            curr[j] = curr[j-1] + prev[j-1];
        }
        prev = curr;
    }

    return prev[0];
}