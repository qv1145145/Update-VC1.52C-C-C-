// sort.h - STL风格排序算法实现 for VC1.52C
#ifndef SORT_H
#define SORT_H

#include <stdlib.h> // 用于 size_t

// 定义比较函数类型
typedef int (__cdecl *CompareFunc)(const void*, const void*);

// 默认比较函数（从小到大）
int __cdecl default_compare(const void* a, const void* b);

// 字节交换函数
static void swap_bytes(void __near* a, void __near* b, size_t size) {
    char __near* pa = (char __near*)a;
    char __near* pb = (char __near*)b;
    char temp;
    
    for (size_t i = 0; i < size; i++) {
        temp = pa[i];
        pa[i] = pb[i];
        pb[i] = temp;
    }
}

// 快速排序分区函数
static size_t partition(void __near* base, size_t num, size_t size, CompareFunc compare) {
    char __near* array = (char __near*)base;
    size_t pivot_index = num / 2; // 选择中间元素作为枢轴
    size_t i = 0;
    size_t j = num - 1;
    
    // 将枢轴移动到末尾
    swap_bytes(array + pivot_index * size, array + (num - 1) * size, size);
    
    while (1) {
        // 从左向右找到第一个大于等于枢轴的元素
        while (i < j && compare(array + i * size, array + (num - 1) * size) < 0) {
            i++;
        }
        
        // 从右向左找到第一个小于等于枢轴的元素
        while (i < j && compare(array + j * size, array + (num - 1) * size) >= 0) {
            j--;
        }
        
        // 如果指针相遇，退出循环
        if (i >= j) {
            break;
        }
        
        // 交换元素
        swap_bytes(array + i * size, array + j * size, size);
    }
    
    // 将枢轴放回正确位置
    swap_bytes(array + i * size, array + (num - 1) * size, size);
    
    return i;
}

// 快速排序主函数
static void quick_sort_impl(void __near* base, size_t num, size_t size, CompareFunc compare) {
    if (num <= 1) {
        return;
    }
    
    size_t pivot_index = partition(base, num, size, compare);
    
    // 递归排序左半部分
    if (pivot_index > 0) {
        quick_sort_impl(base, pivot_index, size, compare);
    }
    
    // 递归排序右半部分
    if (pivot_index + 1 < num) {
        quick_sort_impl((char __near*)base + (pivot_index + 1) * size, 
                       num - pivot_index - 1, size, compare);
    }
}

// STL风格排序函数 - 带比较函数
void __cdecl sort(void __near* begin, void __near* end, size_t size, CompareFunc compare) {
    char __near* begin_ptr = (char __near*)begin;
    char __near* end_ptr = (char __near*)end;
    
    size_t num = (end_ptr - begin_ptr) / size;
    if (num <= 1) return;
    
    quick_sort_impl(begin, num, size, compare);
}

// STL风格排序函数 - 不带比较函数（使用默认比较）
void __cdecl sort_default(void __near* begin, void __near* end, size_t size) {
    sort(begin, end, size, default_compare);
}

// 默认比较函数（从小到大）
int __cdecl default_compare(const void* a, const void* b) {
    // 由于不知道具体类型，这里假设是整数比较
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// 类型特定的比较函数和排序函数

// 整数比较函数
int __cdecl compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// 整数排序函数
void __cdecl sort_int(int __near* begin, int __near* end, CompareFunc compare) {
    if (compare == NULL) {
        compare = compare_int;
    }
    sort(begin, end, sizeof(int), compare);
}

// 浮点数比较函数
int __cdecl compare_double(const void* a, const void* b) {
    double double_a = *(const double*)a;
    double double_b = *(const double*)b;
    
    if (double_a < double_b) return -1;
    if (double_a > double_b) return 1;
    return 0;
}

// 浮点数排序函数
void __cdecl sort_double(double __near* begin, double __near* end, CompareFunc compare) {
    if (compare == NULL) {
        compare = compare_double;
    }
    sort(begin, end, sizeof(double), compare);
}

// 字符串比较函数
int __cdecl compare_string(const void* a, const void* b) {
    const char __near* str_a = *(const char __near* __near*)a;
    const char __near* str_b = *(const char __near* __near*)b;
    
    // 简单的字符串比较实现
    while (*str_a && *str_b && *str_a == *str_b) {
        str_a++;
        str_b++;
    }
    
    return (*str_a > *str_b) - (*str_a < *str_b);
}

// 字符串排序函数
void __cdecl sort_string(const char __near* __near* begin, const char __near* __near* end, CompareFunc compare) {
    if (compare == NULL) {
        compare = compare_string;
    }
    sort((void __near*)begin, (void __near*)end, sizeof(const char __near*), compare);
}

#endif // SORT_H