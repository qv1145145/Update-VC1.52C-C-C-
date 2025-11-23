/* stdbool.h - 最优化的布尔类型实现 */
#ifndef _STDBOOL_H
#define _STDBOOL_H

/* 使用 unsigned char（节省内存）*/
typedef unsigned char bool;
#define true ((unsigned char)1)
#define false ((unsigned char)0)

/* C99 标准要求定义此宏 */
#define __bool_true_false_are_defined 1

/* 调试时使用的字符串表示 */
#ifdef DEBUG
#define bool_str(b) ((b) ? "true" : "false")
#endif

#endif /* _STDBOOL_H */
