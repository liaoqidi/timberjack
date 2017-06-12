//
//  GameStartGUILayer.cpp
//  PantsHero
//
//  Created by lj on 14-3-6.
//
//

#include "GameStartGUILayer.h"
#include "SimpleAudioEngine.h"
#include "global.h"
#include "GameLayer.h"
#include "GameShopGUILayer.h"
#include "GamePayGUILayer.h"

//#import <ShareSDK/ShareSDK.h>


#define TAG_PANTIES_VAL 100

GameStartGUILayer *g_pGameStartGUILayer = NULL;
int	g_nUserID = 0;

//#define __IOS__
bool GameStartGUILayer::init()
{
    bool ret=false;
    do
    {
        //load file
        m_layout=dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(file_json_startgame.c_str()));
        this->addChild(m_layout);
        
        int best_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_SCORE_MAX, 0);
        std::string best_score_text=String::createWithFormat("%d",best_score)->getCString();
        auto best=LabelTTF::create("Best:"+best_score_text, GAME_FONT_NAME, 100*RATIO);
        auto panties_icon=Sprite::createWithSpriteFrameName("ui_img_common_panty_icon.png");
        int panty_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS, 0);
        std::string score_text=String::createWithFormat("%d",panty_score)->getCString();
        auto panties_val=LabelTTF::create(":"+score_text, GAME_FONT_NAME, 100*RATIO);
        panties_val->setTag(TAG_PANTIES_VAL);
        best->setAnchorPoint(cocos2d::Point(0.5,0.5));
        panties_icon->setAnchorPoint(cocos2d::Point(1,0.5));
        panties_val->setAnchorPoint(cocos2d::Point(0,0.5));
        panties_val->setHorizontalAlignment(TextHAlignment::LEFT);
        best->setPosition(cocos2d::Point(g_size.width/2,g_size.height*0.7));
        panties_icon->setPosition(cocos2d::Point(g_size.width/2-10*RATIO,g_size.height*0.65));
        panties_val->setPosition(cocos2d::Point(g_size.width/2,g_size.height*0.65));
        
        this->addChild(best);
        this->addChild(panties_icon);
        this->addChild(panties_val);
        
#ifdef __IOS__
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_heroes_list.plist");
#else
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_heroes_list.plist");
#endif //__IOS__
        map_heroes= FileUtils::getInstance()->getValueMapFromFile(fullPath);
        
        //set button
        Button *ui_start=dynamic_cast<Button*>(m_layout->getChildByName("play_button"));
        Button *ui_leaderbord=dynamic_cast<Button*>(m_layout->getChildByName("leaderboard_button"));
        Button *ui_shop=dynamic_cast<Button*>(m_layout->getChildByName("shop_button"));
        Button *ui_buy=dynamic_cast<Button*>(m_layout->getChildByName("buy_button"));
        Button *ui_setting=dynamic_cast<Button*>(m_layout->getChildByName("seting_bg")->getChildByName("button_setting"));
        Button *button_comment=dynamic_cast<Button*>(m_layout->getChildByName("seting_bg")->getChildByName("button_comment"));
        Button *share_button=dynamic_cast<Button*>(m_layout->getChildByName("seting_bg")->getChildByName("button_share"));
        Button *ui_left_arrow=dynamic_cast<Button*>(m_layout->getChildByName("left_button"));
        Button *ui_right_arrow=dynamic_cast<Button*>(m_layout->getChildByName("right_button"));
        CheckBox *music_button=dynamic_cast<CheckBox*>(m_layout->getChildByName("seting_bg")->getChildByName("button_sound"));
        Button *ui_producer_button=dynamic_cast<Button*>(m_layout->getChildByName("seting_bg")->getChildByName("production_stuff"));
        page_view_hero=dynamic_cast<PageView*>(m_layout->getChildByName("pageview_hero"));
        ui_setting->setPosition(cocos2d::Point(g_size.width*0.1,g_size.height*-0.18));
        music_button->setSelectedState(!g_bMusicPlay);
        Button *new_tip=Button::create();
        new_tip->getTitle()->setString(String::createWithFormat("%d",g_nGotPanty)->getCString());
        new_tip->getTitle()->setFontName(GAME_FONT_NAME);
        new_tip->getTitle()->setFontSize(60*RATIO);
        new_tip->loadTextureNormal("ui_img_common_newunlock_01.png",UI_TEX_TYPE_PLIST);
        new_tip->setPosition(cocos2d::Point(80*RATIO,80*RATIO));
        ui_shop->addChild(new_tip);
        new_tip->setName("new_tip_text");
        if(g_bSeePanty || g_nGotPanty<1)
            new_tip->setVisible(false);
        ui_left_arrow->setTouchEnabled(true);
        ui_right_arrow->setTouchEnabled(true);
        m_layout->getChildByName("hero_buy_bg")->setTouchEnabled(true);
        show_setting=true;
        ui_buy->setTouchEnabled(false);
        ui_buy->setVisible(false);
        init_pageview_hero();
        
        //set listener
        ui_buy->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_buy));
        ui_start->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_start));
        ui_leaderbord->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_leaderbord));
        ui_shop->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_shop));
        ui_setting->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_setting));
        ui_left_arrow->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_scroll_left));
        ui_right_arrow->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_scroll_right));
        page_view_hero->addEventListenerPageView(this, pagevieweventselector(GameStartGUILayer::call_page_turning));
        music_button->addEventListenerCheckBox(this,checkboxselectedeventselector(GameStartGUILayer::call_music));
        button_comment->addTouchEventListener(this,toucheventselector(GameStartGUILayer::call_comment));
		share_button->addTouchEventListener(this,toucheventselector(GameStartGUILayer::call_share));
        ui_producer_button->addTouchEventListener(this, toucheventselector(GameStartGUILayer::call_view_producer));
        scheduleUpdate();
        ret=true;

		playBackgroundMusic("start.mp3",true);
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);

		//fade in
        m_layout->getChildByName("hero_buy_bg")->setOpacity(0);
        auto fadein=FadeIn::create(1);
        m_layout->getChildByName("hero_buy_bg")->runAction(fadein);
		g_nHeroID = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_USED_HERO_ID,1);
		update_play_button();
        
		int nFontSize = 60;
		char szText[256];
		sprintf(szText, "用户ID：%d", g_nUserID);
		m_pLabelUserID=LabelTTF::create(szText, GAME_FONT_NAME, nFontSize*RATIO);
		//m_pLabelUserID->setColor(cocos2d::Color3B(130,130,130));
		m_pLabelUserID->setPosition(g_size.width*0.5f,g_size.height*0.03f);
		m_pLabelUserID->setHorizontalAlignment(TextHAlignment::CENTER);
		//m_pLabelUserID->setVisible(true);
		m_pLabelUserID->setVisible(false);
		addChild(m_pLabelUserID, 2);

		// 移除相关联的选择器，因为我们将要销毁当前实例
		const char *pszGroup = "GameStartGUILayer";
		NDKHelper::RemoveSelectorsInGroup((char*)pszGroup);
		NDKHelper::AddSelector(pszGroup, "SelectorMusicOn", callfuncND_selector(GameStartGUILayer::SelectorMusicOn), this);  
		NDKHelper::AddSelector(pszGroup, "SelectorMusicOff", callfuncND_selector(GameStartGUILayer::SelectorMusicOff), this);  
		SendMessageWithParams(string("SelectorRequestMusic"), NULL);
       
    } while (0);
    return ret;
}

void GameStartGUILayer::SetUserID(int nUserID)
{
	char szText[256];
	g_nUserID = nUserID;
	sprintf(szText, "用户ID：%d", nUserID);
	m_pLabelUserID->setString(szText);
	//m_pLabelUserID->setVisible(true);
	m_pLabelUserID->setVisible(false);
}

void GameStartGUILayer::SelectorMusicOn(CCNode *sender, void *data)
{
	call_music(NULL, CHECKBOX_STATE_EVENT_UNSELECTED);
}

void GameStartGUILayer::SelectorMusicOff(CCNode *sender, void *data)
{
	call_music(NULL, CHECKBOX_STATE_EVENT_SELECTED);
}

void GameStartGUILayer::init_pageview_hero()
{
    page_view_hero=dynamic_cast<PageView*>(m_layout->getChildByName("pageview_hero"));

#ifdef __IOS__
    for(int i=1;i<=12;i++)
#else
	if (map_heroes.size() < 12)
	{
		CCLOGERROR("%s(%d): map_heroes.size() = %d", __FILE__, __LINE__, map_heroes.size());
	}
	
	for(int i=1; i <= (int)map_heroes.size(), i<=12; i++)
#endif //__IOS__

    {
        Layout *page=Layout::create();
        page->setAnchorPoint(cocos2d::Point(0,0));
        ImageView *hero_img=ImageView::create();
        hero_img->setScale(1.3f);
        hero_img->setName("hero");
        hero_img->loadTexture(String::createWithFormat("hero_%03d_01.png",i)->getCString(),UI_TEX_TYPE_PLIST);
        hero_img->setAnchorPoint(cocos2d::Point(0.5,0.5));
        hero_img->setPosition(cocos2d::Point(330*RATIO,480*RATIO));
        page->addChild(hero_img);
        
        //action move
        auto move_up=MoveTo::create(0.7f, cocos2d::Point(330*RATIO,480*RATIO+40*RATIO));
        auto move_down=MoveTo::create(0.55f, cocos2d::Point(330*RATIO,480*RATIO-40*RATIO));
        hero_img->runAction(Repeat::create(Sequence::create(move_up,move_down, NULL), -1));
        
        //text
        ValueMap node=map_heroes.at(String::createWithFormat("%d",i)->getCString()).asValueMap();
        std::string text_name=node.at("LanguageName").asString();
        auto label_text_name=Button::create();
        label_text_name->setAnchorPoint(cocos2d::Point(0.5,0));
        label_text_name->setPosition(cocos2d::Point(330*RATIO,660*RATIO));
        label_text_name->setTitleColor(Color3B::BLACK);
        label_text_name->setTitleFontName("Helvetica-Bold");
        label_text_name->setTitleText(text_name);
        label_text_name->setTitleFontSize(60*RATIO);
        label_text_name->getTitle()->setColor(cocos2d::Color3B(136,23,103));
        page->addChild(label_text_name);
        
        std::string text_1=node.at("LanguageDesc").asString();
        auto text_des=Button::create();
        text_des->setAnchorPoint(cocos2d::Point(0.5,0));
        text_des->setPosition(cocos2d::Point(310*RATIO,80*RATIO));
        text_des->setTitleFontName("Helvetica-Bold");
        text_des->setTitleText(text_1);
        text_des->setTitleFontSize(50*RATIO);
        text_des->getTitle()->setColor(cocos2d::Color3B(65,65,65));
        text_des->getTitle()->setDimensions(cocos2d::Size(500*RATIO,200*RATIO));
        text_des->getTitle()->setHorizontalAlignment(TextHAlignment::CENTER);
        page->addChild(text_des);
        
        int hp=node.at("Function").asInt()+1;
        if(hp==1)
        {
            ImageView *hp_img=ImageView::create();
            hp_img->loadTexture("ui_img_stargame_life_icon.png",UI_TEX_TYPE_PLIST);
            hp_img->setAnchorPoint(cocos2d::Point(0.5,0.5));
            hp_img->setPosition(cocos2d::Point(330*RATIO,260*RATIO));
            page->addChild(hp_img);
        }else if(hp==2)
        {
            for(int i=0;i<2;i++)
            {
                ImageView *hp_img=ImageView::create();
                hp_img->loadTexture("ui_img_stargame_life_icon.png",UI_TEX_TYPE_PLIST);
                hp_img->setAnchorPoint(cocos2d::Point(0.5,0.5));
                hp_img->setPosition(cocos2d::Point((330*RATIO-60*RATIO)+i*100*RATIO,260*RATIO));
                page->addChild(hp_img);
            }
        }else if(hp==3)
        {
            for(int i=0;i<3;i++)
            {
                ImageView *hp_img=ImageView::create();
                hp_img->loadTexture("ui_img_stargame_life_icon.png",UI_TEX_TYPE_PLIST);
                hp_img->setAnchorPoint(cocos2d::Point(0.5,0.5));
                hp_img->setPosition(cocos2d::Point(210*RATIO+i*100*RATIO,260*RATIO));
                page->addChild(hp_img);
            }
        }
            
        
        
        page_view_hero->addPage(page);
    }
}

void GameStartGUILayer::update(float dt)
{
    if(CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume()<0.5f)
    {
        CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(CocosDenshion::SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume()+0.002);
    }
    
    if(!g_bMusicPlay)
    {
        //CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    }
	else
    {
        if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
        {
			playBackgroundMusic("start.mp3");
        }
    }
    anim_time++;
    if(anim_time>anim_fps)
    {
        anim_time=0;
        int i=1;
        for(auto page :page_view_hero->getPages())
        {
            ImageView *hero_img=(ImageView*)page->getChildByName("hero");
            hero_img->loadTexture(String::createWithFormat("hero_%03d_%02d.png",i,anim_farme)->getCString(),UI_TEX_TYPE_PLIST);
            i++;
        }
        anim_farme++;
        if (anim_farme>3)
            anim_farme=1;
    }
    
    if(show_producer)
    {
        if(!_producer_view->show_producer)
        {
            _producer_view->removeFromParent();
            _producer_view=NULL;
            show_producer=false;
            set_widget_touch((Widget*)m_layout,true);
        }
    }
}

void GameStartGUILayer::call_leaderbord(Object *obj,TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");
        //[[GameKitHelper shared_gameKit_helper] show_board_leader];
    }
	if (g_pGameLayer)
		g_pGameLayer->ShowTips(NULL);
}

void GameStartGUILayer::call_start(Object *obj,TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");
        g_nHeroID = page_view_hero->getCurPageIndex()+1;
		UserDefault::sharedUserDefault()->setIntegerForKey(KEY_USED_HERO_ID, g_nHeroID);
		UserDefault::sharedUserDefault()->flush();
		close = true;
		g_pGameLayer->SetCurGameTag(GameTag::GAME_BEGAN);
    }    
}

void GameStartGUILayer::call_shop(Object *obj,TouchEventType type)
{
    if(type==TOUCH_EVENT_ENDED)
    {
		playEffect("music_fx_select.mp3");

		m_layout->getChildByName("shop_button")->getChildByName("new_tip_text")->setVisible(false);
		g_pGameLayer->ShowShop();
    }
}

void GameStartGUILayer::call_rate(Object *obj,TouchEventType type)
{
	if (g_pGameLayer)
		g_pGameLayer->ShowTips(NULL);
}

void GameStartGUILayer::call_comment(Object *obj,TouchEventType type)
{
	if (g_pGameLayer)
		g_pGameLayer->ShowTips(NULL);
}

void GameStartGUILayer::call_share(Object *obj,TouchEventType type)
{
	if (g_pGameLayer)
		g_pGameLayer->ShowTips(NULL);
    /*id<ISSContent> publishContent = [ShareSDK content:@"分享内容"
                                       defaultContent:@"默认分享内容，没内容时显示"
                                                image:nil
                                                title:@"ShareSDK"
                                                  url:@"http://www.sharesdk.cn"
                                          description:@"这是一条测试信息"
                                            mediaType:SSPublishContentMediaTypeNews];
    
    [ShareSDK showShareActionSheet:nil
                         shareList:nil
                           content:publishContent
                     statusBarTips:YES
                       authOptions:nil
                      shareOptions:nil
                            result:^(ShareType type, SSResponseState state, id<ISSPlatformShareInfo> statusInfo, id<ICMErrorInfo> error, BOOL end) {
                                if (state == SSResponseStateSuccess)
                                {
                                    NSLog(@"分享成功");
                                }
                                else if (state == SSResponseStateFail)
                                {
                                    NSLog(@"分享失败,错误码:%d,错误描述:%@", [error errorCode], [error errorDescription]);
                                }
                            }];*/
}

void GameStartGUILayer::call_scroll_left(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    
    PageView *view=(PageView*)m_layout->getChildByName("pageview_hero");
    if(view->getCurPageIndex()>0)
    {
        view->scrollToPage(view->getCurPageIndex()-1);
    }
}

void GameStartGUILayer::call_scroll_right(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    PageView *view=(PageView*)m_layout->getChildByName("pageview_hero");
    if(view->getCurPageIndex()<view->getPages().size())
    {
        view->scrollToPage(view->getCurPageIndex()+1);
    }
}

void GameStartGUILayer::scrollToPage(int nPage)
{
	PageView *view=(PageView*)m_layout->getChildByName("pageview_hero");
	if(nPage >=0 && nPage < view->getPages().size())
		view->scrollToPage(nPage);
}

void GameStartGUILayer::call_page_turning(cocos2d::Object *obj, PageViewEventType type)
{
    if(type != PageViewEventType::PAGEVIEW_EVENT_TURNING)
        return;
	playEffect("music_fx_back.mp3");
    int i=page_view_hero->getCurPageIndex();
    if(g_vecHasHeroes[i])
    {
        auto button_buy=m_layout->getChildByName("buy_button");
        button_buy->setVisible(false);
        button_buy->setTouchEnabled(false);
        
        auto button_play=m_layout->getChildByName("play_button");
        button_play->setVisible(true);
        button_play->setTouchEnabled(true);
    }else
    {
        auto button_buy=(Button*)m_layout->getChildByName("buy_button");
        button_buy->setVisible(true);
        button_buy->setTouchEnabled(true);
        
        ValueMap node=map_heroes.at(String::createWithFormat("%d",i+1)->getCString()).asValueMap();
        if(node.at("Company").asString() == "panty score")
        {
            if(button_buy->getChildByTag(2)==NULL)
            {
                auto title_icon=ImageView::create();
                title_icon->loadTexture("ui_img_common_panty_icon.png",UI_TEX_TYPE_PLIST);
                title_icon->setAnchorPoint(cocos2d::Point(1,0.3f));
                title_icon->setPosition(cocos2d::Point(-10*RATIO,50*RATIO));
                title_icon->setTag(2);
                button_buy->addChild(title_icon);
            }else
                button_buy->getChildByTag(2)->setVisible(true);
            
            std::string title_text=node.at("Price").asString();
            auto title=button_buy->getTitle();
            title->setString(":"+title_text);
            title->setFontSize(68*RATIO);
            title->setFontName(GAME_FONT_NAME);
            title->setColor(cocos2d::Color3B::BLACK);
            title->setHorizontalAlignment(TextHAlignment::LEFT);
            title->setAnchorPoint(cocos2d::Point(0,0.3f));
            title->setPosition(cocos2d::Point(0,50*RATIO));
        }else if(node.at("Company").asString() == "real money(rmb)")
        {
            if(button_buy->getChildByTag(2)!=NULL)
            {
                button_buy->getChildByTag(2)->setVisible(false);
            }
            std::string title_text=node.at("Price").asString();
            auto title=button_buy->getTitle();
            title->setString("RMB:"+title_text);
            title->setFontSize(68*RATIO);
            title->setColor(cocos2d::Color3B::BLACK);
            title->setAnchorPoint(cocos2d::Point(0.5,0));
            title->setPosition(cocos2d::Point(0,50*RATIO));
        }
        
        
        auto button_play=m_layout->getChildByName("play_button");
        button_play->setVisible(false);
        button_play->setTouchEnabled(false);
    }
}

void GameStartGUILayer::call_music(Object *obj,CheckBoxEventType type)
{
    if(type==CHECKBOX_STATE_EVENT_SELECTED)
    {
        g_bMusicPlay=false;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
    }
	else
    {
        g_bMusicPlay=true;
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
    }
    
    UserDefault::sharedUserDefault()->setBoolForKey("g_bMusicPlay", g_bMusicPlay);
    UserDefault::sharedUserDefault()->flush();
}

void GameStartGUILayer::call_buy(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    
	playEffect("music_fx_select.mp3");
    int i=page_view_hero->getCurPageIndex();
    ValueMap node=map_heroes.at(String::createWithFormat("%d",i+1)->getCString()).asValueMap();
    if(node.at("Company").asString() == "panty score")
    {
        int panty_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS, 0);
        int need_panty=node.at("Price").asInt();
		std::string strPayCode = node.at("Paycode").asString();
		if(panty_score < need_panty)
		{
			int nRMB = node.at("RMB").asInt();
			g_pGameLayer->ShowPay(PAY_Buy, nRMB, strPayCode.c_str(), i);
			return;
		}

		panty_score-=need_panty;
		g_nHeroID = i;
		g_vecHasHeroes[i]=true;
		UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_PANTS, panty_score);
		UserDefault::sharedUserDefault()->setIntegerForKey(String::createWithFormat("has_hero_%d",i+1)->getCString(), true);
		UserDefault::sharedUserDefault()->setIntegerForKey(KEY_USED_HERO_ID, g_nHeroID);
		UserDefault::sharedUserDefault()->flush();
		update_play_button();
    }
	else if(node.at("Company").asString() == "real money(rmb)")
    {
       
    }
}

void GameStartGUILayer::call_setting(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TouchEventType::TOUCH_EVENT_ENDED)
        return;
    
	playEffect("music_fx_select.mp3");
    if(!show_setting)
    {
        show_setting=true;
        m_layout->getChildByName("seting_bg")->getChildByName("button_setting")->setTouchEnabled(false);
        MoveTo *move_to=MoveTo::create(0.6f, cocos2d::Point(g_size.width*0.1f,g_size.height*-0.18f));
        CallFunc * func = CallFunc::create(CC_CALLBACK_0(GameStartGUILayer::action_move_setting,this));
        m_layout->getChildByName("seting_bg")->runAction(CCSequence::create(move_to,
                                                                            func,
                                                                            NULL));
    }else
    {
        show_setting=false;
        m_layout->getChildByName("seting_bg")->getChildByName("button_setting")->setTouchEnabled(false);
        MoveTo *move_to=MoveTo::create(0.6f, cocos2d::Point(g_size.width*0.1f,g_size.height*0.18f));
        CallFunc * func = CallFunc::create(CC_CALLBACK_0(GameStartGUILayer::action_move_setting,this));
        m_layout->getChildByName("seting_bg")->runAction(CCSequence::create(move_to,
                                                                            func,
                                                                            NULL));
    }
}

void GameStartGUILayer::call_view_producer(cocos2d::Object *obj, TouchEventType type)
{
    if(type != TOUCH_EVENT_ENDED && !show_producer)
        return;
    _producer_view=ProducerView::create();
    this->addChild(_producer_view,20);
    _producer_view->setAnchorPoint(cocos2d::Point(0,0));
    _producer_view->setPosition(cocos2d::Point(0,0));
    show_producer=true;
   	set_widget_touch((Widget*)m_layout,false);

	SendMessageWithParams(string("SelectorGetUserID"), NULL);

}

void GameStartGUILayer::update_panty_score()
{
    int panty_score=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS, 0);
    std::string score_text=String::createWithFormat("%d",panty_score)->getCString();
    auto panties_val=(LabelTTF*)this->getChildByTag(TAG_PANTIES_VAL);
	if (panties_val)
		panties_val->setString(":"+score_text);
}

void GameStartGUILayer::set_widget_touch(Widget *w,bool tg)
{
    w->setTouchEnabled(tg);
    for(auto _w : w->getChildren())
    {
        set_widget_touch((cocos2d::gui::Widget*)_w, tg);
    }
}

void GameStartGUILayer::update_play_button()
{
    auto button_buy=m_layout->getChildByName("buy_button");
	if (button_buy)
	{
		button_buy->setVisible(false);
		button_buy->setTouchEnabled(false);
	}
    
    auto button_play=m_layout->getChildByName("play_button");
	if (button_play)
	{
		button_play->setVisible(true);
		button_play->setTouchEnabled(true);
	}
    update_panty_score();

	scrollToPage(g_nHeroID - 1);
}

void GameStartGUILayer::action_move_setting()
{
    m_layout->getChildByName("seting_bg")->getChildByName("button_setting")->setTouchEnabled(true);
}