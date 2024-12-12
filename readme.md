# 等价关系处理系统 (Equivalence Relation Processor)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-blue)]()
[![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)]()

欢迎使用等价关系处理系统！这是一个专为BJTU离散数学课程设计的学习辅助工具。在学习离散数学的过程中，理解数学概念和掌握核心算法才是最重要的，而不是被程序结构的设计与填充，亦或是测试用例的生成和验证所困扰。因此，我开发了这个工具，希望能帮助同学们将更多精力投入到数学本质与关键算法中。

## 🎯 项目亮点

这个项目为同学们提供了一个即开即用的程序，主要特点包括：

- 完成了基本要求，还加入了许多可供参考的扩展功能和性能优化
- 预编译的 Windows 可执行文件（.exe），双击即可运行，无需安装任何依赖
- 内置的等价关系测试用例生成器，一键生成符合数学定义的测试数据
- 清晰简洁的命令行交互界面，操作直观友好
- 严格的数学性质验证机制，保证生成数据的正确性

## 💻 快速开始

1. 访问 [Release页面](https://github.com/SuXinxian/BJTU-DiscreteMath-Homework-EquivalenceProcessor/releases)
2. 下载最新版本的可执行文件
3. 双击运行，开始你的离散数学探索之旅！

## 🛠️ 技术特点

### 优化的数据结构
- 使用`std::bitset`优化的矩阵表示，显著降低存储开销
- 实现了带路径压缩和按秩合并的并查集（Union-Find），优化等价类计算
- 采用动态规划算法高效求解等价关系计数问题

### 核心算法
- Warshall算法用于计算传递闭包
- 并查集算法用于等价类划分
- 贝尔数计算的动态规划优化


## 📝 学术诚信

在课程作业中使用本工具时，请遵循学术诚信原则，合理引用。建议在报告中采用如下引用格式：

"本报告中的测试用例由北京交通大学离散数学课程开源项目'等价关系处理系统'（项目地址：https://github.com/SuXinxian/BJTU-DiscreteMath-Homework-EquivalenceProcessor）生成。该工具提供了符合数学定义的等价关系测试用例生成功能。"


## 📋 开源协议

本项目采用 MIT 许可证。这意味着你可以自由地使用、修改和分发这个项目，但需要保留原始的版权声明。

---

让我们一起开启令人兴奋的离散数学学习之旅！如果你对这个项目有任何问题或建议，欢迎在 GitHub 上提出 Issue。

> 温馨提示：本工具仅作为学习辅助，请结合教材和课堂内容深入理解数学概念，并自己撰写核心代码。