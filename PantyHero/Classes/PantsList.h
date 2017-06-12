//
//  PantsList.h
//  PantsHero
//
//  Created by lyon on 14-3-9.
//
//

#ifndef __PantsHero__PantsList__
#define __PantsHero__PantsList__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class PantsList
{
public:
    PantsList(){};
    ~PantsList(){};
    
    bool init(const std::string& pszPlist);
};

#endif /* defined(__PantsHero__PantsList__) */
