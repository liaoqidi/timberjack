//
//  GameOverGUILayer.h
//  胖次英雄
//
//  Created by lj on 14-3-4.
//
//

#ifndef ________GameOverGUILayer__
#define ________GameOverGUILayer__

#include <iostream>
#include "Game.h"
#include "GameShopGUILayer.h"
//#import "GameKitHelper.h"
//#import <iAd/iAd.h>
using namespace cocos2d::gui;
using namespace cocos2d;

enum class PantyType
{
    G_1,
    G_2,
    G_3,
    R_1,
    R_2,
    R_3,
    L_1,
    L_2,
    L_3,
};

class GameOverGUILayer : public Layer
{
public:
    GameOverGUILayer()
    {
		m_ScoreDataValue	= new DataValue(0);
		m_ScoreMaxDataValue	= new DataValue(0);
		m_PantsDataValue	= new DataValue(0);
        _score		= new DataValue(0);
        
        close=false;
        get_new_best=false;
        m_bShowShake=false;
        back=false;
        get_panty=false;
        show_get_panty=false;
        m_fShakeAmplitude=20;
    };
    ~GameOverGUILayer(){};
    
    virtual bool init();
    CREATE_FUNC(GameOverGUILayer);
    void action_fadein(Node *s);
    Button *chest;
    void action_remove(cocos2d::Node *node);
    Sprite* create_ui(const char *name);
    Sprite* create_ui(const char *name,float ws,float hs);
    bool get_panty;
    bool show_get_panty;
    cocos2d::Point offset_y;
    void addChild_all(...);
    void score_add_upadte(float dt);
    void call_restart(Object *obj,TouchEventType type);
    void call_shop(Object *obj,TouchEventType type);
    void call_trophy(Object *obj,TouchEventType type);
    void call_back(Object *obj,TouchEventType type);
    void call_get_chest(Object *obj,TouchEventType type);
	void CloseShop();
    void action_chest_touch(cocos2d::Node *node);
    Vector<Sprite*> ui_list;
    void set_anchor(Node *n);
    void shake_scene(cocos2d::Node *node);
    void update(float dt);
    void update_score();
    void action_shake(float dt);
    void add_iad();
	DataValue *m_ScoreDataValue;
	DataValue *m_ScoreMaxDataValue;
	DataValue *m_PantsDataValue;
    bool m_bShowShake;
    int shake_time;
    int m_fShakeTimer;
    float m_fShakeAmplitude;
    bool close;
    bool back;
   
    bool get_new_best;
private:
    Layout *over_view;
    
    DataValue *_score;
};

extern GameOverGUILayer *g_pGameOverGUILayer;

#endif /* defined(________GameOverGUILayer__) */
