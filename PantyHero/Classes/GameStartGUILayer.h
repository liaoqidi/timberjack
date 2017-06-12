//
//  GameStartGUILayer.h
//  PantsHero
//
//  Created by lj on 14-3-6.
//
//

#ifndef __PantsHero__GameStartGUILayer__
#define __PantsHero__GameStartGUILayer__

#include <iostream>
#include "cocos2d.h"
#include "CocosGUI.h"
#include "CocoStudio.h"
#include "GameShopGUILayer.h"
//#include "GameKitHelper.h"
#include "global.h"
#include "ProducerView.h"

using namespace cocos2d::gui;
using namespace cocostudio;
using namespace cocos2d;

class GameStartGUILayer : public Layer
{
public:
    GameStartGUILayer()
    {
        close=false;
        anim_fps=10;
        anim_farme=1;
        anim_time=0;
        show_producer=false;
    };
    ~GameStartGUILayer(){};
    bool init();
    CREATE_FUNC(GameStartGUILayer);
    void update(float dt);

	void SelectorMusicOn(CCNode *sender, void *data);
	void SelectorMusicOff(CCNode *sender, void *data);
	void SetUserID(int nUserID);
    
    //function call
    void call_leaderbord(Object *obj,TouchEventType type);
    void call_start(Object *obj,TouchEventType type);
    void call_shop(Object *obj,TouchEventType type);
    void call_rate(Object *obj,TouchEventType type);
    void call_comment(Object *obj,TouchEventType type);
    void call_share(Object *obj,TouchEventType type);
    void call_setting(Object *obj,TouchEventType type);
    void call_scroll_left(Object *obj,TouchEventType type);
    void call_scroll_right(Object *obj,TouchEventType type);
    void call_page_turning(Object *obj,PageViewEventType type);
    void call_buy(Object *obj,TouchEventType type);
    void call_music(Object *obj,CheckBoxEventType type);
    void call_view_producer(Object *obj,TouchEventType type);

    void scrollToPage(int nPage);
    void set_widget_touch(Widget *w,bool tg);
    void action_move_setting();
    void update_play_button();
    void update_panty_score();
    bool show_setting;
    
    //producer view
    bool show_producer;
    bool isShow_producer;
    
    void init_pageview_hero();
    int anim_fps;
    int anim_time;
    int anim_farme;
    PageView *page_view_hero;
    bool close;
private:
    Layout *m_layout;
    ValueMap map_heroes;
    ProducerView *_producer_view;
	LabelTTF *m_pLabelUserID;
};

extern GameStartGUILayer *g_pGameStartGUILayer;

#endif /* defined(__PantsHero__GameStartGUILayer__) */
