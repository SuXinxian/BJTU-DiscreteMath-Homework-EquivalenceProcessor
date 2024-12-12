//
// Created by Chypre on 12/8/2024.
//
#ifndef TEST_GENERATOR_H
#define TEST_GENERATOR_H

#include <vector>    // 用于存储矩阵
#include <string>    // 用于文件名和关系类型
#include <random>    // 用于随机数生成器
#include <chrono>


class RelationGenerator {
public:
    RelationGenerator();

    // 生成不同类型的关系矩阵
    std::vector<std::vector<int>> generateRandomRelation(int n, double density = 0.5);
    std::vector<std::vector<int>> generateEquivalenceRelation(int n, int numClasses);
    std::vector<std::vector<int>> generateSpecificRelation(int n, const std::string& type);
    std::vector<std::vector<int>> generateAlmostEquivalenceRelation(int n);
    // 测试套件生成和验证
    void generateTestSuite(const std::string& filename);
    void runConsistencyTests();

private:
    std::mt19937 gen;  // 随机数生成器

    // 关系性质修改器
    void makeSymmetric(std::vector<std::vector<int>>& matrix, int n);
    void makeReflexive(std::vector<std::vector<int>>& matrix, int n);
    void makeTransitive(std::vector<std::vector<int>>& matrix, int n);

    // 关系性质检查器
    struct RelationProperties {
        bool isReflexive;
        bool isSymmetric;
        bool isTransitive;
        bool isEquivalence;
    };
    RelationProperties checkProperties(const std::vector<std::vector<int>>& matrix, int n);
};

#endif // TEST_GENERATOR_H
