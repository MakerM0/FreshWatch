/*** 
 * @Author: MakerM0
 * @Date: 2024-09-11 17:44:24
 * @LastEditTime: 2024-10-03 19:50:20
 * @FilePath: \jdi_watch_gfx\src\ui\ui.h
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#pragma once
#ifndef __UI_H
#define __UI_H

#include "ui/menu.h"
#include "pages/lucky/lucky.h"
#include "pages/home/home.h"
#include "pages/poweroff/poweroff.h"
#include "pages/settings/settings.h"
#include "pages/enterUF2/enterUF2.h"
#include "pages/reset/reset.h"
#include "pages/pomodoro/pomodoro.h"



extern uint8_t TOTAL_ITEMS;
extern Node *curNode;

int hour;
bool flag;
void ui_init()
{

    Node *exit = createNode("Exit"); //  专用退出节点

    // 创建根节点
    Node *root = createNode("root");

    // 创建一级菜单项
    Node *home = createNode("Home");
    addPage(home, &page_home);
    Node *lucky = createNode("Lucky");
    addPage(lucky, &page_lucky);

        Node *pomodoro = createNode("Pomodoro");
    addPage(pomodoro, &page_pomodoro);


    Node *settings = createNode("Settings");
    addPage(settings, &page_settings);

    Node *power = createNode("power");


    Node *poweroff = createNode("PowerOff");
    addPage(poweroff, &page_poweroff);
    Node *enterUF2 = createNode("enterUF2");
    addPage(enterUF2, &page_enteruf2);
    Node *reset = createNode("Reset");
    addPage(reset, &page_reset);

    // 添加一级菜单项到根节点
    addChild(root, home);
    addChild(root, lucky);
    addChild(root, pomodoro);

    addChild(root, settings);
  
    addChild(root, power);

    addChild(power, exit);
    // addChild(power, poweroff);
    addChild(power, reset);
    addChild(power, enterUF2);





    // addValInt(TimeSetting,&hour);

    TOTAL_ITEMS = menu_getChildNum(root);

    curNode = home;
    // curNode = pomodoro;


}

#endif
