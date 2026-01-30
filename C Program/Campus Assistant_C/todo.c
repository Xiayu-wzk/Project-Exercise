#include "function.h"

#define MAX_TASKS 100// 最多待办事项数

/*
void addTask();                      // 添加任务
void markTaskDone();                 // 标记任务完成
void viewAllTasks();                 // 查看所有任务
void deleteTask();                   // 删除任务
void saveTodoList();                 // 保存待办事项列表
void loadTodoList();                 // 加载待办事项列表
*/

typedef struct {
    char description[256];            // 任务描述
    int priority;                     // 1-高, 2-中, 3-低
    int dueDay, dueMonth, dueYear;    // 截止日期
    int isDone;                       // 0未完成 1已完成
} Task;

static Task g_tasks[MAX_TASKS];
static int g_taskCount = 0;

static const char *TODO_FILE = "Datafiles/todo_list.txt"; 

void todoMenu() {
    loadTodoList();
    int choice;
    do {
        printf("\n===== To-Do List =====\n");//待办事项清单
        printf("1. Add Task\n");//添加待办事项
        printf("2. Mark Task Done\n");//标记事项完成
        printf("3. View All Tasks\n");//查看所有事项
        printf("4. Delete Task\n");//删除事项
        printf("5. Save and Return to Main Menu\n");//保存并返回主菜单
        printf("Please enter an option: ");//请输入选项
        
    scanf("%d", &choice);
        switch (choice) {
            case 1:
                addTask();
                break;
            case 2:
                viewAllTasks();
                markTaskDone();
                break;
            case 3:
                viewAllTasks();
                break;
            case 4:
                viewAllTasks();
                deleteTask();
                break;
            case 5:
                saveTodoList();
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid option, please try again!\n");
        }
        
    } while (choice != 5);
}

void loadTodoList() {
    FILE *fp = fopen(TODO_FILE, "r");
    g_taskCount = 0;
    if (!fp) {
        printf("To-Do list file does not exist, treating as empty data.\n");
        return;
    }
    while (g_taskCount < MAX_TASKS) {
        Task t; memset(&t, 0, sizeof(t));
        if (fscanf(fp, "%d %d %d %d %d ", &t.priority, &t.dueYear, &t.dueMonth, &t.dueDay, &t.isDone) != 5) {
            break; 
        }
        if (fgets(t.description, sizeof(t.description), fp) == NULL) break;
        t.description[strcspn(t.description, "\n")] = 0;
        if (t.priority < 1 || t.priority > 3) continue;
        if (!is_valid_date(t.dueYear, t.dueMonth, t.dueDay)) continue;
        g_tasks[g_taskCount++] = t;
    }
    fclose(fp);
    printf("Loaded %d tasks.\n", g_taskCount);
}

void saveTodoList() {
    FILE *fp = fopen(TODO_FILE, "w");
    if (!fp) {
        printf("Save failed: Unable to open file %s\n", TODO_FILE);
        return;
    }
    for (int i = 0; i < g_taskCount; ++i) {
        fprintf(fp, "%d %d %d %d %d %s\n", g_tasks[i].priority, g_tasks[i].dueYear, g_tasks[i].dueMonth, g_tasks[i].dueDay, g_tasks[i].isDone, g_tasks[i].description);
    }
    fclose(fp);
    printf("To-Do list saved to %s\n", TODO_FILE);
}

void addTask() {
    if (g_taskCount >= MAX_TASKS) {
        printf("To-Do list is full, cannot add more tasks.\n");
        return;
    }
    Task t;
    printf("Please enter the task description (up to 255 characters): ");
    getchar(); 
    if (fgets(t.description, sizeof(t.description), stdin) == NULL) {
        printf("Input error.\n");
        return;
    }
    t.description[strcspn(t.description, "\n")] = 0; 

    printf("Please enter priority (1-High, 2-Medium, 3-Low): ");
    if (scanf("%d", &t.priority) != 1 || t.priority < 1 || t.priority > 3) {
        printf("Invalid input.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        return;
    }
    printf("Please enter due date (year month day), e.g., 2025 7 15: ");
    if (scanf("%d %d %d", &t.dueYear, &t.dueMonth, &t.dueDay) != 3 || !is_valid_date(t.dueYear, t.dueMonth, t.dueDay)) {
        printf("Invalid input.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        return;
    }
    t.isDone = 0; 
    g_tasks[g_taskCount++] = t;
    printf("Task added: %s (Priority:%d Due:%04d-%02d-%02d)\n", t.description, t.priority, t.dueYear, t.dueMonth, t.dueDay);
}

void markTaskDone() {
    if (g_taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    int index;
    printf("Please enter the task number to mark as done (1-%d): ", g_taskCount);
    if (scanf("%d", &index) != 1 || index < 1 || index > g_taskCount) {
        printf("Invalid input.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        return;
    }
    g_tasks[index - 1].isDone = 1;
    printf("Task \"%s\" marked as done.\n", g_tasks[index - 1].description);
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {} // 
}

void viewAllTasks() {
    if (g_taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\nTask list:\n");
    for (int i = 0; i < g_taskCount; ++i) {
        Task t = g_tasks[i];
    printf("%d. [%s] %s (Priority:%d Due:%04d-%02d-%02d)\n", i + 1, t.isDone ? "Done" : "Not Done", t.description, t.priority, t.dueYear, t.dueMonth, t.dueDay);
    }
}

void deleteTask() {
    if (g_taskCount == 0) {
        printf("No tasks available.\n");
        return;
    }
    int index;
    printf("Please enter the task number to delete (1-%d): ", g_taskCount);
    if (scanf("%d", &index) != 1 || index < 1 || index > g_taskCount) {
        printf("Invalid input.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
        return;
    }
    printf("Task \"%s\" deleted.\n", g_tasks[index - 1].description);
    for (int i = index - 1; i < g_taskCount - 1; ++i) {
        g_tasks[i] = g_tasks[i + 1];
    }
    g_taskCount--;
    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
}
