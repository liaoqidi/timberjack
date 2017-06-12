//
//  GameLayer.h
//  胖次英雄
//
//  Created by lyon on 14-3-3.
//
//

#ifndef ________GameLayer__
#define ________GameLayer__

#include <iostream>
#include "cocos2d.h"
#include "Hero.h"
#include "Obstacle.h"
#include "Setting.h"
#include "DataValue.h"
#include "cocos2d.h"
#include "GUISetting.h"
#include "CocosGUI.h"

using namespace cocos2d;
using namespace gui;

#define __BACKGROUND_HOUSE__

enum class ObstacleAddTag
{
    ADD_NORMAL,
    ADD_LIMIT,
    ADD_MOVE,
    ADD_MOVEBY,
    ADD_FIX,
};


class GameLayer : public Layer
{
public:
    GameLayer();
    ~GameLayer();
    
	void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent);
	GameTag GetCurGameTag()	{ return m_curGameTag; };
	GameTag GetOldGameTag()	{ return m_oldGameTag; };
	GameTag SetCurGameTag(GameTag curGameTag)	{ return m_curGameTag = curGameTag; };
    bool init();
	void SelectorGetUserID(CCNode *sender, void *data);
    void restart();
    void update(float dt);
    void obstacle_update();
    void sender();
    bool is_between(int min,int max);
    CREATE_FUNC(GameLayer);

	void ShowShop();
	void ShowPay(int nPayWay, float fRMB=2.0f, const char *pszPayCode="", int nHero=0, int nPanty=0);
	void ShowPayHero10();
	void ShowPayRelive();
	void SetTimeBack();

private:
    GameTag m_curGameTag;
	GameTag m_oldGameTag;
    
    //update m_fTime of m_pObstacle create
    float m_fGameTimer;
    float m_fTime;
    //float paly_dead_time;
    float m_fObstaclCreateTimer;
    //m_bShow over ui
    bool m_bShow;
    //m_bShow 
    bool m_bShowWarn;
    bool m_bShowShop;
    bool m_bShowStart;
    
    
    bool m_bShowShake;
    float m_fShakeAmplitude;
    float m_fShakeTimer;
    void action_shake(float dt);
    
    //score
    //random requence
    int m_nRandomReq;
    
#ifdef __BACKGROUND_HOUSE__
	Sprite *m_pBackground_1_0;
	Sprite *m_pBackground_1_1;
#endif //__BACKGROUND_HOUSE__
    Sprite *m_pForeground_0;
    Sprite *m_pForeground_1;
    
    Vector<Sprite*> m_vecLifeIcons;
    //speed setting
    float speed_hero;
    float speed_hero_up;
    float speed_gravity;
    float speed_tmp_1;
    float speed_tmp_2;
    float speed_background;
    float speed_obstacle;
    float speed_backtime;
    
    Obstacle *m_pObstacle;
    ObstacleAddTag m_ObstacleAddTag;
    //list
    Vector<Sprite* >list;
    Vector<Node* >score_list;
    ValueMap map_panty;
    Layout *ready_view;
	LabelTTF *m_pLabelTips;
    
    //scene size
    cocos2d::Size m_SceneSize;
    cocos2d::Point m_SceneOrigin;
    
public:
    
    //read user data
    void init_user_data();
    void load_user_data();
    void save_user_data();
    //create m_pHero
    void create_hero();

	void ShowTips(char *pszText);
    
    int m_nGameOverTimer;
    bool m_bPlayOver;
	bool m_bQuitGame;
	bool m_bPlayDead;
	Hero *m_pHero;
	int m_nSoreGift;
	DataValue *m_ScoreDataValue;
	DataValue *m_ScoreMaxDataValue;
	DataValue *m_PantsDataValue;

    void create_life_icon();
    void update_life_icon();
    void update_score();
	void SetPantys(int nPantys);
    
    void action_remove_node(cocos2d::Node *node_0,cocos2d::Node *node);
    
    void game_began();
    void game_ready();
    void game_playing();
    void game_playdead();
    void game_over();
    
    void game_timeBack();
    
    void add_pants(int i);
    void action_move_background();
    void action_moveBack_background(float lv);
    
    void game_start_ui_init();
    void game_end_ui_init();
    
    void add_obstacle();
    //add_obstacle_mode
    void add_type_normal();
    void add_tyoe_limit(){};
    void add_type_move(){};
    void add_type_moveby(){};
    void add_tyoe_fix(){};
};

extern GameLayer *g_pGameLayer;

#endif /* defined(________GameLayer__) */
