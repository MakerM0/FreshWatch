/*** 
 * @Author: MakerM0
 * @Date: 2024-09-08 17:27:27
 * @LastEditTime: 2024-10-03 19:49:56
 * @FilePath: \jdi_watch_gfx\src\ui\menu.h
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */


#ifndef __MENU_H
#define __MENU_H

#include <Arduino.h>
#include "driver/drv_disp.h"
#include "page.h"
#include "tool.h"
#define DISP disp.printf


//参数类型
typedef enum Type
{
    T_NULL=0,
    T_INT,
    T_BOOL,
    T_FLOAT,
    T_PAGE,
    T_MAXNUM
}Type;


#define MAX_NAME_LENGTH 50

// 定义双向链表节点结构
typedef struct Node {
    char name[MAX_NAME_LENGTH];
    // struct Page* page;
    Type type;
    void *data;
    struct Node* parent;
    struct Node* child;
    struct Node* prev;
    struct Node* next;
} Node;

typedef enum action
{
    ACTION_NONE = 0,
    ACTION_UP = 1,
    ACTION_DOWN = 2,
    ACTION_LEFT = 3,
    ACTION_RIGHT = 4,
    ACTION_ENTER,
    ACTION_MAXNUM
} action;



typedef  union Param{
    int val;
    bool flag;
}Param;


int menu_test() ;

Node *createNode(const char *name);

// 添加子节点
void addChild(Node *parent, Node *child);

// 在指定节点后插入新节点
void insertAfter(Node *node, Node *newNode);

// 删除指定节点
void deleteNode(Node *node);

// 显示菜单
void displayMenu(Node *root, int level);

// 释放内存
void freeMenu(Node *root);
uint8_t menu_getChildNum(Node *node);


// void menu_render();
void menu_render_test();

Node *menu_render(Node *node, int startIndex, int selectedIndex);
 
void addPage(Node *node,Page *page);
void addValInt(Node *node, int *data);
void addValFloat(Node *node, float *data);
void addValBool(Node *node, bool *data);

void menu_setup();
void menu_loop();

void page_exit();



action getAccAction();

#endif
