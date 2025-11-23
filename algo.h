// algo.h - 修复函数指针调用问题
#ifndef ALGO_H
#define ALGO_H

#include <stddef.h>
#include <string.h>
#include "sort.h"

// 比较函数类型定义 - 使用正确的调用约定
typedef int (__cdecl *compare_func_t)(const void*, const void*);

// ==================== 查找算法 ====================

// 线性查找
void __far *find(void __far *first, void __far *last, size_t element_size, 
                 const void *value, compare_func_t comp) {
    char __far *begin = (char __far *)first;
    char __far *end = (char __far *)last;
    
    char __far *ptr;
    for (ptr = begin; ptr < end; ptr += element_size) {
        if (comp((const void *)ptr, value) == 0) {  // 修复：添加const void*转换
            return ptr;
        }
    }
    return 0;
}

// 二分查找 (要求已排序)
void __far *binary_search(void __far *first, void __far *last, size_t element_size,
                         const void *value, compare_func_t comp) {
    char __far *begin = (char __far *)first;
    char __far *end = (char __far *)last;
    
    while (begin < end) {
        char __far *mid = begin + ((end - begin) / element_size / 2) * element_size;
        int result = comp((const void *)mid, value);  // 修复：添加const void*转换
        
        if (result == 0) {
            return mid;
        } else if (result < 0) {
            begin = mid + element_size;
        } else {
            end = mid;
        }
    }
    return 0;
}

// 修复其他函数中的类似问题...

// 最大值
bool max_element(void __far *first, void __far *last, size_t element_size, 
                compare_func_t comp, void __far *__far *result) {  // 修复：使用正确的指针类型
    if (!result) return false;
    
    char __far *begin = (char __far *)first;
    char __far *end = (char __far *)last;
    
    if (begin == end) {
        *result = 0;
        return false;
    }
    
    *result = begin;
    char __far *ptr;
    for (ptr = begin + element_size; ptr < end; ptr += element_size) {
        if (comp((const void *)ptr, (const void *)*result) > 0) {  // 修复
            *result = ptr;
        }
    }
    return true;
}

// 最小值
bool min_element(void __far *first, void __far *last, size_t element_size,
                compare_func_t comp, void __far *__far *result) {  // 修复
    if (!result) return false;
    
    char __far *begin = (char __far *)first;
    char __far *end = (char __far *)last;
    
    if (begin == end) {
        *result = 0;
        return false;
    }
    
    *result = begin;
    char __far *ptr;
    for (ptr = begin + element_size; ptr < end; ptr += element_size) {
        if (comp((const void *)ptr, (const void *)*result) < 0) {  // 修复
            *result = ptr;
        }
    }
    return true;
}

// 计数
size_t count(void __far *first, void __far *last, size_t element_size,
             const void *value, compare_func_t comp) {
    char __far *begin = (char __far *)first;
    char __far *end = (char __far *)last;
    
    size_t count = 0;
    char __far *ptr;
    for (ptr = begin; ptr < end; ptr += element_size) {
        if (comp((const void *)ptr, value) == 0) {  // 修复
            count++;
        }
    }
    return count;
}

#endif /* ALGO_H */