//
//  Hero.h
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#ifndef ________Hero__
#define ________Hero__

#include <iostream>
#include "cocos2d.h"
#include "Setting.h"

#define CREATE(__TYPE__) \
static __TYPE__* create(int val) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
pRet->Set_id(val); \
if (pRet && pRet->init()) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

using namespace cocos2d;

enum class HeroTag
{
    HERO_SUPER,
    HERO_NORMAL,
};

enum class FlyTag
{
    FLY_UP,
    FLY_DOWN,
    FLY_NORMAL,
};

class Hero : public Sprite
{
public:
    HeroTag hero_tag;
    FlyTag fly_tag;
    float rotation;
    
    FlyTag last_hit_state;
    float last_hit_rotation;
    float last_hit_speed;
    float last_hit_speed_tmp;
    bool is_time_back_hit_point;
    
    float out_rotation;
    cocos2d::Point hit_point;
    std::vector<float> touch_time;
    //hero's id for load which texture
    int hero_id;
    //allowed number of failures
    int time_capsule;
public:
    Hero();
    ~Hero();
    bool init();
    CREATE(Hero);
    
    void action_up();
    void action_down();
    void action_rotation_back();

private:
    void Set_id(int val);
};


#endif /* defined(________Hero__) */
