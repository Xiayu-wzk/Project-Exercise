#include "function.h"

/*
void addLog();                         // 添加记录
void viewStats();                      // 查看统计
void searchLogs();                     // 搜索记录
void deleteLog();                      // 删除记录
void showTrend();                      // ASCII图
void saveToFile();                     // 写文件
void loadFromFile();                   // 读文件
*/

typedef struct {
	int day, month, year;		// 日期
	float hours;				// 学习时长
} StudyLog;

typedef struct LogNode {
	StudyLog data;				// 学习记录数据
	struct LogNode *next;		// 指向下一个节点
} LogNode;

static LogNode *g_head = NULL;
static int g_logCount = 0;
static const char *STUDY_LOG_FILE = "Datafiles/study_log.txt";

static void free_list(void) {
	LogNode *cur = g_head;
	while (cur) {
		LogNode *next = cur->next;
		free(cur);
		cur = next;
	}
	g_head = NULL;
	g_logCount = 0;
}

static int same_date(const StudyLog *a, const StudyLog *b) {
	return a->year == b->year && a->month == b->month && a->day == b->day;
}// 判断两个日期是否相同

void studyLogMenu() {
	loadFromFile();
	int choice;
	do {
		printf("\n===== Study Time Management =====\n");// 学习时间管理
		printf("1. Add Study Log\n");// 添加学习记录
		printf("2. View Statistics\n");// 查看统计
		printf("3. Search Study Logs\n");// 搜索学习记录
		printf("4. Delete Study Logs\n");// 删除学习记录
		printf("5. Show Study Trend\n");// 显示学习趋势图
		printf("6. Save and Return to Main Menu\n");// 保存并返回主菜单
		printf("=======================\n");
		printf("Please enter an option: ");// 请输入选项：

		scanf("%d", &choice);
		switch (choice) {
			case 1:
				addLog();
				break;
			case 2:
				viewStats();
				break;
			case 3:
				searchLogs();
				break;
			case 4:
				deleteLog();
				break;
			case 5:
				showTrend();
				break;
			case 6:
				saveToFile();
				printf("Returning to main menu...\n");// 返回主菜单...
				break;
			default:
				printf("Invalid option, please try again!\n");// 选项无效，请重试！
		}
		
	} while (choice != 6);

	free_list();
}

static int append_log(const StudyLog *entry) {
	LogNode *node = (LogNode *)malloc(sizeof(LogNode));
	if (!node) {
		printf("Out of memory, cannot add record.\n");// 内存不足，无法添加记录。
		return 0;
	}
	node->data = *entry;
	node->next = NULL;

	if (!g_head) {
		g_head = node;
	} else {
		LogNode *tail = g_head;
		while (tail->next) {
			tail = tail->next;
		}
		tail->next = node;
	}
	g_logCount++;
	return 1;
}

void loadFromFile() {
	FILE *fp = fopen(STUDY_LOG_FILE, "r");
	free_list();
	if (!fp) {
		printf("Study log file does not exist, treating as empty data.\n");// 学习记录文件不存在，按空数据处理。
		return;
	}

	while (1) {
		StudyLog t;
		if (fscanf(fp, "%d %d %d %f", &t.year, &t.month, &t.day, &t.hours) != 4) {
			break;
		}
		if (t.month < 1 || t.month > 12 || t.day < 1 || t.day > 31 || t.hours < 0) {
			continue;
		}
		if (!append_log(&t)) {
			break;
		}
	}
	fclose(fp);
	printf("Loaded %d study logs.\n", g_logCount);// 已加载 %d 条学习记录。
}

void saveToFile() {
	FILE *fp = fopen(STUDY_LOG_FILE, "w");
	if (!fp) {
		printf("Save failed: Unable to open file %s\n", STUDY_LOG_FILE);// 保存失败：无法打开文件 %s	
		return;
	}
	for (LogNode *cur = g_head; cur; cur = cur->next) {
		fprintf(fp, "%d %d %d %.2f\n", cur->data.year, cur->data.month, cur->data.day, cur->data.hours);
	}
	fclose(fp);
	printf("Data saved to %s\n", STUDY_LOG_FILE);// 数据已保存到 %s
}

void addLog() {
	StudyLog t;
	printf("Please enter the date (year month day), e.g., 2025 7 15: ");// 请输入日期（年 月 日），例如：2025 7 15：
	if (scanf("%d %d %d", &t.year, &t.month, &t.day) != 3) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (!is_valid_date(t.year, t.month, t.day)) {
		printf("Invalid date, please check leap year and days in month.\n");// 日期不合法，请检查闰年与月份天数。
		return;
	}

	printf("Please enter the study hours for the day (hours, can be decimal): ");// 请输入当天学习时长（小时，可为小数）：
	if (scanf("%f", &t.hours) != 1) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (t.hours < 0 || t.hours > 24) {
		printf("Invalid data, please try again.\n");// 数据不合法，请重试。
		return;
	}

	if (append_log(&t)) {
		printf("Added: %04d-%02d-%02d %.2f hours\n", t.year, t.month, t.day, t.hours);// 已添加：%04d-%02d-%02d %.2f 小时
	}
}

void viewStats() {
	if (g_logCount == 0) {
		printf("No study records available.\n");// 暂无学习记录。
		return;
	}

	int totalDays = 0;
	float totalHours = 0.0f;

	for (LogNode *cur = g_head; cur; cur = cur->next) {
		totalHours += cur->data.hours;
	}

	for (LogNode *cur = g_head; cur; cur = cur->next) {
		int isNewDate = 1;
		for (LogNode *pre = g_head; pre != cur; pre = pre->next) {
			if (same_date(&cur->data, &pre->data)) {
				isNewDate = 0;
				break;
			}
		}
		if (isNewDate) {
			totalDays++;
		}
	}

	printf("Total study days: %d days\n", totalDays);// 总学习天数：%d 天
	printf("Total records: %d entries\n", g_logCount);// 总记录数：%d 条
	printf("Total study hours: %.2f hours\n", totalHours);// 总学习时长：%.2f 小时
	printf("Average study hours per day: %.2f hours\n", totalHours / totalDays);// 平均每天学习时长：%.2f 小时
}

void searchLogs() {
	if (g_logCount == 0) {
		printf("No study records available.\n");// 暂无学习记录。
		return;
	}

	int y, m, d;
	printf("Enter the date to search (year month day): ");// 输入要搜索的日期（年 月 日）：
	if (scanf("%d %d %d", &y, &m, &d) != 3 || !is_valid_date(y, m, d)) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}

	float sum = 0.0f;
	for (LogNode *cur = g_head; cur; cur = cur->next) {
		if (cur->data.year == y && cur->data.month == m && cur->data.day == d) {
			sum += cur->data.hours;
		}
	}

	printf("%04d-%02d-%02d Total study hours: %.2f hours\n", y, m, d, sum);// %04d-%02d-%02d 总学习时长：%.2f 小时
}

void deleteLog() {
	if (g_logCount == 0) {
		printf("No study records available.\n");// 暂无学习记录。
		return;
	}

	int y, m, d;
	printf("Enter the date to delete (year month day): ");// 输入要删除的日期（年 月 日）：
	if (scanf("%d %d %d", &y, &m, &d) != 3 || !is_valid_date(y, m, d)) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}

	int found = 0;
	LogNode **pp = &g_head;
	while (*pp) {
		if ((*pp)->data.year == y && (*pp)->data.month == m && (*pp)->data.day == d) {
			LogNode *del = *pp;
			*pp = del->next;
			free(del);
			g_logCount--;
			found = 1;
		} else {
			pp = &((*pp)->next);
		}
	}

	if (found) {
		printf("Deleted all study records for %04d-%02d-%02d.\n", y, m, d);// 已删除 %04d-%02d-%02d 的所有学习记录。
	} else {
		printf("No study records found for %04d-%02d-%02d.\n", y, m, d);// 未找到 %04d-%02d-%02d 的学习记录。
	}
}

void showTrend() {
	if (g_logCount == 0) {
		printf("No study records available.\n");// 暂无学习记录。
		return;
	}

	int year;
	printf("Please enter the year to view (e.g., 2025):");// 请输入要查看的年份（例如 2025）：
	if (scanf("%d", &year) != 1) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}

	int monthHours[12] = {0};
	int hasData = 0;

	for (LogNode *cur = g_head; cur; cur = cur->next) {
		if (cur->data.year == year && cur->data.month >= 1 && cur->data.month <= 12) {
			monthHours[cur->data.month - 1] += (int)(cur->data.hours * 10);
			hasData = 1;
		}
	}

	if (!hasData) {
		printf("No study records available for %d.\n", year);// %d 年暂无学习记录。
		return;
	}

	printf("%d Year study trend (monthly total)\n", year);// %d 年学习趋势（月度总计）
	printf("Month Study hours\n");// 月份 学习时长
	for (int i = 0; i < 12; ++i) {
		printf("%2d month |", i + 1);
		int stars = monthHours[i] / 100;
		for (int j = 0; j < stars; ++j) {
			putchar('*');
		}
		printf(" (%.1f hours)\n", monthHours[i] / 10.0f);
	}
}
