# C_Program_20250801_20250907
Programming assignments completed from the first semester of the freshman year to the summer of the sophomore year.

//来自大一下至大二上暑假所作程序设计作业

# Campus Assistant 🎓

一个用 **C语言** 编写的命令行工具，旨在帮助大学生更好地管理学习与生活。它整合了 **学习时间管理、日常记账本、待办事项管理** 三个核心模块，是一次面向实践的课程设计项目。

## ✨ 功能简介

* **学习时间管理**

  * 添加、查询、删除学习记录
  * 自动统计学习天数、总时长与平均时长
  * 生成按月累计的学习趋势图（ASCII 图表）

* **日常记账本**

  * 支持支出/收入记录
  * 分类统计与查询功能
  * 采用 CSV 格式存储，支持备注中空格和特殊字符

* **待办事项管理**

  * 添加、查看、删除任务
  * 设置任务优先级与完成状态
  * 数据持久化保存到文件

## 🛠 实现方法

* **模块化设计**

  * 各功能模块独立实现（`study_log.c`、`ledger.c`、`todo.c`），统一通过 `function.h` 暴露接口
  * 主程序 `main.c` 负责菜单导航与模块调度

* **数据结构**

  * 使用结构体 (`StudyLog`、`Bill`、`Task`) 管理不同类型的数据
  * 数组存储 + 文件持久化

* **文件操作**

  * 文本文件存储各模块数据（如 `study_log.txt`、`ledger.txt`、`todo.txt`）
  * 账本模块采用 **CSV 风格存储**，保证备注信息的完整性

* **辅助工具模块**

  * 独立实现 `date_utils.c`，处理闰年判断、月份天数计算与日期合法性校验

## 🚀 使用方式

1. 克隆本仓库：

   ```bash
   git clone https://github.com/your-username/Campus_Assistant.git
   cd Campus_Assistant
   ```

2. 编译：

   ```bash
   gcc main.c study_log.c ledger.c todo.c date_utils.c -o Campus_Assistant
   ```

3. 运行：

   ```bash
   ./Campus_Assistant
   ```
## 🔮 未来拓展方向

* **课程日程提醒**：新增课程表和日程管理模块，支持提醒功能。
* **数据可视化**：使用更直观的图形界面或图表展示学习与消费统计。
* **数据库支持**：引入 SQLite/MySQL 等数据库，实现更高效的数据存储和查询。
* **跨平台应用**：将命令行工具扩展为桌面端/移动端应用，提升交互体验。
* **网络化功能**：支持多人协作与数据共享，例如课程小组或校园社区。
