/*** 
 * @Author: MakerM0
 * @Date: 2024-09-10 11:04:02
 * @LastEditTime: 2024-10-03 20:27:07
 * @FilePath: \jdi_watch_gfx\src\ui\tool.h
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#pragma once
#ifndef __TOOL_H
#define __TOOL_H

// 使用 _Generic 关键字（C11及以后版本支持）：
#define type_name(x) _Generic((x),                                                 \
    int: "int",                                                                    \
    float: "float",                                                                \
    double: "double",                                                              \
    char: "char",                                                                  \
    default: "unknown")


// 获取数组内元素个数

#define ARRAY_SIZE(a) (sizeof((a))/sizeof((a)[0]))



#endif