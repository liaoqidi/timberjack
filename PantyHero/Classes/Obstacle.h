//
//  Obstacle.h
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#ifndef ________Obstacle__
#define ________Obstacle__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

#define FIX_HEIGHT      600
#define FIX_HEIGHT_SHAKE     700
#define TIMER_CREATE_WAIT    300
#define TIMER_CREATE_NEAR    60
#define TIMER_CREATE_FAR     200
#define TIMER_CREATE_NORMAL  110


enum class ObstacleTag
{
    NUL,
    PANTY,
    OBSTACLE,
};

enum class ObstacleActionTag
{
    TAG_NORMAL,
    TAG_MOVE,
    TAG_FIX_MOVE,
    TAG_LIMIT,
};
enum class ObstacleCreateTag
{
    TAG_THIN,
    TAG_CLOSE,
};

enum class ObstacleMoveTag
{
    TAG_UP,
    TAG_DOWN,
};

enum class ObstacleRankTag
{
    TAG_NORMAL,
    TAG_HARD,
    TAG_MASTER,
};

enum class CreateComboTag
{
    TAG_WAVES,
    TAG_LEVELS_NORMAL,
    TAG_LEVELS_RISE,
    TAG_LEVELS_FALL,
    TAG_LADDER_NORMAL,
    TAG_LADDER_RISE,
    TAG_LADDER_FALL,
    TAG_SHAKE,
    TAG_SWITHC,
    TAG_RANDOM,
    
    TAG_NORMAL,
};

class _Obstacle : public Node
{
public:
    _Obstacle(){};
    ~_Obstacle(){};
    ObstacleActionTag action_tag;
    ObstacleMoveTag move_tag;
    ObstacleRankTag rank_tag;
    float counter;
    bool add_score;
    CREATE_FUNC(_Obstacle);
};

class Obstacle : public Node
{
public:
    float speed;
    bool limit_lock;
    int limit_count;
    int near_count;
    Vector<_Obstacle*> list;
    Vector<Sprite*> panty_list;
    ObstacleCreateTag create_tag;
    CreateComboTag combo_tag;
    bool switch_fall;
    bool switch_rise;
    float last_PosY;
    int combo_timer;
    std::function<void()> update_score;
    float spring_create;
    
    Obstacle();
    ~Obstacle();
    
    void add_obstacle(int score);
    void del_obstacle();
    void update(int score);
    void update_back(float lv);
    void reset_obstacle();
    ObstacleTag is_hit(cocos2d::Rect rect);
    bool is_between(int min,int max,int val);
    bool is_between(float min,float max,float val);
    CREATE_FUNC(Obstacle);
private:
    float m_fTime;
    float timer;
    
    //combo function
    void combo_random(_Obstacle *tmp);
    void combo_shake(_Obstacle *tmp);
    void combo_limit(_Obstacle *tmp);
};

#endif /* defined(________Obstacle__) */
