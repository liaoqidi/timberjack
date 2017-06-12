//
//  Obstacle.cpp
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#include "Obstacle.h"
#include "GameLayer.h"
#include "global.h"
Obstacle::Obstacle()
{
    m_fTime=0;
    combo_timer=0;
    limit_count=0;
    timer=TIMER_CREATE_NORMAL;
    spring_create=0;
};
Obstacle::~Obstacle(){};

void Obstacle::update_back(float lv)
{
    for(auto &obs_node : list)
    {
        float level=0;
        do
        {
            if(obs_node->rank_tag == ObstacleRankTag::TAG_NORMAL)
            {
                level=1;
                break;
            }
            
            if(obs_node->rank_tag == ObstacleRankTag::TAG_HARD)
            {
                level=1.5;
                break;
            }
            
            if(obs_node->rank_tag == ObstacleRankTag::TAG_MASTER)
            {
                level=2.5;
                break;
            }
        } while (0);
        
        if(obs_node->action_tag == ObstacleActionTag::TAG_MOVE)
        {
            float vertical_speed=0;
            if(obs_node->move_tag == ObstacleMoveTag::TAG_UP)
                vertical_speed=1;
            else if(obs_node->move_tag == ObstacleMoveTag::TAG_DOWN)
                vertical_speed=-1;
            
            vertical_speed*=level;
            obs_node->setPositionY(obs_node->getPositionY()-vertical_speed*RATIO_V);
        }
        
        obs_node->setPositionX(obs_node->getPositionX()+speed*lv*RATIO);
    }
    for(auto node : panty_list)
    {
        node->setPositionX(node->getPositionX()+speed*lv*RATIO);
    }
}

void Obstacle::update(int score)
{
    m_fTime++;
    if(m_fTime>=timer)
    {
        m_fTime=0;
        add_obstacle(score);
    }
    int i=0;
    for(auto &a : list)
    {
        if(a->getPositionX()< -(a->getChildByTag(1)->getBoundingBox().size.width))
        {
            a->removeFromParent();
            list.erase(i);
			break;	//  [3/25/2014 Fisho]
        }
		i++;
    }
    
    for(auto &obs_node : list)
    {
        if(obs_node->getPositionX()<=g_size.width/4)
        {
            if(obs_node->add_score)
            {
                obs_node->add_score=false;
                update_score();
            }
        }
        /* Test 2 */
        
        float level=0;
        do
        {
            if(obs_node->rank_tag == ObstacleRankTag::TAG_NORMAL)
            {
                level=1;
                break;
            }
            
            if(obs_node->rank_tag == ObstacleRankTag::TAG_HARD)
            {
                level=2;
                break;
            }
            
            if(obs_node->rank_tag == ObstacleRankTag::TAG_MASTER)
            {
                level=3;
                break;
            }
        } while (0);
        
        if(obs_node->action_tag == ObstacleActionTag::TAG_MOVE)
        {
            float vertical_speed=0;
            if(obs_node->move_tag == ObstacleMoveTag::TAG_UP)
                vertical_speed=1;
            else if(obs_node->move_tag == ObstacleMoveTag::TAG_DOWN)
                vertical_speed=-1;
            
            vertical_speed*=level;
            obs_node->setPositionY(obs_node->getPositionY()+vertical_speed*RATIO_V);
        }
        obs_node->setPositionX(obs_node->getPositionX()-speed*RATIO);
    }
    for(auto node : panty_list)
    {
        node->setPositionX(node->getPositionX()-speed*RATIO);
    }
}

void Obstacle::add_obstacle(int score)
{
    auto _obstacle=_Obstacle::create();
    _obstacle->add_score=true;
    
    //the lock of limit for move
    limit_lock=false;
    
    /* Test 2*/
    if(combo_timer>0)
        goto MARK_CREATE;
    
    switch_fall=false;
    switch_rise=false;
    last_PosY=0;
    
    //swhich of combos
    do
    {
        float pl=CCRANDOM_0_1();
        if(is_between(0, 8, score))
        {
            combo_tag=CreateComboTag::TAG_NORMAL;
            break;
        }
        
        if(is_between(8, 1000, score) && is_between(0.0f, 0.7f, pl))
        {
            combo_tag=CreateComboTag::TAG_RANDOM;
            break;
        }
        
        if(is_between(10, 1000, score) && is_between(0.7f, 1.0f, pl))
        {
            if(CCRANDOM_0_1()>0.5)
                combo_tag=CreateComboTag::TAG_LADDER_FALL;
            else
                combo_tag=CreateComboTag::TAG_LADDER_RISE;
            
            combo_timer=5;
            break;
        }
        
        /*if(is_between(0, 1000, score) && is_between(0.0, 1.0, pl))
        {
            combo_tag=CreateComboTag::TAG_SHAKE;
            break;
        }*/
        
        combo_tag=CreateComboTag::TAG_NORMAL;
        break;
        
    }while(0);
    
MARK_CREATE:
    if(combo_timer>0)
        combo_timer--;
    
    //swhich the rank
    if(is_between(0, 40, score))
        _obstacle->rank_tag=ObstacleRankTag::TAG_NORMAL;
    else if(is_between(40, 90, score))
        _obstacle->rank_tag=ObstacleRankTag::TAG_HARD;
    else
        _obstacle->rank_tag=ObstacleRankTag::TAG_MASTER;
    
    //get the combo setting
    combo_random(_obstacle);
    combo_shake(_obstacle);
    combo_limit(_obstacle);
    if(combo_tag == CreateComboTag::TAG_NORMAL)
    {
        _obstacle->action_tag=ObstacleActionTag::TAG_NORMAL;
    }
    
    //create
    auto obstacle_0=Sprite::createWithSpriteFrameName("scene_build_0.png");
    auto obstacle_1=Sprite::createWithSpriteFrameName("scene_build_0.png");
    
    obstacle_0->setAnchorPoint(cocos2d::Point(0.5,0));
    obstacle_1->setAnchorPoint(cocos2d::Point(0.5,1));
    
    if(CCRANDOM_0_1()>=0)
    {
        int i=0;
        float random=CCRANDOM_0_1();
        if(is_between(0.0, 0.25,random))
        {
            i=1;
        }
        if(is_between(0.25, 0.5,random))
        {
            i=2;
        }
        if(is_between(0.5, 0.75,random))
        {
            i=3;
        }
        if(is_between(0.75, 1.0,random))
        {
            i=4;
        }
        
        auto pants=Sprite::createWithSpriteFrameName(String::createWithFormat("panty_%02d.png",i)->getCString());
        pants->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
        //float offset=CCRANDOM_0_1()*400*RATIO-200*RATIO;
        pants->setPosition(0,g_size.height/2);
        _obstacle->addChild(pants,5);
        pants->setTag(3);
    }
    
    for(int i=0;i<1;i++)
    {
        if(CCRANDOM_0_1()>0.5)
        {
            int i=0;
            float random=CCRANDOM_0_1();
            if(is_between(0.0, 0.25,random))
            {
                i=1;
            }
            if(is_between(0.25, 0.5,random))
            {
                i=2;
            }
            if(is_between(0.5, 0.75,random))
            {
                i=3;
            }
            if(is_between(0.75, 1.0,random))
            {
                i=4;
            }
            
            auto panty=Sprite::createWithSpriteFrameName(String::createWithFormat("panty_%02d.png",i)->getCString());
            panty->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
            float offset=CCRANDOM_0_1()*(-600*RATIO)+CCRANDOM_0_1()*(600*RATIO);
            float _x=CCRANDOM_0_1()*300*RATIO+100*RATIO;
            panty->setPosition(g_size.width+obstacle_1->getContentSize().width*1.5+100*RATIO+_x,
                               g_size.height/2+offset);
            panty->setTag(3);
            panty_list.pushBack(panty);
            this->addChild(panty);
        }
    }
    
    _obstacle->addChild(obstacle_0);
    _obstacle->addChild(obstacle_1);
    
    obstacle_0->setTag(1);
    obstacle_1->setTag(2);
    
    int max=0;
    int min=0;
    float pl_y = 0;
    //set random range
    do
    {
        if(_obstacle->action_tag == ObstacleActionTag::TAG_MOVE)
        {
            if(_obstacle->move_tag == ObstacleMoveTag::TAG_UP)
            {
                max=0;
                min=-350*RATIO;
            }else if(_obstacle->move_tag == ObstacleMoveTag::TAG_DOWN)
            {
                max=600*RATIO;
                min=0;
            }
            pl_y=CCRANDOM_0_1()*(max-min)+min;
            
            obstacle_0->setPosition(0,g_size.height/2+(FIX_HEIGHT*RATIO/2));
            obstacle_1->setPosition(0,g_size.height/2-(FIX_HEIGHT*RATIO/2));
            
            break;
        }
        
        if(_obstacle->action_tag == ObstacleActionTag::TAG_NORMAL)
        {
            max=600*RATIO;
            min=-300*RATIO;
            pl_y=CCRANDOM_0_1()*(max-min)+min;
            
            obstacle_0->setPosition(0,g_size.height/2+(FIX_HEIGHT*RATIO/2));
            obstacle_1->setPosition(0,g_size.height/2-(FIX_HEIGHT*RATIO/2));
            
            break;
        }
        
        if(_obstacle->action_tag == ObstacleActionTag::TAG_LIMIT)
        {
            if(combo_tag == CreateComboTag::TAG_LADDER_FALL)
            {
                if(!switch_fall)
                {
                    last_PosY=600*RATIO;
                    switch_fall=true;
                }else
                    last_PosY-=100*RATIO;
            }else if(combo_tag == CreateComboTag::TAG_LADDER_RISE)
            {
                if(!switch_rise)
                {
                    last_PosY=-300*RATIO;
                    switch_rise=true;
                }else
                    last_PosY+=100*RATIO;
            }
            
            obstacle_0->setPosition(0,g_size.height/2+(FIX_HEIGHT*RATIO/2));
            obstacle_1->setPosition(0,g_size.height/2-(FIX_HEIGHT*RATIO/2));
            
            pl_y=last_PosY;
        }
        
        if(_obstacle->action_tag == ObstacleActionTag::TAG_FIX_MOVE)
        {
            if(_obstacle->move_tag == ObstacleMoveTag::TAG_UP)
            {
                max=0;
                min=-350*RATIO;
            }else if(_obstacle->move_tag == ObstacleMoveTag::TAG_DOWN)
            {
                max=600*RATIO;
                min=0;
            }
            
            obstacle_0->setPosition(0,g_size.height/2+(FIX_HEIGHT*RATIO/2));
            obstacle_1->setPosition(0,g_size.height/2-(FIX_HEIGHT*RATIO/2));
            
            pl_y=CCRANDOM_0_1()*(max-min)+min;
            break;
        }
        
        obstacle_0->setPosition(0,g_size.height/2+(FIX_HEIGHT*RATIO/2));
        obstacle_1->setPosition(0,g_size.height/2-(FIX_HEIGHT*RATIO/2));
        
    } while (0);
    
    _obstacle->setAnchorPoint(cocos2d::Point(0.5,0.5));
    _obstacle->setPositionY(pl_y);
    _obstacle->setPositionX(g_size.width+obstacle_1->getContentSize().width);
    this->addChild(_obstacle);
    if(_obstacle == NULL)
    {
        
    }
        
    list.pushBack(_obstacle);
}

bool Obstacle::is_between(int min, int max, int val)
{
    bool ret=true;
    if(val<min)
        ret=false;
    if(val>=max)
        ret=false;
    
    return ret;
}

bool Obstacle::is_between(float min, float max, float val)
{
    bool ret=true;
    if(val<min)
        ret=false;
    if(val>=max)
        ret=false;
    
    return ret;
}

void Obstacle::combo_limit(_Obstacle *tmp)
{
    if(combo_tag != CreateComboTag::TAG_LADDER_FALL || combo_tag != CreateComboTag::TAG_LADDER_RISE)
        return;
    tmp->action_tag=ObstacleActionTag::TAG_LIMIT;
}

void Obstacle::combo_random(_Obstacle *tmp)
{
    if(combo_tag != CreateComboTag::TAG_RANDOM)
        return;
    int pl=CCRANDOM_0_1()*100;
    do
    {
        if(is_between(0, 80, pl))
        {
            tmp->action_tag=ObstacleActionTag::TAG_MOVE;
            break;
        }
        
        if(is_between(80, 100, pl))
        {
            tmp->action_tag=ObstacleActionTag::TAG_NORMAL;
            break;
        }
    }while(0);
    
    if(tmp->action_tag == ObstacleActionTag::TAG_MOVE)
    {
        if(CCRANDOM_0_1()>0.5)
            tmp->move_tag=ObstacleMoveTag::TAG_UP;
        else
            tmp->move_tag=ObstacleMoveTag::TAG_DOWN;
    }

}

void Obstacle::combo_shake(_Obstacle *tmp)
{
    if(combo_tag != CreateComboTag::TAG_SHAKE)
        return;
    tmp->action_tag=ObstacleActionTag::TAG_FIX_MOVE;
    
    if(CCRANDOM_0_1()>0.5)
        tmp->move_tag=ObstacleMoveTag::TAG_UP;
    else
        tmp->move_tag=ObstacleMoveTag::TAG_DOWN;
}

ObstacleTag Obstacle::is_hit(cocos2d::Rect rect)
{
    ObstacleTag tag=ObstacleTag::NUL;
    
    for(auto obs : list)
    {
        float base_x=obs->getPositionX();
        float base_y=obs->getPositionY();
        cocos2d::Rect _1=obs->getChildByTag(1)->getBoundingBox();
        cocos2d::Rect _2=obs->getChildByTag(2)->getBoundingBox();
        if(obs->getChildByTag(3)!=NULL)
        {
            cocos2d::Rect _3=obs->getChildByTag(3)->getBoundingBox();
            _3.origin.y+=base_y;
            _3.origin.x+=base_x;
            if(rect.intersectsRect(_3))
            {
                if(obs->add_score)
                {
                    tag=ObstacleTag::PANTY;
                    obs->add_score=false;
                    update_score();
                }
                obs->getChildByTag(3)->setTag(5);
                obs->getChildByTag(5)->removeFromParent();
                return tag;
            }
        }
        _1.origin.x+=base_x;
        _2.origin.x+=base_x;
        _1.origin.y+=base_y;
        _2.origin.y+=base_y;
        
        if(rect.intersectsRect(_1) || rect.intersectsRect(_2))
        {
            tag=ObstacleTag::OBSTACLE;
            return tag;
        }
    }
    int i=0;
    for(auto a : panty_list)
    {
        if(rect.intersectsRect(a->getBoundingBox()))
        {
            a->removeFromParent();
            panty_list.erase(i);
            tag=ObstacleTag::PANTY;
            update_score();
            return tag;
        }
        i++;
    }
    
    return tag;
}

void Obstacle::del_obstacle()
{
    for(auto obs : list)
    {
        obs->removeFromParent();
    }
    list.clear();
    
    for(auto a : panty_list)
    {
        a->removeFromParent();
    }
    panty_list.clear();
}

void Obstacle::reset_obstacle()
{
    for(auto node : list)
    {
        node->add_score=true;
    }
}

