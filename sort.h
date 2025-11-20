// sort.h - STL风格排序算法 for VC1.52C
#ifndef SORT_H
#define SORT_H

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

// 比较函数类型定义
typedef int (*compare_func_t)(const void*, const void*);

// ==================== 比较函数对象（类似STL） ====================

// 升序比较（类似std::less）
int less(const void* a, const void* b) {
    // 默认使用memcmp进行字节比较，适用于基本类型
    return memcmp(a, b, sizeof(int));
}

// 降序比较（类似std::greater）  
int greater(const void* a, const void* b) {
    return -less(a, b);
}

// 整数升序
int less_int(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

// 整数降序
int greater_int(const void* a, const void* b) {
    return -less_int(a, b);
}

// 双精度升序
int less_double(const void* a, const void* b) {
    double da = *(const double*)a;
    double db = *(const double*)b;
    return (da < db) ? -1 : (da > db) ? 1 : 0;
}

// 双精度降序
int greater_double(const void* a, const void* b) {
    return -less_double(a, b);
}

// 字符串升序（字典序）
int less_string(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

// 字符串降序（逆字典序）
int greater_string(const void* a, const void* b) {
    return -less_string(a, b);
}

// ==================== 内部实现函数 ====================

// 插入排序（用于小数组优化）
static void insertion_sort(char* base, size_t num, size_t size, compare_func_t comp) {
    char* end = base + num * size;
    char* i, *j;
    size_t k;
    char key[64];
    
    for (i = base + size; i < end; i += size) {
        j = i - size;
        
        // 复制当前元素到key
        for (k = 0; k < size; k++) {
            key[k] = *((char*)i + k);
        }
        
        // 移动比key大的元素
        while (j >= base && comp(j, key) > 0) {
            for (k = 0; k < size; k++) {
                *((char*)j + size + k) = *((char*)j + k);
            }
            j -= size;
        }
        
        // 插入key到正确位置
        for (k = 0; k < size; k++) {
            *((char*)j + size + k) = key[k];
        }
    }
}

// 三数取中法选择枢轴
static char* median_of_three(char* a, char* b, char* c, compare_func_t comp) {
    if (comp(a, b) < 0) {
        if (comp(b, c) < 0) return b;
        if (comp(a, c) < 0) return c;
        return a;
    } else {
        if (comp(a, c) < 0) return a;
        if (comp(b, c) < 0) return c;
        return b;
    }
}

// 快速排序分区函数
static char* partition(char* low, char* high, size_t size, compare_func_t comp) {
    // 选择枢轴（三数取中）
    char* mid = low + ((high - low) / size / 2) * size;
    char* pivot_ptr = median_of_three(low, mid, high - size, comp);
    
    // 将枢轴移动到末尾
    char pivot[64];
    size_t i;
    char* j;
    
    for (i = 0; i < size; i++) {
        pivot[i] = *((char*)pivot_ptr + i);
        *((char*)pivot_ptr + i) = *((char*)(high - size) + i);
        *((char*)(high - size) + i) = pivot[i];
    }
    
    char* current = low;
    for (j = low; j < high - size; j += size) {
        if (comp(j, pivot) <= 0) {
            // 交换元素
            for (i = 0; i < size; i++) {
                char temp = *((char*)current + i);
                *((char*)current + i) = *((char*)j + i);
                *((char*)j + i) = temp;
            }
            current += size;
        }
    }
    
    // 将枢轴放回正确位置
    for (i = 0; i < size; i++) {
        char temp = *((char*)current + i);
        *((char*)current + i) = *((char*)(high - size) + i);
        *((char*)(high - size) + i) = temp;
    }
    
    return current;
}

// 优化的快速排序主函数
static void quick_sort(char* base, size_t num, size_t size, compare_func_t comp) {
    // 小数组使用插入排序
    if (num < 16) {
        insertion_sort(base, num, size, comp);
        return;
    }
    
    char* low = base;
    char* high = base + num * size;
    
    // 使用栈模拟递归
    struct Stack {
        char* low;
        char* high;
    };
    
    static Stack stack[64];
    int top = -1;
    
    stack[++top].low = low;
    stack[top].high = high;
    
    while (top >= 0) {
        low = stack[top].low;
        high = stack[top].high;
        top--;
        
        if (high - low <= 16 * (ptrdiff_t)size) {
            insertion_sort(low, (high - low) / size, size, comp);
            continue;
        }
        
        char* pivot = partition(low, high, size, comp);
        size_t left_size = (pivot - low) / size;
        size_t right_size = (high - pivot - size) / size;
        
        if (left_size < right_size) {
            if (left_size > 1) {
                top++;
                stack[top].low = low;
                stack[top].high = pivot;
            }
            if (right_size > 1) {
                top++;
                stack[top].low = pivot + size;
                stack[top].high = high;
            }
        } else {
            if (right_size > 1) {
                top++;
                stack[top].low = pivot + size;
                stack[top].high = high;
            }
            if (left_size > 1) {
                top++;
                stack[top].low = low;
                stack[top].high = pivot;
            }
        }
    }
}

// ==================== 主排序函数 ====================

// STL风格排序函数 - 主要接口
void sort(void* begin, void* end, size_t size, compare_func_t comp) {
    if (begin == end || size == 0) return;
    
    char* begin_ptr = (char*)begin;
    char* end_ptr = (char*)end;
    size_t num = (end_ptr - begin_ptr) / size;
    
    quick_sort(begin_ptr, num, size, comp);
}

// 默认排序函数 - 使用less作为默认比较函数
void sort(void* begin, void* end, size_t size) {
    sort(begin, end, size, less);
}

// ==================== 类型特定的便捷函数（可选） ====================

// 整数数组排序
void sort(int* begin, int* end, compare_func_t comp) {
    if (comp == NULL) comp = less_int;
    sort(begin, end, sizeof(int), comp);
}

void sort(int* begin, int* end) {
    sort(begin, end, less_int);
}

// 双精度数组排序
void sort(double* begin, double* end, compare_func_t comp) {
    if (comp == NULL) comp = less_double;
    sort(begin, end, sizeof(double), comp);
}

void sort(double* begin, double* end) {
    sort(begin, end, less_double);
}

// 字符串数组排序
void sort(const char** begin, const char** end, compare_func_t comp) {
    if (comp == NULL) comp = less_string;
    sort((void*)begin, (void*)end, sizeof(const char*), comp);
}

void sort(const char** begin, const char** end) {
    sort(begin, end, less_string);
}

#endif // SORT_H