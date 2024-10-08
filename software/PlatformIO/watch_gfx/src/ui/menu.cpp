/*** 
 * @Author: MakerM0
 * @Date: 2024-09-08 17:27:33
 * @LastEditTime: 2024-10-04 00:27:13
 * @FilePath: \jdi_watch_gfx\src\ui\menu.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */

#include "menu.h"
#include "fonts/fonts.h"
#include "driver/drv_sensor.h"

Node *curNode; // 当前节点
// 创建新节点
Node *createNode(const char *name)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        // printf("内存分配失败\n");
        return NULL;
    }
    strncpy(newNode->name, name, MAX_NAME_LENGTH - 1);
    newNode->name[MAX_NAME_LENGTH - 1] = '\0';
    newNode->parent = NULL;
    newNode->child = NULL;
    newNode->prev = NULL;
    newNode->next = NULL;
    newNode->data = NULL;
    newNode->type = T_NULL;
    return newNode;
}

void addPage(Node *node, Page *page)
{
    node->data = page;
    node->type = T_PAGE;
}

void addValInt(Node *node, int *data)
{
    node->data = data;
    node->type = T_INT;
}

void addValFloat(Node *node, float *data)
{
    node->data = data;
    node->type = T_FLOAT;
}

void addValBool(Node *node, bool *data)
{
    node->data = data;
    node->type = T_BOOL;
}

// 添加子节点
void addChild(Node *parent, Node *child)
{
    if (parent->child == NULL)
    {
        parent->child = child;
    }
    else
    {
        Node *sibling = parent->child;
        while (sibling->next != NULL)
        {
            sibling = sibling->next;
        }
        sibling->next = child;
        child->prev = sibling;
    }
    child->parent = parent;
}

// 在指定节点后插入新节点
void insertAfter(Node *node, Node *newNode)
{
    newNode->next = node->next;
    newNode->prev = node;
    if (node->next != NULL)
    {
        node->next->prev = newNode;
    }
    node->next = newNode;

    // 如果插入的是父节点的第一个子节点
    if (node->parent != NULL && node->parent->child == node)
    {
        node->parent->child = newNode;
    }
    newNode->parent = node->parent;
}

// 删除指定节点
void deleteNode(Node *node)
{
    if (node->parent != NULL && node->parent->child == node)
    {
        node->parent->child = node->next;
    }
    if (node->prev != NULL)
    {
        node->prev->next = node->next;
    }
    if (node->next != NULL)
    {
        node->next->prev = node->prev;
    }
    free(node);
}

// 显示菜单
void displayMenu(Node *root, int level)
{
    if (root == NULL)
        return;

    for (int i = 0; i < level; i++)
    {
        DISP("  ");
    }
    DISP("- %s\n", root->name);

    Node *child = root->child;
    while (child != NULL)
    {
        displayMenu(child, level + 1);
        child = child->next;
    }
}

// 释放内存
void freeMenu(Node *root)
{
    if (root == NULL)
        return;

    Node *child = root->child;
    while (child != NULL)
    {
        Node *next = child->next;
        freeMenu(child);
        child = next;
    }

    free(root);
}

// test
int menu_test()
{
    disp.fillScreen(COLOR_BLACK);
    disp.setFont();
    disp.setTextSize(1);
    disp.setCursor(0, 0);

    // 创建根节点
    Node *root = createNode("Main");

    // 创建一级菜单项
    Node *file = createNode("File");
    Node *edit = createNode("Edit");
    Node *view = createNode("View");

    // 添加一级菜单项到根节点
    addChild(root, file);
    addChild(root, edit);
    addChild(root, view);

    // 创建文件菜单的子项
    Node *newFile = createNode("New");
    Node *openFile = createNode("Open");
    Node *saveFile = createNode("Save");

    // 添加文件菜单的子项
    addChild(file, newFile);
    addChild(file, openFile);
    addChild(file, saveFile);

    // 创建编辑菜单的子项
    Node *cut = createNode("Cut");
    Node *copy = createNode("Copy");
    Node *paste = createNode("Paste");

    // 添加编辑菜单的子项
    addChild(edit, cut);
    addChild(edit, copy);
    addChild(edit, paste);

    // 在"复制"后插入新节点
    Node *find = createNode("Find");
    insertAfter(copy, find);

    DISP("MENU Stucture\n");
    displayMenu(root, 0);

    // 释放内存
    freeMenu(root);
    disp.refresh();

    return 0;
}

uint8_t menu_getChildNum(Node *node)
{
    uint8_t num = 0;
    Node *child = node->child;
    while (child != NULL)
    {
        child = child->next;
        num++;
    }

    return num;
}

// void menu_render(Node *node, uint8_t selectedIndex)
// {
//     int16_t x1;
//     int16_t y1;
//     uint16_t w;
//     uint16_t h;

//     uint8_t index = 0;
//     if (node->child == NULL)
//         return;

//     Node *child = node->child;

//     disp.fillScreen(COLOR_BLACK);
//     disp.setFont(&FreeMonoBold9pt7b);
//     disp.setTextSize(1);

//     disp.getTextBounds("#", 0, 0, &x1, &y1, &w, &h);
//     uint16_t height_selected = h + 10;

//     uint8_t DISPLAY_ITEMS = DISP_HEIGHT / height_selected;

//     uint8_t TOTAL_ITEMS =  menu_getChildNum(node);

//     while (child != NULL)
//     {
//         if (selectedIndex < DISPLAY_ITEMS)
//         {
//             if (selectedIndex == index)
//             {
//                 disp.setTextColor(COLOR_BLACK);
//                 disp.fillRect(0, index * (height_selected), DISP_WIDTH, height_selected, COLOR_GREEN);
//             }
//             else
//             {
//                 disp.setTextColor(COLOR_WHITE);
//             }

//             disp.setCursor(2 - x1, 5 + index * (height_selected)-y1);
//             disp.print(child->name);
//         }
//         else
//         {
//             if (selectedIndex == index)
//             {
//                 disp.setTextColor(COLOR_BLACK);
//                 disp.fillRect(0, DISPLAY_ITEMS * (height_selected), DISP_WIDTH, height_selected, COLOR_GREEN);
//             }
//             else
//             {
//                 disp.setTextColor(COLOR_WHITE);
//             }
//             disp.setCursor(2 - x1, 5 + index%DISPLAY_ITEMS * (height_selected)-y1);
//             disp.print(child->name);

//         }
//         index++;
//         child = child->next;
//     }

//     disp.setTextColor(COLOR_RED,COLOR_WHITE);
//     disp.setCursor(40 - x1, 0-y1);
//     disp.printf("%d",TOTAL_ITEMS);

//     disp.refresh();
// }

Node *menu_render(Node *node, int startIndex, int selectedIndex)
{
    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    Node *selectedNode = NULL;

    int index = 0;
    if (node->child == NULL)
        return NULL;

    Node *child = node->child;

    disp.fillScreen(COLOR_BLACK);

    // title
    disp.setFont();
    disp.setTextSize(1);
    disp.setTextColor(COLOR_GREEN);
    disp.setCursor(0, 0);
    disp.printf("%s", node->name);

    disp.setFont(&FreeMonoBold9pt7b);
    disp.setTextSize(1);
    disp.setTextWrap(false);

    disp.getTextBounds("#", 0, 0, &x1, &y1, &w, &h);
    uint16_t height_selected = h + 10;

    int DISPLAY_ITEMS = DISP_HEIGHT / height_selected;
    int START_Y = DISP_HEIGHT - (DISPLAY_ITEMS * height_selected);

    int TOTAL_ITEMS = menu_getChildNum(node);
    int i = 0;
    while (child != NULL)
    {
        if (index >= startIndex && index < (startIndex + DISPLAY_ITEMS))
        {
            int currentIndex = startIndex + i;

            if (currentIndex >= TOTAL_ITEMS)
                break;
            if (currentIndex == selectedIndex)
            {
                disp.getTextBounds(child->name, 0, 0, &x1, &y1, &w, &h);

                disp.setTextColor(COLOR_BLACK);
                disp.fillRect(0, START_Y + i * (height_selected),  DISP_WIDTH, height_selected, COLOR_GREEN);
                // disp.fillRoundRect(0, START_Y + i * (height_selected), w<DISP_WIDTH?(w+4):DISP_WIDTH, height_selected, 3,COLOR_GREEN);


                

                if (w > DISP_WIDTH - 2)
                {
                    static int16_t iii = 0;
                    static int direction = 0;

                    disp.setCursor(2 - x1 - iii, START_Y + 5 + i * (height_selected)-y1);
                    if (direction == 0)
                    {
                        iii++;
                        if (iii > w - DISP_WIDTH + 10)
                        {

                            direction = 1;
                        }
                    }
                    else if (direction == 1)
                    {
                        iii--;
                        if (iii < 0 - 10)
                        {
                            direction = 0;
                        }
                    }
                }
                else
                {
                    disp.setCursor(2 - x1, START_Y + 5 + i * (height_selected)-y1);
                }

                disp.print(child->name);
                selectedNode = child;
            }
            else
            {
                disp.setTextColor(COLOR_WHITE);
                disp.setCursor(2 - x1, START_Y + 5 + i * (height_selected)-y1);
                disp.print(child->name);
            }

            i++;
        }

        index++;
        child = child->next;
    }

    disp.setFont();
    disp.setTextColor(COLOR_GREEN);

    char s[10];
    snprintf(s, 10, "%d/%d", selectedIndex + 1, TOTAL_ITEMS);
    disp.getTextBounds(s, 0, 0, &x1, &y1, &w, &h);
    disp.fillRect(DISP_WIDTH - w, 0, w, h, COLOR_BLACK);
    disp.setCursor(DISP_WIDTH - w, 0);
    disp.printf("%s", s);

    disp.refresh();
    delay(20);
    return selectedNode;
}

void menu_render_test()
{
    // 创建根节点
    Node *root = createNode("Main");

    // 创建一级菜单项
    Node *file = createNode("File");
    Node *edit = createNode("Edit");
    Node *view = createNode("View");

    // 添加一级菜单项到根节点
    addChild(root, file);
    addChild(root, edit);
    addChild(root, view);

    // 创建文件菜单的子项
    Node *newFile = createNode("New");
    Node *openFile = createNode("Open");
    Node *saveFile = createNode("Save");
    Node *sasveFile = createNode("exit");
    Node *sa2veFile = createNode("1");
    Node *s3aveFile = createNode("2");
    Node *s4aveFile = createNode("3");
    Node *s5aveFile = createNode("4");

    // 添加文件菜单的子项
    addChild(file, newFile);
    addChild(file, openFile);
    addChild(file, saveFile);
    addChild(file, sasveFile);
    addChild(file, sa2veFile);
    addChild(file, s3aveFile);
    addChild(file, s4aveFile);
    addChild(file, s5aveFile);

    // 创建编辑菜单的子项
    Node *cut = createNode("Cut");
    Node *copy = createNode("Copy");
    Node *paste = createNode("Paste");

    // 添加编辑菜单的子项
    addChild(edit, cut);
    addChild(edit, copy);
    addChild(edit, paste);

    // 在"复制"后插入新节点
    Node *find = createNode("Find");
    insertAfter(copy, find);

    // menu_render(root, 0);

    delay(1000);
    uint8_t TOTAL_ITEMS = menu_getChildNum(root->child);

    int selectedIndex = 0;
    int startIndex = 0;
    int direction = 1;

    while (1)
    {
    }
    while (1)
    {
        delay(1000);
        menu_render(file, startIndex, selectedIndex);
        if (direction == 1)
        {

            if (selectedIndex < TOTAL_ITEMS - 1)
            {
                selectedIndex++;
                if (TOTAL_ITEMS > 6)
                {
                    if (selectedIndex >= startIndex + 6)
                    {
                        startIndex = selectedIndex - 6 + 1;
                        direction = -1;
                    }
                }
                else
                {
                    if (selectedIndex == TOTAL_ITEMS - 1)
                    {
                        direction = -1;
                    }
                }
            }
        }
        else if (direction == -1)
        {
            if (selectedIndex > 0)
            {
                selectedIndex--;
                if (TOTAL_ITEMS > 6)
                {
                    if (selectedIndex < startIndex)
                    {
                        startIndex = selectedIndex;
                        direction = 1;
                    }
                }
                else
                {
                    if (selectedIndex == 0)
                    {
                        direction = 1;
                    }
                }
            }

            /* code */
        }
    }
}

void page_loop()
{
    if (curNode->data != NULL)
    {
        Page *x = (Page *)(curNode->data);
        // (Page *)(curNode->data)->loop(NULL);
        x->loop(NULL);
    }
}
void page_enter()
{
    if (curNode->data != NULL)
    {
        Page *x = (Page *)(curNode->data);
        x->enter(NULL);
    }
}
void page_exit()
{
    if (curNode->data != NULL)
    {
        Page *x = (Page *)(curNode->data);
        x->exit(NULL);
        curNode = curNode->parent; // 更新node
    }
}

action input_action = ACTION_NONE;
int selectedIndex = 0;
int startIndex = 0;
uint8_t TOTAL_ITEMS;

#define DISPLAY_ITEMS 6 // 一页最多显示的菜单项

void menu_setup()
{
    startIndex = 0;
    selectedIndex = 0;
    TOTAL_ITEMS = menu_getChildNum(curNode);
}

void EnterSetValPage()
{
    disp.fillScreen(COLOR_BLUE);

    disp.fillRect(5, 10, DISP_WIDTH - (5 * 2), 20, COLOR_GREEN);
    disp.fillRect(5, 30, DISP_HEIGHT - (5 * 2), 30, COLOR_WHITE);
    disp.drawRect(5, 30, DISP_HEIGHT - (5 * 2), 30, COLOR_GREEN);

    disp.setTextWrap(false);

    disp.setFont();
    disp.setTextSize(2);
    disp.setTextColor(COLOR_BLACK);
    disp.setCursor(5 + 2, 10);
    disp.printf(curNode->name);

    disp.setTextSize(2);
    disp.setTextColor(COLOR_BLACK, COLOR_GREEN);
    disp.setCursor(0, 50);
    disp.printf("%d\r\n", sizeof(int));
    disp.printf("%d\r\n", sizeof(float));
    disp.printf("%d\r\n", sizeof(bool));
    disp.printf("%d\r\n", sizeof(Page));

    // disp.setTextColor(COLOR_BLACK);
    // disp.setCursor(5 + 10, 40);
    // disp.print(curNode->data);
    disp.refresh();
}

void drawPara()
{
    disp.setTextColor(COLOR_BLACK, COLOR_GREEN);
    disp.setCursor(5 + 10, 40);
    switch (curNode->type)
    {
    case T_BOOL:
        disp.printf("%3s", *(bool *)(curNode->data) ? "ON" : "OFF");
        break;
    case T_INT:
        disp.printf("%5d", *(int *)(curNode->data));
        break;
    case T_FLOAT:
        disp.printf("%.3f", *(float *)(curNode->data));
        break;

    default:
        break;
    }

    disp.refresh();
}

__WEAK action getAction()
{
    return ACTION_NONE;
}

void menu_loop()
{
    // page_enter();
    if (curNode->child == NULL)
    {
        if (curNode->type == T_PAGE)
        {
            page_loop();
        }
        else if (curNode->type == T_BOOL)
        {
            bool *x = (bool *)(curNode->data);
            switch (getAction())
            {
            case ACTION_UP:

                *(bool *)(curNode->data) = !(*x);
                drawPara();
                break;
            case ACTION_DOWN:
                *(bool *)(curNode->data) = !*(x);
                drawPara();
                break;

            case ACTION_ENTER:
                break;
            default:
                break;
            }
        }
        else if (curNode->type == T_INT)
        {
            // bool *x = (int *)(curNode->data);
            switch (getAction())
            {
            case ACTION_UP:

                *(int *)(curNode->data)-=1;
                drawPara();
                break;
            case ACTION_DOWN:
                *(int *)(curNode->data)+=1;
                drawPara();
                break;

            case ACTION_ENTER:
            curNode = curNode->parent;
                break;
            default:
                break;
            }
        }
        // page_exit();
    }
    else
    {

        Node *selectedNode = menu_render(curNode, startIndex, selectedIndex);

        // 根据按键或加速度计数据进行动作
        switch (getAction())
        {
        case ACTION_UP:
            if (selectedIndex > 0)
            {
                selectedIndex--;
                if (selectedIndex < startIndex)
                {
                    startIndex = selectedIndex;
                }
            }

            break;
        case ACTION_DOWN:
            if (selectedIndex < TOTAL_ITEMS - 1)
            {
                selectedIndex++;
                if (selectedIndex >= startIndex + DISPLAY_ITEMS)
                {
                    startIndex = selectedIndex - DISPLAY_ITEMS + 1;
                }
            }
            break;

        case ACTION_ENTER:
            if (selectedNode->child != NULL)
            {
                curNode = selectedNode;
                startIndex = 0;
                selectedIndex = 0;
                TOTAL_ITEMS = menu_getChildNum(selectedNode);
            }
            else if (selectedNode->data != NULL)
            {
                if (selectedNode->type == T_PAGE)
                {
                    curNode = selectedNode;
                    // disp.fillScreen(COLOR_BLACK);
                    // disp.refresh();
                    // delay(1000);
                    page_enter();
                }
                else
                {
                    curNode = selectedNode;
                    EnterSetValPage();
                }
            }
            else if (selectedNode->child == NULL)
            {
                if (strcmp(selectedNode->name, "Exit") == 0)
                {

                    curNode = curNode->parent; // 更新node
                    TOTAL_ITEMS = menu_getChildNum(curNode);
                }
            }

            break;
        default:
            break;
        }
    }
}

#define ACC_THRESHOLD_X 0.3
#define ACC_THRESHOLD_Y 0.5
static bool acc_key_valid = false;

// action getAccAction()
// {
//     action ac = ACTION_NONE;
//     sensor.readAcceleration();
//     if (acc_key_valid == false)
//     {
//         if (sensor.readY() > ACC_THRESHOLD)
//         {
//             ac = ACTION_UP;
//         }
//         else if (sensor.readY() < -ACC_THRESHOLD)
//         {
//             ac = ACTION_DOWN;
//         }
//         if (sensor.readX() > ACC_THRESHOLD)
//         {
//             ac = ACTION_LEFT;
//         }
//         else if (sensor.readX() < -ACC_THRESHOLD)
//         {
//             ac = ACTION_RIGHT;
//         }
//         if (ac != ACTION_NONE)
//             acc_key_valid = true;
//     }
//     else
//     {
//         if (sensor.readY() < ACC_THRESHOLD && sensor.readY() > -ACC_THRESHOLD)
//         {
//             acc_key_valid = false;
//             ac = ACTION_NONE;
//         }
//     }
//     return ac;
// }

action getAccAction()
{
    action ac = ACTION_NONE;
    sensor.readAcceleration();

    {
        if (sensor.readY() > ACC_THRESHOLD_Y)
        {
            ac = ACTION_UP;
        }
        else if (sensor.readY() < -ACC_THRESHOLD_Y)
        {
            ac = ACTION_DOWN;
        }
        if (sensor.readX() > ACC_THRESHOLD_X)
        {
            ac = ACTION_LEFT;
        }
        else if (sensor.readX() < -ACC_THRESHOLD_X)
        {
            ac = ACTION_RIGHT;
        }
        if (ac != ACTION_NONE)
            acc_key_valid = true;
    }
  
    return ac;
}