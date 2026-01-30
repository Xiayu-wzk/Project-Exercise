#include "function.h"

void Menu() {
    printf("\n===== Campus Assistant =====\n");    //校园生活助手
    printf("1. Study Time Management\n");    //学习时间管理
    printf("2. Daily Expense Ledger\n");    //日常支出记账本
    printf("3. To-Do List\n");    //待办事项清单
    printf("4. Exit\n");    //退出系统
    printf("=======================\n");
    printf("Please enter an option: ");    // 请输入选项:
}

int main() {
    
    int choice;
    do {
        Menu();
        scanf("%d", &choice);
        getchar(); // 清除换行符

        switch (choice) {
            case 1:
                studyLogMenu();
                break;
            case 2:
                ledgerMenu();
                break;
            case 3:
                todoMenu();
                break;
            case 4:
                printf("Thank you for using, goodbye!\n");   // 感谢使用，再见！
                break;
            default:
                printf("Invalid option, please try again!\n");    // 选项无效，请重试！
        }
        Sleep(1000); // 暂停1秒
        system("cls"); // 清屏
    } while (choice != 4);

    getchar();// 防止程序运行结束后闪退
    return 0;
}
