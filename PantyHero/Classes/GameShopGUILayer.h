//
//  GameShopGUILayer.h
//  胖次英雄
//
//  Created by lj on 14-3-5.
//
//

#ifndef ________GameShopGUILayer__
#define ________GameShopGUILayer__

#include <iostream>
#include "cocos2d.h"
#include "GUISetting.h"
#include "CocosGUI.h"
#include "Setting.h"
#include "DataValue.h"
using namespace cocos2d;
using namespace gui;

#define TAG_CELL_HERO   1
#define TAG_CELL_PANTS  2

class GameShopGUILayer : public Layer
{
public:
    GameShopGUILayer()
    {
        play=false;
        value_pants=new DataValue(0);
        _value_pants=new DataValue(0);
    };
    ~GameShopGUILayer(){};
    
    Layout *m_layout;
    Vector<cocos2d::gui::Button*> button_list;
    Vector<cocos2d::gui::ImageView*> img_list;
    Button *tmp_use_button;
    ValueMap map_heroes;
    ValueMap map_panties;
    bool play;
    
    bool init();
    void heros_init();
    void pants_init();
    void add_pants(int i);
    void del_pants(int i);
    void update_pants_text(int pants);
    void trans_use_hero_id(Object *obj,TouchEventType type);
    Widget* init_cell(int type,int index);
    CREATE_FUNC(GameShopGUILayer);
    void action_fadein(Node *n);
    void call_tab_left_button(Object *obj,TouchEventType type);
    void call_tab_right_button(Object *obj,TouchEventType type);
    void call_back_button(Object *obj,TouchEventType type);
    void call_share_button(Object *obj,TouchEventType type);
    void call_left_toggle(Object *obj,CheckBoxEventType type);
    void call_right_toggle(Object *obj,CheckBoxEventType type);
    void call_exchange(Object *obj,TouchEventType type);
    void call_buy_button(Object *obj,TouchEventType type);
    void pants_add_upadte(float dt);
    DataValue *value_pants;
    DataValue *_value_pants;
	void SetPrevGameTag(GameTag curGameTag)	{ m_prevGameTag = curGameTag; };
	void UpdateGUI(int idx);

private:
	GameTag m_prevGameTag;
	Button *m_pButtonBuy;
};

extern GameShopGUILayer *g_pGameShopGUILayer;

#endif /* defined(________GameShopGUILayer__) */
