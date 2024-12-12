//
// Created by Chypre on 12/8/2024.
//

#include "test_generator.h"
#include <iostream>   // 用于控制台输出
#include <fstream>    // 用于文件操作
#include <cassert>    // 用于断言
#include <algorithm>  // 用于 std::shuffle

//---------------------- 构造函数 ----------------------

RelationGenerator::RelationGenerator()
    : gen(std::chrono::system_clock::now().time_since_epoch().count()) {}

//---------------------- 关系性质修改器 ----------------------

void RelationGenerator::makeReflexive(std::vector<std::vector<int>>& matrix, int n) {
    // 将主对角线上的所有元素设为1，确保自反性
    for (int i = 0; i < n; i++) {
        matrix[i][i] = 1;
    }
}

void RelationGenerator::makeSymmetric(std::vector<std::vector<int>>& matrix, int n) {
    // 对每个非对角线元素，确保(i,j)和(j,i)相等
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            matrix[j][i] = matrix[i][j];
        }
    }
}

void RelationGenerator::makeTransitive(std::vector<std::vector<int>>& matrix, int n) {
    // 使用Warshall算法实现传递闭包
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][k] && matrix[k][j]) {
                    matrix[i][j] = 1;
                }
            }
        }
    }
}

//---------------------- 关系生成器 ----------------------

std::vector<std::vector<int>> RelationGenerator::generateRandomRelation(int n, double density) {
    // 生成n×n的随机关系矩阵，density控制1的密度
    std::uniform_real_distribution<> dis(0.0, 1.0);
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (dis(gen) < density) {
                matrix[i][j] = 1;
            }
        }
    }
    return matrix;
}

std::vector<std::vector<int>> RelationGenerator::generateEquivalenceRelation(int n, int numClasses) {
    // 通过生成随机划分来构造等价关系
    if (numClasses > n) numClasses = n;

    std::vector<std::vector<int>> matrix(n, std::vector<int>(n, 0));
    std::vector<int> partition(n);

    // 初始化划分
    for (int i = 0; i < n; i++) {
        partition[i] = i % numClasses;
    }

    // 随机打乱划分以生成随机的等价类
    std::shuffle(partition.begin(), partition.end(), gen);

    // 根据划分构造关系矩阵
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (partition[i] == partition[j]) {
                matrix[i][j] = 1;
            }
        }
    }

    return matrix;
}

std::vector<std::vector<int>> RelationGenerator::generateSpecificRelation(int n, const std::string& type) {
    // 根据指定类型生成具有特定性质的关系
    if (type == "reflexive") {
        auto matrix = generateRandomRelation(n, 0.3);
        makeReflexive(matrix, n);
        return matrix;
    }
    else if (type == "symmetric") {
        auto matrix = generateRandomRelation(n, 0.3);
        makeSymmetric(matrix, n);
        return matrix;
    }
    else if (type == "transitive") {
        return generateEquivalenceRelation(n, n/2);
    }

    throw std::invalid_argument("未知的关系类型: " + type);
}

//---------------------- 关系性质检查器 ----------------------

RelationGenerator::RelationProperties RelationGenerator::checkProperties(
    const std::vector<std::vector<int>>& matrix, int n) {
    RelationProperties props = {true, true, true, false};

    // 检查自反性
    for (int i = 0; i < n; i++) {
        if (!matrix[i][i]) {
            props.isReflexive = false;
            break;
        }
    }

    // 检查对称性
    for (int i = 0; i < n && props.isSymmetric; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                props.isSymmetric = false;
                break;
            }
        }
    }

    // 检查传递性
    for (int i = 0; i < n && props.isTransitive; i++) {
        for (int j = 0; j < n; j++) {
            if (matrix[i][j]) {
                for (int k = 0; k < n; k++) {
                    if (matrix[j][k] && !matrix[i][k]) {
                        props.isTransitive = false;
                        goto transitive_break;  // 使用goto跳出多重循环
                    }
                }
            }
        }
    }
    transitive_break:

    props.isEquivalence = props.isReflexive && props.isSymmetric && props.isTransitive;
    return props;
}

//---------------------- 测试套件生成 ----------------------

void RelationGenerator::generateTestSuite(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        throw std::runtime_error("无法创建测试用例文件: " + filename);
    }

    struct TestCase {
        int size;
        std::string description;
        int numEquivClasses;
        double nonEquivDensity;
    };

    std::vector<TestCase> testCases = {
        {1, "单元素测试", 1, 0.0},
        {2, "最小非平凡测试", 1, 0.4},
        {5, "中等规模测试", 2, 0.4},
        {20, "大规模测试", 5, 0.3},
        {99, "极限规模测试", 7, 0.3}
    };

    for (const auto& testCase : testCases) {
        outFile << "# " << testCase.description << " - 等价关系\n"
               << testCase.size << "\n";

        auto equivRelation = generateEquivalenceRelation(
            testCase.size,
            testCase.numEquivClasses
        );

        for (int i = 0; i < testCase.size; i++) {
            for (int j = 0; j < testCase.size; j++) {
                outFile << equivRelation[i][j] << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";

        outFile << "# " << testCase.description << " - 非等价关系\n"
               << testCase.size << "\n";

        std::vector<std::vector<int>> nonEquivRelation;

        if (testCase.size >= 20) {
            nonEquivRelation = generateEquivalenceRelation(testCase.size, testCase.numEquivClasses);
            std::uniform_int_distribution<> dis(0, testCase.size - 1);
            int modifications = testCase.size / 3;

            for (int i = 0; i < modifications; i++) {
                int x = dis(gen);
                int y = dis(gen);
                if (x != y) {
                    nonEquivRelation[x][y] = 1;
                    int z = dis(gen);
                    if (z != x && z != y) {
                        nonEquivRelation[y][z] = 1;
                    }
                }
            }
        } else {
            nonEquivRelation = generateRandomRelation(
                testCase.size,
                testCase.nonEquivDensity
            );
        }

        for (int i = 0; i < testCase.size; i++) {
            for (int j = 0; j < testCase.size; j++) {
                outFile << nonEquivRelation[i][j] << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";
    }

    outFile.close();
}

//---------------------- 一致性测试 ----------------------

void RelationGenerator::runConsistencyTests() {
    std::cout << "开始运行一致性测试...\n";

    // 测试等价关系生成器
    for (int n : {2, 3, 5}) {
        auto matrix = generateEquivalenceRelation(n, n/2);
        auto props = checkProperties(matrix, n);
        assert(props.isEquivalence);
        std::cout << "等价关系测试通过 (n=" << n << ")\n";
    }

    // 测试特定性质关系生成器
    std::vector<std::string> types = {"reflexive", "symmetric", "transitive"};
    for (const auto& type : types) {
        auto matrix = generateSpecificRelation(5, type);
        auto props = checkProperties(matrix, 5);

        if (type == "reflexive") assert(props.isReflexive);
        if (type == "symmetric") assert(props.isSymmetric);
        if (type == "transitive") assert(props.isTransitive);

        std::cout << type << "关系测试通过\n";
    }

    std::cout << "所有一致性测试通过!\n";
}