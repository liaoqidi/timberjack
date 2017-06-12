//
//  Hero.cpp
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#include "Hero.h"

Hero::Hero(){};

Hero::~Hero(){};

bool Hero::init()
{
    bool ret=false;
    do
    {
        CC_BREAK_IF(!Sprite::initWithSpriteFrameName("hero_001_01.png"));
        {//init animation
            Animation *anim=Animation::create();
            for(int i=1;i<4;i++)
            {
                const char *img=String::createWithFormat("hero_%03d_%02d.png",hero_id,i)->getCString();
                SpriteFrame *frame=SpriteFrameCache::getInstance()->getSpriteFrameByName(img);
                anim->addSpriteFrame(frame);
            }
            anim->setRestoreOriginalFrame(true);
            anim->setDelayPerUnit(0.5f/3);
            anim->setLoops(-1);
            
            auto action_fly=Animate::create(anim);
            this->runAction(action_fly);
        }
        hero_tag=HeroTag::HERO_NORMAL;
        fly_tag=FlyTag::FLY_NORMAL;
        rotation=0;
        time_capsule=3;
        ret=true;
    }while(0);
    return ret;
}

void Hero::action_up()
{
    if(this->rotation>-20)
    {
        this->rotation-=4;
        this->setRotation(this->rotation);
    }else
        out_rotation-=4;
}

void Hero::action_down()
{
    if(this->rotation<80)
    {
        this->rotation+=3;
        this->setRotation(this->rotation);
    }else
        out_rotation+=3;
}

void Hero::action_rotation_back()
{
    if(fly_tag==FlyTag::FLY_DOWN)
    {
        if(this->rotation>0)
        {
            this->rotation-=3;
            this->setRotation(this->rotation);
        }
    }else if(fly_tag==FlyTag::FLY_UP)
    {
        if(this->rotation<0)
        {
            this->rotation+=4;
            this->setRotation(this->rotation);
        }
    }
}

void Hero::Set_id(int val)
{
    hero_id=val;
}