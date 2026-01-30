#include "function.h" 

int is_leap_year(int year) {
    if (year % 400 == 0) return 1;
    if (year % 100 == 0) return 0;
    return year % 4 == 0;
}

int days_in_month(int year, int month) {
    if (month < 1 || month > 12) return 0;
    static const int mdays[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && is_leap_year(year)) return 29;
    return mdays[month - 1];
}

int is_valid_date(int year, int month, int day) {                               // 年 1900-2100, 月 1-12, 日 （合法范围）
    if (year < 1900 || year > 2100) return 0; // 简单范围限制
    int dim = days_in_month(year, month);
    if (dim == 0) return 0;
    if (day < 1 || day > dim) return 0;
    return 1;
}
