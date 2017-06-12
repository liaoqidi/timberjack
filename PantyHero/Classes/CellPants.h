//
//  CellPants.h
//  PantsHero
//
//  Created by lj on 14-3-7.
//
//

#ifndef __PantsHero__CellPants__
#define __PantsHero__CellPants__

#include <iostream>
#include "cocos2d.h"
#include "CocosGUI.h"
#include "CocoStudio.h"
using namespace cocos2d::gui;
using namespace cocostudio;
using namespace cocos2d;

class CellPants : public Widget
{
public:
    CellPants(){};
    ~CellPants(){};
    
    bool init();
    void setting(int i);
    CREATE_FUNC(CellPants);
};
#endif /* defined(__PantsHero__CellPants__) */
