 //
//  GameShopGUILayer.cpp
//  胖次英雄
//
//  Created by lj on 14-3-5.
//
//

#include "GameShopGUILayer.h"
#include "CocoStudio.h"
#include "CocosGUI.h"
#include "Setting.h"
#include "global.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"
#include "GameOverGUILayer.h"
#include "GameStartGUILayer.h"
#include "GamePayGUILayer.h"
#include "autostring.h"

#define TAG_LEFT_TOGGLE 1
#define TAG_RIGHT_TOGGLE    2
#define TAG_SCROLLVIEW  3
#define TAG_LEFT_TEXT   4
#define TAG_RIGHT_TEXT  5
#define TAG_PANTIES_VAL 66

GameShopGUILayer *g_pGameShopGUILayer = NULL;

//#define __IOS__
bool GameShopGUILayer::init()
{
    bool ret=false;
    do
    {
        //load plist
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file_plist_panty.c_str());
        //load layout info
        m_layout=dynamic_cast<Layout*>(cocostudio::GUIReader::shareReader()->widgetFromJsonFile(file_json_shop.c_str()));
        
        //top toggles for swith heros and pants
        CheckBox *top_left_toggle=dynamic_cast<CheckBox*>(m_layout->getChildByName("toggle_left"));
        CheckBox *top_right_toggle=dynamic_cast<CheckBox*>(m_layout->getChildByName("toggle_right"));
        
        int got_hero_count=0;
        for(auto tmp : g_vecHasHeroes)
        {
            if(tmp)
                got_hero_count++;
        }
        int got_panty_count=0;
        for(auto tmp : g_vecHasPanties)
        {
            if(tmp)
                got_panty_count++;
        }
        std::string got_hero_count_text=String::createWithFormat("%d",got_hero_count)->getCString();
        std::string got_panty_count_text=String::createWithFormat("%d",got_panty_count)->getCString();
        
        auto left_toggle_text=LabelTTF::create("Heroes:"+got_hero_count_text, GAME_FONT_NAME, 150*RATIO);
        left_toggle_text->setAnchorPoint(cocos2d::Point(0.5,0));
        left_toggle_text->setPosition(cocos2d::Point(g_size.width*0.25,g_size.height*0.915));
        left_toggle_text->setColor(cocos2d::Color3B(199,76,9));
        left_toggle_text->setTag(TAG_LEFT_TEXT);
        auto right_toggle_text=LabelTTF::create("Panties:"+got_panty_count_text, GAME_FONT_NAME, 150*RATIO);
        right_toggle_text->setAnchorPoint(cocos2d::Point(0.5,0));
        right_toggle_text->setPosition(cocos2d::Point(g_size.width*0.75,g_size.height*0.915));
        right_toggle_text->setColor(cocos2d::Color3B::BLACK);
        right_toggle_text->setTag(TAG_RIGHT_TEXT);
        this->addChild(left_toggle_text,10);
        this->addChild(right_toggle_text,10);
        
        Button *button_left=dynamic_cast<Button*>(m_layout->getChildByName("left_button"));
        Button *button_right=dynamic_cast<Button*>(m_layout->getChildByName("right_button"));
        Label *label_best=dynamic_cast<Label*>(m_layout->getChildByName("best_text"));
        int best=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_SCORE_MAX);
        //label_best->setString(String::createWithFormat("BEST: %d",best)->getCString());
        
        //bottom buttons
        Button *back=dynamic_cast<Button*>(m_layout->getChildByName("back_button"));
        Button *share=dynamic_cast<Button*>(m_layout->getChildByName("share_button"));
        
        //set listener
        back->addTouchEventListener(this, toucheventselector(GameShopGUILayer::call_back_button));
        share->addTouchEventListener(this, toucheventselector(GameShopGUILayer::call_share_button));
        
        top_left_toggle->addEventListenerCheckBox(this,checkboxselectedeventselector(GameShopGUILayer::call_left_toggle));
        top_right_toggle->addEventListenerCheckBox(this,checkboxselectedeventselector(GameShopGUILayer::call_right_toggle));
        button_left->addTouchEventListener(this,toucheventselector(GameShopGUILayer::call_tab_left_button));
        button_right->addTouchEventListener(this, toucheventselector(GameShopGUILayer::call_tab_right_button));
        //bottom info label
        int best_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_SCORE_MAX, 0);
        std::string best_score_text=String::createWithFormat("%d",best_score)->getCString();
        auto best_ttf=LabelTTF::create("Best:", GAME_FONT_NAME, 130*RATIO);
        auto best_ttf_val=LabelTTF::create(best_score_text, GAME_FONT_NAME, 130*RATIO);
        auto panties_icon=Sprite::createWithSpriteFrameName("scene_build_panty_0.png");
        int panty_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS, 0);
        std::string score_text=String::createWithFormat("%d",panty_score)->getCString();
        auto panties_val=LabelTTF::create(":"+score_text, GAME_FONT_NAME, 130*RATIO);
        panties_val->setTag(TAG_PANTIES_VAL);
        best_ttf->setAnchorPoint(cocos2d::Point(1,0.5));
        best_ttf_val->setAnchorPoint(cocos2d::Point(0.5,0.5));
        panties_icon->setAnchorPoint(cocos2d::Point(1,0.5));
        panties_val->setAnchorPoint(cocos2d::Point(0,0.5));
        best_ttf->setHorizontalAlignment(TextHAlignment::RIGHT);
        best_ttf_val->setHorizontalAlignment(TextHAlignment::LEFT);
        panties_val->setHorizontalAlignment(TextHAlignment::LEFT);
        best_ttf->setPosition(cocos2d::Point(g_size.width*0.33,115*RATIO));
        best_ttf_val->setPosition(cocos2d::Point(g_size.width*0.37,115*RATIO));
        panties_icon->setPosition(cocos2d::Point(g_size.width*0.65,115*RATIO));
        panties_val->setPosition(cocos2d::Point(g_size.width*0.65,115*RATIO));
        best_ttf->setColor(cocos2d::Color3B(136,23,103));
        best_ttf_val->setColor(cocos2d::Color3B(199,76,9));
        panties_val->setColor(cocos2d::Color3B(199,76,9));
        this->addChild(best_ttf,10);
        this->addChild(panties_icon,10);
        this->addChild(panties_val,10);
        this->addChild(best_ttf_val,10);
#ifdef __IOS__
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_heroes_list.plist");
        map_heroes= FileUtils::getInstance()->getValueMapFromFile(fullPath);
        fullPath = FileUtils::getInstance()->fullPathForFilename("data_panty_list.plist");
        map_panties= FileUtils::getInstance()->getValueMapFromFile(fullPath);
#else
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_heroes_list.plist");
        map_heroes= FileUtils::getInstance()->getValueMapFromFile(fullPath);
		fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_panty_list.plist");
        map_panties= FileUtils::getInstance()->getValueMapFromFile(fullPath);
#endif //__IOS__

        
        //jion layer
        this->addChild(m_layout);
        
        if(g_bSeePanty || g_nGotPanty<1)
        {
            heros_init();
            top_left_toggle->setTouchEnabled(false);
            top_left_toggle->setSelectedState(true);
            
            top_right_toggle->setTouchEnabled(true);
            top_right_toggle->setSelectedState(false);
            this->getChildByTag(TAG_RIGHT_TEXT)->setColor(cocos2d::Color3B::BLACK);
            this->getChildByTag(TAG_LEFT_TEXT)->setColor(cocos2d::Color3B(199,76,9));
        }else
        {
            pants_init();
            top_left_toggle->setTouchEnabled(true);
            top_left_toggle->setSelectedState(false);
             
            top_right_toggle->setTouchEnabled(false);
            top_right_toggle->setSelectedState(true);
            this->getChildByTag(TAG_RIGHT_TEXT)->setColor(cocos2d::Color3B(199,76,9));
            this->getChildByTag(TAG_LEFT_TEXT)->setColor(cocos2d::Color3B::BLACK);
        }
        //action
        action_fadein(m_layout);
        g_bSeePanty=true;
        g_nGotPanty=0;
        UserDefault::sharedUserDefault()->flush();
        ret=true;
    } while (0);
    
    int pants=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
    _value_pants->set_data(pants);
	value_pants->set_data(pants);
	this->schedule(schedule_selector(GameShopGUILayer::pants_add_upadte), 0.01f, kRepeatForever, 0.4f);

    return ret;
}

void GameShopGUILayer::heros_init()
{
    PageView *page_view=(PageView*)m_layout->getChildByName("PageView_66");
    if(page_view==NULL)
        return;
    float a=9%4;
    int max_number=12;
    int tmp=0;
    if(max_number%4>0)
    {
        tmp=1;
    }
    int page_number=(int)max_number/4+tmp;
    int i=1;
    for(int x=0;x<page_number;x++)
    {
        Layout *page=Layout::create();
        for(int y=0;y<4;y++)
        {
            Widget *cell=init_cell(TAG_CELL_HERO, i);
            cell->setAnchorPoint(cocos2d::Point(0.5,0));
            
            switch (y)
            {
                case 0:
                    cell->setPosition(cocos2d::Point(1350*0.25*RATIO,1350*0.58*RATIO));
                    break;
                case 1:
                    cell->setPosition(cocos2d::Point(1350*0.75*RATIO,1350*0.58*RATIO));
                    break;
                case 2:
                    cell->setPosition(cocos2d::Point(1350*0.25*RATIO,1350*0.02*RATIO));
                    break;
                case 3:
                    cell->setPosition(cocos2d::Point(1350*0.75*RATIO,1350*0.02*RATIO));
                    break;
            }
            if(i>max_number)
            {
                page_view->addPage(page);
                return;
            }
            page->addChild(cell);
            i++;
        }
        page_view->addPage(page);
    }
}

void GameShopGUILayer::pants_init()
{
    PageView *page_view=(PageView*)m_layout->getChildByName("PageView_66");
    if(page_view==NULL)
        return;
    int max_number=57;
    int tmp=0;
    if(max_number%4>0)
    {
        tmp=1;
    }
    int page_number=(int)max_number/4+tmp;
    int i=1;
    
    for(int x=0;x<page_number;x++)
    {
        Layout *page=Layout::create();
        for(int y=0;y<4;y++)
        {
            Widget *cell=init_cell(TAG_CELL_PANTS, i);
            cell->setAnchorPoint(cocos2d::Point(0.5,0));
            switch (y)
            {
                case 0:
                    cell->setPosition(cocos2d::Point(1350*0.25*RATIO,1350*0.58*RATIO));
                    break;
                case 1:
                    cell->setPosition(cocos2d::Point(1350*0.75*RATIO,1350*0.58*RATIO));
                    break;
                case 2:
                    cell->setPosition(cocos2d::Point(1350*0.25*RATIO,1350*0.02*RATIO));
                    break;
                case 3:
                    cell->setPosition(cocos2d::Point(1350*0.75*RATIO,1350*0.02*RATIO));
                    break;
            }
            if(i>max_number)
            {
                page_view->addPage(page);
                goto MARK_SET;
            }
            page->addChild(cell);
            i++;
        }
        page_view->addPage(page);
    }
    
MARK_SET:
    
    int list_count=0;
    int count=0;
    for(auto has : g_vecHasPanties)
    {
        if(has)
        {
            int index=list_count+1;

            if(!g_vecHasExchange[index-1])
            {
                ImageView *pants_bg_img=(ImageView*)img_list.at(count)->getChildByName("bg_img");
                ImageView *pants_img=(ImageView*)pants_bg_img->getChildByName("img");
                Button *text=(Button*)img_list.at(count)->getChildByName("text");
                Button *text_1=(Button*)img_list.at(count)->getChildByName("text_1");
                img_list.at(count)->getChildByName("icon")->setVisible(true);
                //text
                ValueMap node=map_panties.at(String::createWithFormat("%d",index)->getCString()).asValueMap();
                std::string type_name=node.at("Quality").asString();
                
                if(type_name== "general_1" || type_name=="general_2" || type_name=="general_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_001.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }else if(type_name== "rare_1" || type_name=="rare_2" || type_name=="rare_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_003.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }else if(type_name=="legend_1" || type_name=="legend_2" || type_name=="legend_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_002.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }
                std::string text_name=node.at("Name").asString();
                text->getTitle()->setString(text_name);
                text_1->getTitle()->setString("x"+node.at("Score").asString());
                Button *exchange=Button::create();
                exchange->loadTextureNormal("ui_button_shop_buy_01.png",UI_TEX_TYPE_PLIST);
                exchange->setAnchorPoint(cocos2d::Point(0.5,0));
                exchange->setPosition(cocos2d::Point(0,-20*RATIO));
                exchange->setTitleFontName("Helvetica-Bold");
				autostring str;
				exchange->setTitleText(str.ANSIToUTF8("领取积分"));
                exchange->setTitleFontSize(70*RATIO);
                exchange->getTitle()->setColor(cocos2d::Color3B(65,65,65));
                exchange->setTag(index);
                exchange->setTouchEnabled(true);
                exchange->addTouchEventListener(this, toucheventselector(GameShopGUILayer::call_exchange));
                img_list.at(count)->addChild(exchange);
                
                auto new_tip=Button::create();
                new_tip->loadTextureNormal("ui_img_common_newunlock_01.png",UI_TEX_TYPE_PLIST);
                new_tip->setAnchorPoint(cocos2d::Point(0.5,0));
                new_tip->setPosition(cocos2d::Point(200*RATIO,600*RATIO));
                new_tip->setTitleFontName("Helvetica-Bold");
                new_tip->setTitleText("new");
                new_tip->setTitleFontSize(50*RATIO);
                new_tip->getTitle()->setColor(cocos2d::Color3B::WHITE);
                img_list.at(count)->addChild(new_tip);
                count++;
            }
        }
        list_count++;
    }
    
    list_count=0;
    for(auto has : g_vecHasPanties)
    {
        if(has)
        {
            int index=list_count+1;
            
            if(g_vecHasExchange[index-1])
            {
                ImageView *pants_bg_img=(ImageView*)img_list.at(count)->getChildByName("bg_img");
                ImageView *pants_img=(ImageView*)pants_bg_img->getChildByName("img");
                Button *text=(Button*)img_list.at(count)->getChildByName("text");
                Button *text_1=(Button*)img_list.at(count)->getChildByName("text_1");
                img_list.at(count)->getChildByName("icon")->setVisible(true);
                //text
                ValueMap node=map_panties.at(String::createWithFormat("%d",index)->getCString()).asValueMap();
                std::string type_name=node.at("Quality").asString();
                
                if(type_name== "general_1" || type_name=="general_2" || type_name=="general_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_001.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }else if(type_name== "rare_1" || type_name=="rare_2" || type_name=="rare_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_003.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }else if(type_name=="legend_1" || type_name=="legend_2" || type_name=="legend_3")
                {
                    pants_bg_img->loadTexture("panty_view_bg_002.png",UI_TEX_TYPE_PLIST);
                    pants_img->loadTexture((node.at("Number").asString()+".png").c_str(),UI_TEX_TYPE_PLIST);
                }
                std::string text_name=node.at("Name").asString();
                text->getTitle()->setString(text_name);
                text_1->getTitle()->setString("x"+node.at("Score").asString());
                count++;
            }
        }
        list_count++;
    }
    
}

Widget* GameShopGUILayer::init_cell(int type,int index)
{
	autostring str;
    ImageView *cell=ImageView::create();
    cell->loadTexture("ui_view_shop_viewbox_01.png",UI_TEX_TYPE_PLIST);
    cell->setAnchorPoint(cocos2d::Point(0.5,0));
    if(type==TAG_CELL_HERO)
    {
        //img
        ImageView *hero_img=ImageView::create();
        hero_img->loadTexture(String::createWithFormat("hero_%03d_01.png",index)->getCString(),UI_TEX_TYPE_PLIST);
        hero_img->setAnchorPoint(cocos2d::Point(0.5,0));
        hero_img->setPosition(cocos2d::Point(0,350*RATIO));
        
        //text
        ValueMap node=map_heroes.at(String::createWithFormat("%d",index)->getCString()).asValueMap();
        std::string text_name=node.at("LanguageName").asString();
        auto label_text_name=Button::create();
        label_text_name->setAnchorPoint(cocos2d::Point(0.5,0));
        label_text_name->setPosition(cocos2d::Point(0,650*RATIO));
        label_text_name->setTitleColor(Color3B::BLACK);
        label_text_name->setTitleFontName("Helvetica-Bold");
        label_text_name->setTitleText(text_name);
        label_text_name->setTitleFontSize(50*RATIO);
        label_text_name->getTitle()->setColor(cocos2d::Color3B(136,23,103));
        cell->addChild(label_text_name);
        
        std::string text_1=node.at("LanguageDesc").asString();
        auto text_des=Button::create();
        text_des->setAnchorPoint(cocos2d::Point(0.5,0));
        text_des->setPosition(cocos2d::Point(0,200*RATIO));
        text_des->setTitleColor(Color3B::BLACK);
        text_des->setTitleFontName("Helvetica-Bold");
        text_des->setTitleText(text_1);
        text_des->setTitleFontSize(48*RATIO);
        text_des->getTitle()->setColor(cocos2d::Color3B(65,65,65));
        text_des->getTitle()->setDimensions(cocos2d::Size(500*RATIO,200*RATIO));
        text_des->getTitle()->setHorizontalAlignment(TextHAlignment::CENTER);
        cell->addChild(text_des);
        
        int hp=node.at("Function").asInt()+1;
        for(int i=0;i<hp;i++)
        {
            ImageView *hp_img=ImageView::create();
            hp_img->loadTexture("ui_img_stargame_life_icon.png",UI_TEX_TYPE_PLIST);
            hp_img->setAnchorPoint(cocos2d::Point(1,0));
            hp_img->setScale(0.7f);
            hp_img->setPosition(cocos2d::Point(258*RATIO,570*RATIO-60*i*RATIO));
            cell->addChild(hp_img);
        }
        
        if(!g_vecHasHeroes[index-1])
        {
            m_pButtonBuy = Button::create();
            m_pButtonBuy->loadTextureNormal("ui_button_shop_buy_01.png",UI_TEX_TYPE_PLIST);
            m_pButtonBuy->setAnchorPoint(cocos2d::Point(0.5,0));
            m_pButtonBuy->setPosition(cocos2d::Point(0,-10*RATIO));
            m_pButtonBuy->setName(String::createWithFormat("%d",index)->getCString());

            if(node.at("Company").asString() == "panty score")
            {
                auto title_icon=ImageView::create();
                title_icon->loadTexture("ui_img_common_panty_icon.png",UI_TEX_TYPE_PLIST);
                title_icon->setAnchorPoint(cocos2d::Point(1,0.3f));
                title_icon->setPosition(cocos2d::Point(-10*RATIO,50*RATIO));
                title_icon->setTag(2);
                m_pButtonBuy->addChild(title_icon);
                std::string title_text=node.at("Price").asString();
                auto title=m_pButtonBuy->getTitle();
                title->setString(title_text);
                title->setFontSize(68*RATIO);
                title->setFontName(GAME_FONT_NAME);
                title->setColor(cocos2d::Color3B::BLACK);
                title->setHorizontalAlignment(TextHAlignment::LEFT);
                title->setAnchorPoint(cocos2d::Point(0,0.3f));
                title->setPosition(cocos2d::Point(0,65*RATIO));
            }else if(node.at("Company").asString() == "real money(rmb)")
            {
                if(m_pButtonBuy->getChildByTag(2)!=NULL)
                {
                    m_pButtonBuy->getChildByTag(2)->setVisible(false);
                }
                std::string title_text=node.at("Price").asString();
                auto title=m_pButtonBuy->getTitle();
                title->setString("RMB:"+title_text);
                title->setFontSize(68*RATIO);
                title->setColor(cocos2d::Color3B::BLACK);
                title->setAnchorPoint(cocos2d::Point(0.5,0));
                title->setPosition(cocos2d::Point(0,50*RATIO));
            }
            cell->addChild(m_pButtonBuy,5);
            m_pButtonBuy->setTouchEnabled(true);
            m_pButtonBuy->addTouchEventListener(this, toucheventselector(GameShopGUILayer::call_buy_button));
        }
        //button
        Button *button=Button::create();
        button->loadTextureNormal("ui_button_shop_buy_01.png",UI_TEX_TYPE_PLIST);
        button->setAnchorPoint(cocos2d::Point(0.5,0));
        button->setPosition(cocos2d::Point(0,-10*RATIO));
        button->setTitleFontName("Helvetica-Bold");
		autostring str;
		button->setTitleText(str.ANSIToUTF8("使用"));
        button->setTitleFontSize(70*RATIO);
        button->setTouchEnabled(true);
        button->getTitle()->setColor(cocos2d::Color3B(65,65,65));
        button->setTag(index);
        if(!g_vecHasHeroes[index-1])
        {
            button->setTouchEnabled(false);
            button->setVisible(false);
        }
        button->addTouchEventListener(this, toucheventselector(GameShopGUILayer::trans_use_hero_id));
        button_list.pushBack(button);
        if(index==g_nHeroID)
        {
            tmp_use_button=button;
            button->loadTextureNormal("ui_button_shop_bought.png",UI_TEX_TYPE_PLIST);
			button->setTitleText(str.ANSIToUTF8("玩一把!"));
        }
        //jion the cell
        cell->addChild(button,3);
        cell->addChild(hero_img);
    }else
    {
        ImageView *pants_bg_img=ImageView::create();
        ImageView *pants_img=ImageView::create();
        
        pants_bg_img->loadTexture("panty_bg_004.png",UI_TEX_TYPE_PLIST);
        
        pants_bg_img->setAnchorPoint(cocos2d::Point(0.5,0));
        pants_bg_img->setPosition(cocos2d::Point(0,250*RATIO));
        pants_img->setAnchorPoint(cocos2d::Point(0.5,0));
        pants_bg_img->addChild(pants_img);
        
        pants_bg_img->setName("bg_img");
        pants_img->setName("img");
        
        Button *text=Button::create();
        text->setAnchorPoint(cocos2d::Point(0.5,0));
        text->setPosition(cocos2d::Point(0,190*RATIO));
        text->setTitleFontName("Helvetica-Bold");
		text->setTitleText(str.ANSIToUTF8("未获得"));
        text->setTitleFontSize(70*RATIO);
        text->getTitle()->setColor(cocos2d::Color3B(65,65,65));
        text->setName("text");
        text->setTouchEnabled(false);
        cell->addChild(text);
        
        
        ImageView *icon=ImageView::create();
        icon->loadTexture("ui_img_common_panty_icon.png",UI_TEX_TYPE_PLIST);
        icon->setScale(0.8f);
        icon->setAnchorPoint(cocos2d::Point(1,0));
        icon->setPosition(cocos2d::Point(-20*RATIO,50*RATIO));
        icon->setName("icon");
        icon->setVisible(false);
        cell->addChild(icon);
        Button *text_1=Button::create();
        text_1->setAnchorPoint(cocos2d::Point(0,0.5));
        text_1->setPosition(cocos2d::Point(30*RATIO,80*RATIO));
        text_1->setTitleFontName("Helvetica-Bold");
        text_1->getTitle()->setHorizontalAlignment(TextHAlignment::LEFT);
        text_1->setTitleText("");
        text_1->setTitleFontSize(60*RATIO);
        text_1->getTitle()->setColor(cocos2d::Color3B(65,65,65));
        text_1->setName("text_1");
        text_1->setTouchEnabled(false);
        cell->addChild(text_1);
        
        cell->addChild(pants_bg_img);
        
        img_list.pushBack(cell);
    }

    return cell;
}
void GameShopGUILayer::call_tab_left_button(Object *obj,TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    
    PageView *view=(PageView*)m_layout->getChildByName("PageView_66");
    if(view->getCurPageIndex()>0)
    {
        view->scrollToPage(view->getCurPageIndex()-1);
		playEffect("music_fx_back.mp3");
    }
}

void GameShopGUILayer::call_tab_right_button(Object *obj,TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    
    PageView *view=(PageView*)m_layout->getChildByName("PageView_66");
    if(view->getCurPageIndex()<view->getPages().size()-1)
    {
        view->scrollToPage(view->getCurPageIndex()+1);
		playEffect("music_fx_back.mp3");
    }
}

void GameShopGUILayer::call_back_button(Object *obj,TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");

		if (g_pGameLayer->GetCurGameTag() == GameTag::GAME_OVER)
			g_pGameOverGUILayer->CloseShop();

		if (g_pGameShopGUILayer)
		{
			g_pGameLayer->SetCurGameTag((GameTag)m_prevGameTag);
			removeFromParent();
			g_pGameShopGUILayer = NULL;
		}
    }
}

void GameShopGUILayer::call_share_button(Object *obj,TouchEventType type)
{
	if (g_pGameLayer)
		g_pGameLayer->ShowTips(NULL);
}

void GameShopGUILayer::call_left_toggle(cocos2d::Object *obj, CheckBoxEventType type)
{
    if(type==CHECKBOX_STATE_EVENT_SELECTED)
    {
		playEffect("music_fx_select.mp3");
        CheckBox *right=(CheckBox*)m_layout->getChildByName("toggle_right");
        m_layout->getChildByName("toggle_left")->setTouchEnabled(false);
        right->setTouchEnabled(true);
        right->setSelectedState(false);
        this->getChildByTag(TAG_RIGHT_TEXT)->setColor(cocos2d::Color3B::BLACK);
        this->getChildByTag(TAG_LEFT_TEXT)->setColor(cocos2d::Color3B(199,76,9));
        PageView* pages=(PageView*)m_layout->getChildByName("PageView_66");
        pages->scrollToPage(1);
        //pages->removeAllPages();
        pages->cocos2d::gui::Widget::removeAllChildren();
        img_list.clear();
        heros_init();
        pages->scrollToPage(0);
    }
}

void GameShopGUILayer::call_right_toggle(Object *obj, CheckBoxEventType type)
{
    if(type==CHECKBOX_STATE_EVENT_SELECTED)
    {
		playEffect("music_fx_select.mp3");
        CheckBox *left=(CheckBox*)m_layout->getChildByName("toggle_left");
        m_layout->getChildByName("toggle_right")->setTouchEnabled(false);
        left->setTouchEnabled(true);
        left->setSelectedState(false);
        this->getChildByTag(TAG_RIGHT_TEXT)->setColor(cocos2d::Color3B(199,76,9));
        this->getChildByTag(TAG_LEFT_TEXT)->setColor(cocos2d::Color3B::BLACK);
        PageView* pages=(PageView*)m_layout->getChildByName("PageView_66");
        pages->scrollToPage(1);
        //pages->removeAllPages();
        pages->cocos2d::gui::Widget::removeAllChildren();
        button_list.clear();
        pants_init();
        pages->scrollToPage(0);
    }
}

void GameShopGUILayer::trans_use_hero_id(Object *obj,TouchEventType type)
{
    if(type!=TOUCH_EVENT_ENDED)
        return;
	playEffect("music_fx_select.mp3");
    Button *button=(Button*)obj;
    int i=button->getTag();
    
    if(g_nHeroID ==i)
    {
        play=true;
		//  [2/10/2015 FishoPaul]
		if (g_pGameOverGUILayer)
		{
			g_pGameLayer->SetCurGameTag(GameTag::GAME_OVER);
			g_pGameOverGUILayer->call_restart(NULL, TOUCH_EVENT_ENDED);
			g_pGameLayer->game_over();
		}
		call_back_button(NULL, TOUCH_EVENT_ENDED);
		g_pGameLayer->SetCurGameTag(GameTag::GAME_READY);
		if (g_pGameStartGUILayer)
			g_pGameStartGUILayer->scrollToPage(g_nHeroID-1);
        return;
    }
	autostring str;
    g_nHeroID=i;
    UserDefault::sharedUserDefault()->setIntegerForKey(KEY_USED_HERO_ID,g_nHeroID);
    UserDefault::sharedUserDefault()->flush();
    tmp_use_button->setTouchEnabled(true);
    tmp_use_button->loadTextureNormal("ui_button_shop_buy_01.png",UI_TEX_TYPE_PLIST);
    tmp_use_button->setAnchorPoint(cocos2d::Point(0.5,0));
	tmp_use_button->setTitleText(str.ANSIToUTF8("使用"));
    
    //button->setTouchEnabled(false);
    button->loadTextureNormal("ui_button_shop_bought.png",UI_TEX_TYPE_PLIST);
	button->setTitleText(str.ANSIToUTF8("玩一把!"));
    tmp_use_button=button;
}

void GameShopGUILayer::call_buy_button(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;

    m_pButtonBuy = (Button*)obj;
	playEffect("music_fx_select.mp3");
    int i=atoi(m_pButtonBuy->getName())-1;
    ValueMap node=map_heroes.at(String::createWithFormat("%d",i+1)->getCString()).asValueMap();
    if(node.at("Company").asString() == "panty score")
    {
        int panty_score = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS, 0);
        int need_panty = node.at("Price").asInt();
		std::string strPayCode = node.at("Paycode").asString();
        if(panty_score < need_panty)
		{
			float fRMB = node.at("RMB").asFloat();
			g_pGameLayer->ShowPay(PAY_Buy, fRMB, strPayCode.c_str(), i);
			return;
		}

		del_pants(i+1);
		g_vecHasHeroes[i]=true;
    }
    
    UserDefault::sharedUserDefault()->flush();
	UpdateGUI(i);
}

void GameShopGUILayer::UpdateGUI(int idx)
{
	if (!m_pButtonBuy)
		return;

    m_pButtonBuy->setTouchEnabled(false);
    //action
    auto fade_out=FadeOut::create(0.4f);
    auto move_up=MoveBy::create(0.5, cocos2d::Point(0,100*RATIO));
    m_pButtonBuy->runAction(cocos2d::Spawn::create(fade_out,move_up,NULL));
    
    button_list.at(idx)->setVisible(true);
    button_list.at(idx)->setTouchEnabled(true);

	//if(idx == g_nHeroID)
	//{
	//	tmp_use_button=m_pButtonBuy;
	//	m_pButtonBuy->loadTextureNormal("ui_button_shop_bought.png",UI_TEX_TYPE_PLIST);
	//	m_pButtonBuy->setTitleText("玩一把!");
	//}
}

void GameShopGUILayer::call_exchange(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;

	playEffect("music_fx_select.mp3");
    Button *button=(Button*)obj;
    int i=button->getTag();
    g_vecHasExchange[i-1]=true;
    button->setTouchEnabled(false);
    //action
    auto fade_out=FadeOut::create(0.4f);
    auto move_up=MoveBy::create(0.5, cocos2d::Point(0,100*RATIO));
    button->runAction(cocos2d::Spawn::create(fade_out,move_up,NULL));
    add_pants(i);
    for(int i=1;i<100;i++)
    {
        UserDefault::sharedUserDefault()->setBoolForKey(String::createWithFormat(KEY_HAS_EXCHANGE,i)->getCString(), g_vecHasExchange[i-1]);
    }
    UserDefault::sharedUserDefault()->flush();
}

void GameShopGUILayer::add_pants(int i)
{
    int pants=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
    ValueMap node=map_panties.at(String::createWithFormat("%d",i)->getCString()).asValueMap();
    int _panty_score=node.at("Score").asInt();
    pants+=_panty_score;
    UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_PANTS, pants);
    UserDefault::sharedUserDefault()->flush();
    update_pants_text(pants);
}

void GameShopGUILayer::del_pants(int i)
{
    int pants=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
    ValueMap node=map_heroes.at(String::createWithFormat("%d",i)->getCString()).asValueMap();
    int _panty_score=node.at("Price").asInt();
    pants-=_panty_score;
    UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_PANTS, pants);
    UserDefault::sharedUserDefault()->flush();
    update_pants_text(pants);
}

void GameShopGUILayer::update_pants_text(int pants)
{
    value_pants->set_data(pants);
}

void GameShopGUILayer::action_fadein(Node *n)
{
    auto fadein=FadeIn::create(0.5f);
    n->setOpacity(0);
    n->runAction(fadein);
}

void GameShopGUILayer::pants_add_upadte(float dt)
{
    if(_value_pants->get_data()!=value_pants->get_data())
    {
        int i=0;
        if(_value_pants->get_data()<value_pants->get_data())
        {
            i=1;
        }else if(_value_pants->get_data()>value_pants->get_data())
        {
            i=-1;
        }

		playEffect("music_fx_kata.mp3");
        _value_pants->offset_data(i);
        auto label_pants=(LabelTTF*)this->getChildByTag(TAG_PANTIES_VAL);
        label_pants->setString(String::createWithFormat(":%d",_value_pants->get_data())->getCString());
    }
}
