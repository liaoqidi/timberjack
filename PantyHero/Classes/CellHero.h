//
//  CellHero.h
//  PantsHero
//
//  Created by lj on 14-3-7.
//
//

#ifndef __PantsHero__CellHero__
#define __PantsHero__CellHero__

#include <iostream>
#include "cocos2d.h"
#include "CocosGUI.h"
#include "CocoStudio.h"
using namespace cocos2d::gui;
using namespace cocostudio;
using namespace cocos2d;

class CellHero : public Widget
{
public:
    CellHero(){};
    ~CellHero(){};
    
    bool init();
    void setting(int i);
    CREATE_FUNC(CellHero);
};
#endif /* defined(__PantsHero__CellHero__) */
