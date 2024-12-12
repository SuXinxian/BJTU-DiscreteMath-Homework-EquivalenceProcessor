//
// Created by Chypre on 12/8/2024.
//

/**
 * @file main.cpp
 * @brief 等价关系处理系统的主程序
 * @details 提供用户交互界面，整合关系处理器和测试生成器的功能
 */

#include <iostream>
#include <string>
#include <limits>
#include "relation_processor.h"
#include "test_generator.h"

//---------------------- 用户界面辅助函数 ----------------------

void clearInputBuffer() {
    // 清除输入缓冲区，确保后续输入正常
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void pauseExecution() {
    // 在程序继续前暂停，让用户有时间查看输出
    std::cout << "\n按回车键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void displayTitle() {
    std::cout << "\n╔════════════════════════════╗\n"
              << "║     等价关系处理系统           ║\n"
              << "╚════════════════════════════╝\n";
}

void displayMenu() {
    std::cout << "\n┌────────────────────────┐\n"
              << "│  1. 处理关系             │\n"
              << "│  2. 生成测试用例          │\n"
              << "│  3. 退出                │\n"
              << "└────────────────────────┘\n"
              << "请选择功能 (1-3): ";
}

//---------------------- 核心功能实现 ----------------------

void processRelation() {
    // 处理单个关系的主要逻辑
    try {
        // 获取并验证集合大小
        std::cout << "\n请输入集合的大小 [1-" << MAX_N << "]: ";
        int n;
        std::cin >> n;

        if (std::cin.fail() || n <= 0 || n > MAX_N) {
            clearInputBuffer();
            throw std::invalid_argument("无效的集合大小！必须是1到" + std::to_string(MAX_N) + "之间的整数。");
        }

        // 创建并使用关系处理器
        RelationProcessor processor(n);
        processor.readRelation();

        // 分析并显示结果
        std::cout << "\n━━━━━ 分析结果 ━━━━━\n\n";
        std::cout << "原始关系矩阵:\n";
        processor.printMatrix();

        // 判断是否为等价关系并显示相应信息
        bool isEquiv = processor.isEquivalence();
        std::cout << "\n这是" << (isEquiv ? "" : "不") << "等价关系\n";

        if (isEquiv) {
            // 如果是等价关系，显示等价类划分
            std::cout << "\n等价类划分:\n";
            auto partition = processor.getPartition();
            for (const auto& component : partition) {
                std::cout << "{";
                for (size_t i = 0; i < component.size(); i++) {
                    std::cout << component[i] << (i < component.size() - 1 ? "," : "");
                }
                std::cout << "} ";
            }
            std::cout << "\n";
        } else {
            // 如果不是等价关系，计算并显示最小等价关系
            std::cout << "\n计算包含原关系的最小等价关系:\n";
            processor.computeMinimalEquivalence();
            processor.printMatrix();
        }

        // 显示等价关系的总数
        std::cout << "\n" << n << "元集合上等价关系的个数: "
                 << processor.countEquivalenceRelations(n) << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n⚠ 错误: " << e.what() << "\n";
    }
}

void generateTests() {
    // 生成测试用例的功能实现
    try {
        RelationGenerator generator;
        generator.generateTestSuite("test_cases.txt");
        generator.runConsistencyTests();
        std::cout << "✓ 测试用例生成完成，已保存到 test_cases.txt\n";
    }
    catch (const std::exception& e) {
        std::cerr << "\n⚠ 生成测试用例时发生错误: " << e.what() << "\n";
    }
}

//---------------------- 主函数 ----------------------

int main() {
    try {
        // 设置UTF-8编码
        #ifdef _WIN32
        system("chcp 65001");
        #endif

        // 主程序循环
        while (true) {
            displayTitle();
            displayMenu();

            // 获取并处理用户选择
            int choice;
            std::cin >> choice;

            if (std::cin.fail()) {
                clearInputBuffer();
                std::cout << "⚠ 无效的输入！请输入1-3之间的数字。\n";
                continue;
            }

            clearInputBuffer();

            switch (choice) {
                case 1:
                    processRelation();
                    break;
                case 2:
                    generateTests();
                    break;
                case 3:
                    std::cout << "\n━━━━━ 程序结束 ━━━━━\n"
                             << "感谢使用！再见！\n";
                    return 0;
                default:
                    std::cout << "⚠ 无效的选择！请输入1-3之间的数字。\n";
            }

            pauseExecution();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\n⚠ 程序发生错误: " << e.what() << "\n";
        pauseExecution();
        return 1;
    }

    return 0;
}