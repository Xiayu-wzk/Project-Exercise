#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <windows.h>


void studyLogMenu();

void addLog();                          // 添加记录
void viewStats();                       // 查看统计
void searchLogs();                      // 搜索记录
void deleteLog();                       // 删除记录
void showTrend();                       // ASCII图
void saveToFile();                      // 写文件
void loadFromFile();                    // 读文件


void ledgerMenu();

void addTransaction();                // 添加账单
void viewTransactions();              // 查看账单
void viewCategoryStats();            // 查看分类统计
void searchByDate();                 // 按日期搜索账单记录
void deleteTransaction();           // 删除账单
void generateMonthlyReport();       // 生成月度报告
void saveLedger();                   // 保存账本
void loadLedger();                   // 加载账本


void todoMenu();

void addTask();                      // 添加任务
void markTaskDone();                 // 标记任务完成
void viewAllTasks();                 // 查看所有任务
void deleteTask();                   // 删除任务
void saveTodoList();                 // 保存列表
void loadTodoList();                 // 加载列表


int is_leap_year(int year);                         // 判断闰年
int days_in_month(int year, int month);             // 返回指定年月的天数，非法月份返回0
int is_valid_date(int year, int month, int day);    // 校验日期合法性：year 1900-2100，month 1-12，day 合法范围
