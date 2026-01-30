#include "function.h"

/*
void addTransaction();                // 添加交易
void viewTransactions();              // 查看交易
void viewCategoryStats();             // 查看分类统计
void searchByDate();                  // 按日期搜索
void deleteTransaction();             // 删除交易
void generateMonthlyReport();         // 生成月度报告
void saveLedger();                    // 保存账本
void loadLedger();                    // 加载账本
*/

typedef struct {
	int day, month, year;
	float amount;
	char category[30];
	char note[100];
	int isIncome; // 0: 支出, 1: 收入
} Bill;

typedef struct BillNode {
	Bill data;
	struct BillNode *next;
} BillNode;

static BillNode *g_head = NULL;
static int g_billCount = 0;
static const char *LEDGER_FILE = "Datafiles/ledger.txt";

static void free_list(void) {
	BillNode *cur = g_head;
	while (cur) {
		BillNode *next = cur->next;
		free(cur);
		cur = next;
	}
	g_head = NULL;
	g_billCount = 0;
}

void ledgerMenu() {
	loadLedger();
	int choice;
	do {
		printf("\n===== Daily Expense Ledger =====\n");//日常支出记账本
		printf("1. Add Transaction\n");// 添加账单
		printf("2. View Transactions\n");// 查看账单
		printf("3. View Category Stats\n");// 查看分类统计
		printf("4. Search Transactions\n");// 搜索账单记录
		printf("5. Delete Transaction\n");// 删除账单
		printf("6. Generate Monthly Report\n");// 生成月度报告
		printf("7. Save and Return to Main Menu\n");// 保存并返回主菜单
		printf("Please enter an option: ");// 请输入选项：

		scanf("%d", &choice);
		switch (choice) {
			case 1:
				addTransaction();
				break;
			case 2:
				viewTransactions();
				break;
			case 3:
				viewCategoryStats();
				break;
			case 4:
				searchByDate();
				break;
			case 5:
				deleteTransaction();
				break;
			case 6:
				generateMonthlyReport();
				break;
			case 7:
				saveLedger();
				printf("Returning to main menu...\n");// 返回主菜单...
				break;
			default:
				printf("Invalid option, please try again!\n");// 选项无效，请重试！
		}
		
	} while (choice != 7);

	free_list();
}

static int append_bill(const Bill *entry) {
	BillNode *node = (BillNode *)malloc(sizeof(BillNode));
	if (!node) {
		printf("Out of memory, cannot add record.\n");// 内存不足，无法添加记录。
		return 0;
	}
	node->data = *entry;
	node->next = NULL;

	if (!g_head) {
		g_head = node;
	} else {
		BillNode *tail = g_head;
		while (tail->next) {
			tail = tail->next;
		}
		tail->next = node;
	}
	g_billCount++;
	return 1;
}

static void escape_note_csv(const char *src, char *dst, size_t dstSize) {
	size_t j = 0;
	if (dstSize == 0) return;
	if (j < dstSize - 1) dst[j++] = '"';
	for (size_t i = 0; src[i] != '\0'; ++i) {
		if (src[i] == '"') {
			if (j < dstSize - 1) dst[j++] = '"';
			if (j < dstSize - 1) dst[j++] = '"';
		} else {
			if (j < dstSize - 1) dst[j++] = src[i];
		}
	}
	if (j < dstSize - 1) dst[j++] = '"';
	dst[j < dstSize ? j : dstSize - 1] = '\0';
}

static int unescape_note_csv(const char *srcQuoted, char *dst, size_t dstSize) {
	size_t len = strlen(srcQuoted);
	if (len < 2 || srcQuoted[0] != '"' || srcQuoted[len - 1] != '"') return 0;
	size_t j = 0;
	for (size_t i = 1; i < len - 1; ++i) {
		char c = srcQuoted[i];
		if (c == '"' && i + 1 < len - 1 && srcQuoted[i + 1] == '"') {
			if (j < dstSize - 1) dst[j++] = '"';
			i++;
		} else {
			if (j < dstSize - 1) dst[j++] = c;
		}
	}
	dst[j] = '\0';
	return 1;
}

void loadLedger() {
	FILE *fp = fopen(LEDGER_FILE, "r");
	free_list();
	if (!fp) {
		printf("Ledger file does not exist, treating as empty data.\n");// 账本文件不存在，按空数据处理。
		return;
	}

	char line[512];
	while (fgets(line, sizeof(line), fp)) {
		size_t len = strlen(line);
		while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) line[--len] = '\0';
		if (len == 0) continue;
		if (len > 0 && line[len - 1] == ',') line[--len] = '\0';

		char *cursor = line;
		char *fields[7];
		int fidx = 0;
		while (fidx < 7 && *cursor) {
			if (fidx == 5) {
				if (*cursor != '"') { fidx = -1; break; }
				char *start = cursor;
				cursor++;
				int closed = 0;
				while (*cursor) {
					if (*cursor == '"') {
						if (*(cursor + 1) == '"') {
							cursor += 2;
							continue;
						} else {
							cursor++;
							closed = 1;
							break;
						}
					}
					cursor++;
				}
				if (!closed) { fidx = -1; break; }
				if (*cursor == ',') {
					*cursor = '\0';
					cursor++;
				}
				fields[fidx++] = start;
			} else {
				char *start = cursor;
				char *c = strchr(cursor, ',');
				if (c) {
					*c = '\0';
					cursor = c + 1;
				} else {
					cursor = start + strlen(start);
				}
				fields[fidx++] = start;
			}
		}
		if (fidx != 7) continue;

		Bill t; memset(&t, 0, sizeof(t));
		t.year = atoi(fields[0]);
		t.month = atoi(fields[1]);
		t.day = atoi(fields[2]);
		t.amount = (float)atof(fields[3]);
		strncpy(t.category, fields[4], sizeof(t.category) - 1);
		t.category[sizeof(t.category) - 1] = '\0';
		if (!unescape_note_csv(fields[5], t.note, sizeof(t.note))) continue;
		t.isIncome = atoi(fields[6]);

		if (!is_valid_date(t.year, t.month, t.day) || t.amount < 0 || (t.isIncome != 0 && t.isIncome != 1)) {
			continue;
		}
		if (!append_bill(&t)) {
			break;
		}
	}
	fclose(fp);
	printf("Loaded %d transaction records.\n", g_billCount);// 已加载 %d 条交易记录。
}

void saveLedger() {
	FILE *fp = fopen(LEDGER_FILE, "w");
	if (!fp) {
		printf("Save failed: Unable to open file %s\n", LEDGER_FILE);// 保存失败：无法打开文件 %s
		return;
	}
	char escaped[256];
	for (BillNode *cur = g_head; cur; cur = cur->next) {
		escape_note_csv(cur->data.note, escaped, sizeof(escaped));
		fprintf(fp, "%d,%d,%d,%.2f,%s,%s,%d\n",
				cur->data.year, cur->data.month, cur->data.day,
				cur->data.amount, cur->data.category, escaped, cur->data.isIncome);
	}
	fclose(fp);
	printf("Data saved to %s\n", LEDGER_FILE);// 数据已保存到 %s
}

void addTransaction() {
	Bill t;
	printf("Please enter the date (year month day), e.g., 2025 7 15: ");// 请输入日期（年 月 日），例如：2025 7 15：
	if (scanf("%d %d %d", &t.year, &t.month, &t.day) != 3) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (!is_valid_date(t.year, t.month, t.day)) {
		printf("Invalid date.\n");// 日期不合法。
		return;
	}
	printf("Please enter the amount (positive number): ");// 请输入金额（正数）：
	if (scanf("%f", &t.amount) != 1 || t.amount <= 0) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	printf("Please enter the category (up to 29 characters): ");// 请输入分类（不超过29字符）：
	if (scanf("%29s", t.category) != 1) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	printf("Please enter a note (up to 99 characters): ");// 请输入备注（不超过99字符）：
	getchar();
	if (fgets(t.note, sizeof(t.note), stdin) == NULL) {
		printf("Input error.\n");// 输入错误。
		return;
	}
	t.note[strcspn(t.note, "\n")] = 0;

	printf("Is this income or expense? (1-Income, 0-Expense): ");// 这是收入还是支出？(1-收入, 0-支出)：
	if (scanf("%d", &t.isIncome) != 1 || (t.isIncome != 0 && t.isIncome != 1)) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (!append_bill(&t)) return;

	printf("Added: %04d-%02d-%02d %s %.2f (%s) Note:%s\n",
		   t.year, t.month, t.day, t.isIncome ? "Income" : "Expense", t.amount, t.category, t.note);// 已添加：%04d-%02d-%02d %s %.2f (%s) 备注:%s
}

void viewTransactions() {
	if (g_billCount == 0) {
		printf("No transaction records.\n");// 暂无交易记录。
		return;
	}
	printf("\nDate          Type   Amount      Category         Note\n");// 日期          类型   金额      分类         备注
	printf("-----------------------------------------------------\n");
	for (BillNode *cur = g_head; cur; cur = cur->next) {
		Bill t = cur->data;
		printf("%04d-%02d-%02d  %s  %.2f  %-10s  %s\n", t.year, t.month, t.day, t.isIncome ? "Income" : "Expense", t.amount, t.category, t.note);// %04d-%02d-%02d  %s  %.2f  %-10s  %s
	}
}

void viewCategoryStats() {
	if (g_billCount == 0) {
		printf("No transaction records.\n");// 暂无交易记录。
		return;
	}
	typedef struct {
		char category[30];
		float income;
		float expense;
	} CatStat;
	CatStat stats[100];
	int statCount = 0;

	for (BillNode *cur = g_head; cur; cur = cur->next) {
		Bill t = cur->data;
		int found = 0;
		for (int j = 0; j < statCount; ++j) {
			if (strcmp(stats[j].category, t.category) == 0) {
				if (t.isIncome) {
					stats[j].income += t.amount;
				} else {
					stats[j].expense += t.amount;
				}
				found = 1;
				break;
			}
		}
		if (!found && statCount < 100) {
			strcpy(stats[statCount].category, t.category);
			stats[statCount].income = t.isIncome ? t.amount : 0.0f;
			stats[statCount].expense = t.isIncome ? 0.0f : t.amount;
			statCount++;
		}
	}

	printf("\nCategory Statistics:\n");// 分类统计：
	printf("Category     Income    Expense\n");// 分类         收入      支出
	printf("-------------------------------\n");
	for (int i = 0; i < statCount; ++i) {
		printf("%-10s  %.2f  %.2f\n", stats[i].category, stats[i].income, stats[i].expense);
	}
}

void searchByDate() {
	if (g_billCount == 0) {
		printf("No transaction records.\n");// 暂无交易记录。
		return;
	}
	int y, m, d;
	printf("Enter date to search (year month day): ");// 查询日期（年 月 日）：
	if (scanf("%d %d %d", &y, &m, &d) != 3) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (!is_valid_date(y, m, d)) {
		printf("Invalid date.\n");// 日期不合法。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	printf("\nDate          Type   Amount      Category         Note\n");// 日期          类型   金额      分类         备注
	printf("-----------------------------------------------------\n");
	int found = 0;
	for (BillNode *cur = g_head; cur; cur = cur->next) {
		Bill t = cur->data;
		if (t.year == y && t.month == m && t.day == d) {
			printf("%04d-%02d-%02d  %s  %.2f  %-10s  %s\n", t.year, t.month, t.day, t.isIncome ? "Income" : "Expense", t.amount, t.category, t.note);// %04d-%02d-%02d  %s  %.2f  %-10s  %s
			found = 1;
		}
	}
	if (!found) {
		printf("No transactions found for this date.\n");// 该日期无交易记录。
	}
}

void deleteTransaction() {
	if (g_billCount == 0) {
		printf("No transaction records.\n");// 暂无交易记录。
		return;
	}
	int index;
	printf("Enter the transaction number to delete (1-%d): ", g_billCount);// 请输入要删除的交易编号（1-%d）：
	if (scanf("%d", &index) != 1 || index < 1 || index > g_billCount) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}

	int curIdx = 1;
	BillNode **pp = &g_head;
	while (*pp && curIdx < index) {
		pp = &((*pp)->next);
		curIdx++;
	}
	if (*pp) {
		BillNode *del = *pp;
		*pp = del->next;
		printf("Transaction \"%04d-%02d-%02d %s %.2f (%s) Note:%s\" deleted.\n",
			   del->data.year, del->data.month, del->data.day,
			   del->data.isIncome ? "Income" : "Expense", del->data.amount,
			   del->data.category, del->data.note);
		free(del);
		g_billCount--;
	}
}

void generateMonthlyReport() {
	if (g_billCount == 0) {
		printf("No transaction records.\n");// 暂无交易记录。
		return;
	}
	int year, month;
	printf("Enter year and month (e.g., 2025 7): ");// 请输入年份和月份（例如 2025 7）：
	if (scanf("%d %d", &year, &month) != 2 || month < 1 || month > 12) {
		printf("Invalid input.\n");// 输入无效。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	if (year < 1900 || year > 2100) {
		printf("Year out of allowed range.\n");// 年份超出允许范围。
		int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
		return;
	}
	float totalIncome = 0.0f, totalExpense = 0.0f;
	for (BillNode *cur = g_head; cur; cur = cur->next) {
		Bill t = cur->data;
		if (t.year == year && t.month == month) {
			if (t.isIncome) {
				totalIncome += t.amount;
			} else {
				totalExpense += t.amount;
			}
		}
	}
	printf("\n%d-%02d Income: %.2f Expense: %.2f Balance: %.2f\n", year, month, totalIncome, totalExpense, totalIncome - totalExpense);
}
