# Update-VC1.52C-C-C-
Think Microsoft Visual C++ 1.52c's C/C++ support is outdated? Why not give this a try!
# sort.h 使用手册

## 头文件
使用 `sort.h` 排序库需要包含头文件：
```cpp
#include "sort.h"
```

## sort() 基本使用方法

`sort()` 函数可以对给定区间所有元素进行排序。它有三个参数 `sort(begin, end, cmp)`，其中：
- `begin`：指向待排序数组的第一个元素的指针
- `end`：指向待排序数组的最后一个元素的下一个位置的指针  
- `cmp`：排序准则函数（可选参数）

如果省略 `cmp` 参数，默认按升序进行排序。

## 支持的数据类型

### 整数数组排序
```cpp
int arr[] = {5, 2, 8, 1, 9};
int n = sizeof(arr) / sizeof(arr[0]);

// 默认升序
sort(arr, arr + n);

// 降序排序
sort(arr, arr + n, greater_int);
```

### 浮点数数组排序
```cpp
double values[] = {3.14, 2.71, 1.41};
sort(values, values + 3);                    // 升序
sort(values, values + 3, greater_double);    // 降序
```

### 字符串数组排序
```cpp
const char* names[] = {"Charlie", "Alice", "Bob"};
sort(names, names + 3);                      // 字典序升序
sort(names, names + 3, greater_string);      // 字典序降序
```

## 内置比较函数

我们提供了一系列内置比较函数，类似 STL 的 `less` 和 `greater`：

### 整数比较
- `less_int` - 升序排序
- `greater_int` - 降序排序

### 浮点数比较  
- `less_double` - 升序排序
- `greater_double` - 降序排序

### 字符串比较
- `less_string` - 字典序升序
- `greater_string` - 字典序降序

## 自定义比较函数

您可以定义自己的比较函数来实现特殊排序需求：

### 按绝对值排序
```cpp
int compare_abs(const void* a, const void* b) {
    int ia = abs(*(const int*)a);
    int ib = abs(*(const int*)b);
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

// 使用自定义比较函数
sort(arr, arr + n, compare_abs);
```

### 按字符串长度排序
```cpp
int compare_strlen(const void* a, const void* b) {
    int len1 = strlen(*(const char**)a);
    int len2 = strlen(*(const char**)b);
    return (len1 < len2) ? -1 : (len1 > len2) ? 1 : 0;
}

// 使用自定义比较函数  
sort(names, names + n, compare_strlen);
```

### 奇偶排序（奇数在前）
```cpp
int compare_odd_even(const void* a, const void* b) {
    int ia = *(const int*)a;
    int ib = *(const int*)b;
    int a_odd = ia % 2;
    int b_odd = ib % 2;
    
    if (a_odd != b_odd) {
        return (a_odd > b_odd) ? -1 : 1;
    }
    return (ia < ib) ? -1 : (ia > ib) ? 1 : 0;
}

sort(arr, arr + n, compare_odd_even);
```

## 通用排序函数

对于自定义结构体或其他数据类型，可以使用通用排序函数：

```cpp
// 需要显式指定元素大小
sort(begin, end, element_size, compare_function);

// 示例：自定义结构体排序
struct Point {
    int x, y;
};

int compare_point(const void* a, const void* b) {
    Point* p1 = (Point*)a;
    Point* p2 = (Point*)b;
    // 按x坐标排序
    return (p1->x < p2->x) ? -1 : (p1->x > p2->x) ? 1 : 0;
}

Point points[10];
sort(points, points + 10, sizeof(Point), compare_point);
```

## 完整示例

```cpp
#include <iostream.h>
#include "sort.h"

int main() {
    // 整数数组排序示例
    int numbers[] = {5, 2, 8, 1, 9, 3};
    int n = sizeof(numbers) / sizeof(numbers[0]);
    
    // 默认升序
    sort(numbers, numbers + n);
    
    // 降序
    sort(numbers, numbers + n, greater_int);
    
    return 0;
}
```

## 注意事项

1. **比较函数返回值**：
   - 返回 `-1` 表示 a < b
   - 返回 `0` 表示 a == b  
   - 返回 `1` 表示 a > b

2. **指针范围**：`end` 指针指向最后一个元素的下一个位置

3. **类型安全**：确保比较函数与数组类型匹配

这个排序库提供了与 C++ STL 类似的接口，同时针对 VC1.52C 环境进行了优化，支持高性能的快速排序算法。
