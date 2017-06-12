//
//  GameOverGUILayer.cpp
//  胖次英雄
//
//  Created by lj on 14-3-4.
//
//

#include "GameOverGUILayer.h"
#include "GameShopGUILayer.h"
#include "GameLayer.h"

//#import "iAdHelper.h"
using namespace cocos2d;
using namespace std;
using namespace gui;
#define TAG_SCORE       1
#define TAG_SCORE_MAX   2

GameOverGUILayer *g_pGameOverGUILayer = NULL;

bool GameOverGUILayer::init()
{
    bool ret=false;
    do
    {
        add_iad();
        offset_y=cocos2d::Point(0,0);
        //score
        over_view=dynamic_cast<Layout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile(file_json_gameover.c_str()));
        this->addChild(over_view,1);
        auto ui_score=LabelTTF::create("SCORE", GAME_FONT_NAME,120*RATIO);
        auto ui_score_max=LabelTTF::create("BEST", GAME_FONT_NAME,130*RATIO);;
        ui_score->setColor(cocos2d::Color3B(199,76,9));
        ui_score_max->setColor(cocos2d::Color3B(199,76,9));
        set_anchor(ui_score);
        ui_score->setPosition(POINT_UI_SCORE*RATIO+offset_y);
        set_anchor(ui_score_max);
        ui_score_max->setPosition(POINT_UI_SCORE_MAX*RATIO+offset_y);
        this->addChild(ui_score,5);
        this->addChild(ui_score_max,5);
        
        Button *start=(Button*)over_view->getChildByName("play_button");
        start->addTouchEventListener(this, toucheventselector(GameOverGUILayer::call_restart));
        Button *shop=(Button*)over_view->getChildByName("shop_button");
        shop->addTouchEventListener(this, toucheventselector(GameOverGUILayer::call_shop));
        Button *leaderboard=(Button*)over_view->getChildByName("leaderboard_button");
        leaderboard->addTouchEventListener(this, toucheventselector(GameOverGUILayer::call_trophy));
        Button *back=(Button*)over_view->getChildByName("button_back");
        back->setTouchEnabled(true);
        back->addTouchEventListener(this, toucheventselector(GameOverGUILayer::call_back));
        auto *ui_pants=create_ui(NAME_UI_GAMEOVER_PANTS);
        this->addChild(ui_pants,5);
        ui_pants->setPosition(POINT_UI_GAMEOVER_PANTS*RATIO+offset_y);
        action_fadein(ui_pants);
        scheduleUpdate();
        
        ret=true;
    } while (0);
    
    return ret;
}

void GameOverGUILayer::score_add_upadte(float dt)
{
    if(_score->get_data()<m_ScoreDataValue->get_data())
    {
        _score->offset_data(1);
        auto ui_score_value=(LabelTTF*)this->getChildByTag(TAG_SCORE);
        ui_score_value->setString(String::createWithFormat("%d",_score->get_data())->getCString());
		playEffect("music_fx_kata.mp3");
        
        if(_score->get_data()>=m_ScoreDataValue->get_data())
        {
            if(get_new_best)
            {
                ui_score_value->setColor(cocos2d::Color3B::RED);
                ui_score_value->setScale(1.5);
                auto new_record=create_ui("ui_img_gameover_newrecord.png");
                new_record->setPosition(cocos2d::Point(g_size.width*0.38,g_size.height/2));
                new_record->setScale(4);
                this->addChild(new_record,10);
                auto scale_to=ScaleTo::create(0.3f, 1);
                new_record->runAction(cocos2d::Sequence::create(scale_to,
                                                                CallFuncN::create(CC_CALLBACK_1(GameOverGUILayer::shake_scene, this)),
                                                                NULL));
            }
			else
			{
				playEffect("music_fx_over.mp3");
			}
        }
    }
}

void GameOverGUILayer::update(float dt)
{
    action_shake(dt);
}

void GameOverGUILayer::action_shake(float dt)
{
    if(!m_bShowShake)
        return;
    
    float randx = (CCRANDOM_0_1()*m_fShakeAmplitude-m_fShakeAmplitude);
    float randy = (CCRANDOM_0_1()*m_fShakeAmplitude-m_fShakeAmplitude);
    
    // move the target to a shaked position
    this->setPosition(cocos2d::Point(randx,randy));
    
    m_fShakeTimer-=dt;
    m_fShakeAmplitude-=1;
    if(m_fShakeTimer<0 || m_fShakeAmplitude<=0)
    {
        m_bShowShake=false;
        m_fShakeTimer=1;
        this->setPosition(cocos2d::Point(0,0));
        m_fShakeAmplitude=20;
		playEffect("music_fx_getnew_1.mp3");
    }
}

void GameOverGUILayer::update_score()
{
    auto ui_score_value=LabelTTF::create("0", GAME_FONT_NAME,90*RATIO);
    set_anchor(ui_score_value);
    ui_score_value->setPosition(POINT_UI_SCORE_VALUE*RATIO+offset_y);
    auto ui_score_max_value=LabelTTF::create("0", GAME_FONT_NAME,90*RATIO);
    set_anchor(ui_score_max_value);
    ui_score_max_value->setScale(1.5);
    ui_score_max_value->setPosition(POINT_UI_SCORE_MAX_VALUE*RATIO+offset_y);
    auto ui_pants_value=LabelTTF::create("0", GAME_FONT_NAME,100*RATIO);
    ui_pants_value->setAnchorPoint(cocos2d::Point(0,0.5));
    ui_pants_value->setPosition(POINT_UI_GAMEOVER_PANTS_VALUE*RATIO+offset_y);
    ui_pants_value->setTag(10);
    ui_score_value->setColor(cocos2d::Color3B::BLACK);
    ui_score_max_value->setColor(cocos2d::Color3B::BLACK);
    ui_pants_value->setColor(cocos2d::Color3B::BLACK);
    ui_score_value->setTag(TAG_SCORE);
    ui_score_value->setString(String::createWithFormat("%d",0)->getCString());
	ui_score_max_value->setString(String::createWithFormat("%d",m_ScoreMaxDataValue->get_data())->getCString());
	ui_pants_value->setString(String::createWithFormat(":%d",m_PantsDataValue->get_data())->getCString());
    ui_pants_value->setHorizontalAlignment(TextHAlignment::LEFT);
    const char *view_lv;
	if(m_ScoreDataValue->get_data()<5)
        view_lv=NAME_UI_GAMEOVER_VIEW_NORMAL;
    else if(m_ScoreDataValue->get_data()<20)
        view_lv=NAME_UI_GAMEOVER_VIEW_GOOD;
    else
        view_lv=NAME_UI_GAMEOVER_VIEW_EXCELLENT;
    
    auto ui_view=create_ui(view_lv);
    ui_view->setPosition(POINT_UI_GAMEOVER_VIEW*RATIO+offset_y);
    
    this->addChild(ui_view,5);
    this->addChild(ui_score_value,5);
    this->addChild(ui_score_max_value,5);
    this->addChild(ui_pants_value,5);
    
    Button *new_tip=Button::create();
    new_tip->getTitle()->setString(String::createWithFormat("%d",g_nGotPanty)->getCString());
    new_tip->getTitle()->setFontName(GAME_FONT_NAME);
    new_tip->getTitle()->setFontSize(60*RATIO);
    new_tip->loadTextureNormal("ui_img_common_newunlock_01.png",UI_TEX_TYPE_PLIST);
    new_tip->setPosition(cocos2d::Point(80*RATIO,80*RATIO));
    over_view->getChildByName("shop_button")->addChild(new_tip);
    new_tip->setName("new_tip");
    if(g_bSeePanty || g_nGotPanty<1)
        new_tip->setVisible(false);
    
    if(m_ScoreDataValue->get_data()>0)
    {
        float interval=1/(float)m_ScoreDataValue->get_data();
        this->schedule(schedule_selector(GameOverGUILayer::score_add_upadte), interval, kRepeatForever, 0.4f);
    }
    //if got the panty,then m_bShow the box
    if(!get_panty)
        return;
    
    chest=Button::create();
    chest->loadTextureNormal("ui_button_gameover_gift_sleep_02.png",UI_TEX_TYPE_PLIST);
    chest->setAnchorPoint(cocos2d::Point(0.5,0.5));
    chest->setPosition(over_view->getChildByName("shop_button")->getPosition());
    chest->setTouchEnabled(false);
    chest->addTouchEventListener(this, toucheventselector(GameOverGUILayer::call_get_chest));
    chest->setScale(0);
    ScaleTo *scale_to=ScaleTo::create(0.4f, 1.2f);
    ScaleTo *scale_to_1=ScaleTo::create(0.2f, 0.8f);
    chest->runAction(cocos2d::Sequence::create(scale_to,
                                               scale_to_1,
                                               CallFuncN::create(CC_CALLBACK_1(GameOverGUILayer::action_chest_touch, this)),
                                               NULL));
    g_nGotPanty++;
    over_view->addChild(chest,20);
}

void GameOverGUILayer::action_chest_touch(cocos2d::Node *node)
{
    chest->setTouchEnabled(true);
    over_view->getChildByName("shop_button")->setTouchEnabled(false);
}

void GameOverGUILayer::action_remove(cocos2d::Node *node)
{
    chest->removeFromParent();
    call_shop(nullptr, TouchEventType::TOUCH_EVENT_ENDED);
}

void GameOverGUILayer::call_get_chest(cocos2d::Object *obj, TouchEventType type)
{
    if(type == TOUCH_EVENT_ENDED)
        return;
    chest->loadTextureNormal("ui_button_gameover_gift_wakeup_01.png",UI_TEX_TYPE_PLIST);
    Button *new_tip=(Button*)over_view->getChildByName("shop_button")->getChildByName("new_tip");
    new_tip->setVisible(true);
    new_tip->getTitle()->setString(String::createWithFormat("%d",g_nGotPanty)->getCString());
    over_view->getChildByName("shop_button")->setTouchEnabled(false);
    ScaleTo *scale_to=ScaleTo::create(0.7f, 0);
    FadeOut *out=FadeOut::create(1);
    chest->runAction(cocos2d::Sequence::create(cocos2d::Spawn::create(scale_to,out, NULL),
                                               CallFuncN::create(CC_CALLBACK_1(GameOverGUILayer::action_remove, this)),
                                               NULL));
}

void GameOverGUILayer::call_restart(Object *obj,TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
        return;
    playEffect("music_fx_select.mp3");
    close=true;
    
    //[iAdHelper remove_iAdHelper];
}

void GameOverGUILayer::call_shop(Object *obj,TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED)
        return;

	playEffect("music_fx_select.mp3");

	g_pGameLayer->ShowShop();
}

void GameOverGUILayer::CloseShop()
{
	if (g_pGameShopGUILayer && g_pGameShopGUILayer->play)
		close=true;

	over_view->getChildByName("shop_button")->setTouchEnabled(true);
	over_view->getChildByName("shop_button")->getChildByName("new_tip")->setVisible(false);
	auto ui_pants_value=(LabelTTF*)this->getChildByTag(10);
	int _tmp=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
	ui_pants_value->setString(String::createWithFormat(":%d",_tmp)->getCString());
	add_iad();
}

void GameOverGUILayer::call_trophy(Object *obj,TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");
        //[[GameKitHelper shared_gameKit_helper] show_board_leader];
		if (g_pGameLayer)
			g_pGameLayer->ShowTips(NULL);
    }
}

void GameOverGUILayer::call_back(cocos2d::Object *obj, TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");
        back=true;
        //[iAdHelper remove_iAdHelper];
    }
}

void GameOverGUILayer::add_iad()
{
    //UIWindow *window=[[UIApplication sharedApplication] keyWindow];
    //iAdHelper *iAd=[iAdHelper shared_iAdHelper];
    //[window addSubview:iAd.view];
}

Sprite* GameOverGUILayer::create_ui(const char* name,float ws,float hs)
{
    auto *ui=Sprite::createWithSpriteFrameName(name);
    ui->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    ui->setScale(ws,hs);
    return ui;
}

Sprite* GameOverGUILayer::create_ui(const char* name)
{
    auto *ui=Sprite::createWithSpriteFrameName(name);
    ui->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
    ui->setScale(1,1);
    return ui;
}

void GameOverGUILayer::shake_scene(cocos2d::Node *node)
{
    m_bShowShake=true;
    shake_time=0;
	playEffect("music_fx_getnew.mp3");
}

void GameOverGUILayer::set_anchor(Node *n)
{
    n->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
}

void GameOverGUILayer::action_fadein(Node *n)
{
    auto fadein=FadeIn::create(1);
    n->setOpacity(0);
    n->runAction(fadein);
}