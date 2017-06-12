//
//  GameLayer.cpp
//  胖次英雄
//
//  Created by lyon on 14-3-3.
//
//
#include "GameLayer.h"
#include "global.h"
#include "PantsList.h"
#include "SimpleAudioEngine.h"
#include "GamePayGUILayer.h"
#include "GameOverGUILayer.h"
#include "GameStartGUILayer.h"
#include "GameShopGUILayer.h"
#include "NDKHelper.h"

//#import "iAdHelper.h"
//using namespace CocosDenshion;
//#define __IOS__

//#define FREE_GAME

enum UI_TAG
{
	TAG_HERO,
	TAG_OBSTACLE,
	TAG_PANTS,
	TAG_SCORE,
	TAG_NUL,
	TAG_OVER_LAYER,
	TAG_WARN_FINGER,
	TAG_WARN_DOWN,
	TAG_WARN_TEXT,
	TAG_READY,
};

#define TIMER_DT   0.0001

#ifdef IPHONE_5
#define ADD_LIMIT_MIN       1
#define ADD_LIMIT_MAX       2
#define BASE_POINT_Y        4
#else
#define ADD_LIMIT_MIN       1
#define ADD_LIMIT_MAX       2
#define BASE_POINT_Y        12
#endif

GameLayer *g_pGameLayer = NULL;


GameLayer::GameLayer()
{
	m_nSoreGift			= 0;
    m_ScoreDataValue	= new DataValue(0);
    m_ScoreMaxDataValue	= new DataValue(0);
    m_PantsDataValue	= new DataValue(0);
    m_pHero=NULL;
    m_fTime=0;
    m_bShow=false;
    m_bShowWarn=false;
    m_bPlayDead=false;
    m_bShowShop=false;
    m_bShowShake=false;
    m_bShowStart=false;
	//paly_dead_time=0;
    m_fShakeTimer=1;
    m_fShakeAmplitude=20;
	m_curGameTag=GameTag::GAME_BEGAN;
	m_oldGameTag=GameTag::GAME_BEGAN;
    speed_hero=0;
    speed_hero_up=26;
    speed_gravity=1;
    speed_background=2;
    speed_obstacle=11;
    speed_tmp_1=0;
    speed_tmp_2=0;
    m_nGameOverTimer=40;
    m_bPlayOver	= false;
	m_bQuitGame	= false;
    m_fObstaclCreateTimer=115;
}

GameLayer::~GameLayer()
{
    
}

void GameLayer::create_hero()
{
    if(m_pHero!=NULL)
    {
        m_pHero->removeFromParent();
        m_pHero=NULL;
    }
    m_pHero=Hero::create(g_nHeroID);
    this->addChild(m_pHero, LAYER_HERO);
    m_pHero->setTag(TAG_HERO);
    m_pHero->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
    m_pHero->setPosition(cocos2d::Point(m_SceneSize.width/4, m_SceneSize.height/2));
#ifdef __IOS__
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_heroes_list.plist");
#else
	std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_heroes_list.plist");
#endif //__IOS__
    ValueMap map_heroes= FileUtils::getInstance()->getValueMapFromFile(fullPath);
    ValueMap node=map_heroes.at(String::createWithFormat("%d",g_nHeroID)->getCString()).asValueMap();
    g_nHeroHP=node.at("Function").asInt()+1;
    m_pHero->time_capsule=g_nHeroHP;
    for(auto _node : m_vecLifeIcons)
    {
        _node->removeFromParent();
    }
    m_vecLifeIcons.clear();
    create_life_icon();
}

bool GameLayer::init()
{
    bool ret=false;
//#ifdef UMENG
//	DebugLog("MOBCLICKCPP_START_WITH_APPKEY 111");
//	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("568cf5a267e58e1a370022dd", "MiniPi");
//	DebugLog("MOBCLICKCPP_START_WITH_APPKEY 2222");
//#endif
	DebugLog("path=%s", UserDefault::sharedUserDefault()->getXMLFilePath().c_str());

	do
    {
		//对手机键盘监听
		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyReleased = CC_CALLBACK_2(GameLayer::onKeyReleased,this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener,this);

#ifdef __IOS__
        std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_panty_list.plist");
#else
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_panty_list.plist");
#endif //__IOS__
        map_panty= FileUtils::getInstance()->getValueMapFromFile(fullPath);
        
        g_size=Director::getInstance()->getVisibleSize();
        m_SceneSize=Director::getInstance()->getVisibleSize();
        m_SceneOrigin=Director::getInstance()->getVisibleOrigin();
        g_nHeroID=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_USED_HERO_ID,1);
        load_user_data();
        
        g_pGameStartGUILayer=GameStartGUILayer::create();
        this->addChild(g_pGameStartGUILayer,10);
        m_bShowStart=true;
        
        //base background
        auto background_0=Sprite::createWithSpriteFrameName(NAME_BACKGROUND);
        this->addChild(background_0,LAYER_BACKGROUND_0);
        background_0->setAnchorPoint(cocos2d::Point(0,0));
        background_0->setPosition(cocos2d::Point(0,0));
        
        //m_pHero
        m_pHero=Hero::create(g_nHeroID);
        this->addChild(m_pHero, LAYER_HERO);
        m_pHero->setTag(TAG_HERO);
        m_pHero->setAnchorPoint(cocos2d::Point(0.5f, 0.5f));
        m_pHero->setPosition(cocos2d::Point(m_SceneSize.width/2, m_SceneSize.height*0.65));
        
        //1 background
#ifdef __BACKGROUND_HOUSE__
		m_pBackground_1_0=Sprite::createWithSpriteFrameName(NAME_BACKGROUND_1_0);
		m_pBackground_1_0->setAnchorPoint(cocos2d::Point(0,0));
		m_pBackground_1_0->setPosition(cocos2d::Point(0,0));
		m_pBackground_1_0->getTexture()->setAliasTexParameters();
        this->addChild(m_pBackground_1_0,LAYER_BACKGROUND_1);

		m_pBackground_1_1=Sprite::createWithSpriteFrameName(NAME_BACKGROUND_1_1);
		m_pBackground_1_1->setAnchorPoint(cocos2d::Point(0,0));
		m_pBackground_1_1->setPosition(cocos2d::Point(g_size.width-1,0));
		m_pBackground_1_1->getTexture()->setAliasTexParameters();
        
        this->addChild(m_pBackground_1_1,LAYER_BACKGROUND_1);
#endif //__BACKGROUND_HOUSE__
        
        //foreground
        m_pForeground_0=Sprite::createWithSpriteFrameName("scene_building_1_0.png");
        m_pForeground_1=Sprite::createWithSpriteFrameName("scene_building_1_0.png");
        m_pForeground_0->setAnchorPoint(cocos2d::Point(0,0));
        m_pForeground_1->setAnchorPoint(cocos2d::Point(0,0));
        m_pForeground_0->setPosition(cocos2d::Point(0,-100));
        m_pForeground_1->setPosition(cocos2d::Point(g_size.width,-100));
        m_pForeground_0->getTexture()->setAliasTexParameters();
        m_pForeground_1->getTexture()->setAliasTexParameters();
        this->addChild(m_pForeground_0,4);
        this->addChild(m_pForeground_1,4);
        
        //score
        auto ui_score=LabelTTF::create("0", GAME_FONT_NAME, 300*RATIO);
        ui_score->setAnchorPoint(cocos2d::Point(0.5,0));
        ui_score->setPosition(g_size.width*0.5,g_size.height*0.85);
        ui_score->setHorizontalAlignment(TextHAlignment::RIGHT);
        ui_score->setTag(TAG_SCORE);
        this->addChild(ui_score,LAYER_HERO);
        
        //warn
        ready_view=dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(file_json_getready.c_str()));
        ready_view->setVisible(false);
        this->addChild(ready_view);
        
        // update
        scheduleUpdate();
        
        //m_pObstacle
        m_pObstacle=Obstacle::create();
        m_pObstacle->setAnchorPoint(cocos2d::Point(0,0));
        this->addChild(m_pObstacle);
        m_pObstacle->speed=speed_obstacle;
        m_pObstacle->update_score=std::bind(&GameLayer::update_score,this);
        
        // touch
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        auto listener = EventListenerTouchAllAtOnce::create();
        listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
        listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
        dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        
        init_user_data();
        
        m_pHero->setVisible(false);
		m_curGameTag = GameTag::GAME_BEGAN;
        ui_score->setVisible(false);

		int nFontSize = 150;
		m_pLabelTips = LabelTTF::create("该功能暂未开放", GAME_FONT_NAME, nFontSize*RATIO);
		m_pLabelTips->setColor(cocos2d::Color3B(255,0,0));
		m_pLabelTips->setPosition(g_size.width*0.5f, g_size.height*0.1f);
		m_pLabelTips->setHorizontalAlignment(TextHAlignment::CENTER);
		m_pLabelTips->setVisible(false);
		addChild(m_pLabelTips, 50);

		// 移除相关联的选择器，因为我们将要销毁当前实例
		const char *pszGroup = "GameLayer";
		NDKHelper::RemoveSelectorsInGroup((char*)pszGroup);
		NDKHelper::AddSelector(pszGroup, "SelectorGetUserID", callfuncND_selector(GameLayer::SelectorGetUserID), this);
		SendMessageWithParams(string("SelectorRequestUserID"), NULL);

#ifdef _WIN32
		ShowPay(PAY_Gift);//除了PAY_Buy外，其他数值是内部赋值
#endif // _WIN32
		//ShowPayHero10();
        
        ret=true;
    } while (0);

	return ret;
}

extern int m_IsNormalVer;
extern int m_nMaxMoney;
extern int m_nTodayMoney;

void GameLayer::SelectorGetUserID(CCNode *sender, void *data)
{
	CCDictionary *pDict = (CCDictionary*)data;
	if (pDict)
	{
		const __String *pValue = pDict->valueForKey("UserID");
		int nUserID = pValue->intValue();
		if (g_pGameStartGUILayer)
		{
			g_pGameStartGUILayer->SetUserID(nUserID);
		}

		//pValue = pDict->valueForKey("Source");
		//g_nSource = pValue->intValue();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		const __String *pValueChannel = pDict->valueForKey("ChannelName");
		const char *pszChannelName = pValueChannel->getCString();

		const __String *pValuePhoneNumber = pDict->valueForKey("PhoneNumber");
		const char *pszPhoneNumber = pValuePhoneNumber->getCString();

//#ifdef UMENG
//		MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("568cf5a267e58e1a370022dd", pszChannelName);
//		DebugLog("APPKEY=%s", "568cf5a267e58e1a370022dd");
//		DebugLog("ChannelName=%s", pszChannelName);
//#endif //UMENG

#endif //CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

		pValue = pDict->valueForKey("IsNormalVer");
		m_IsNormalVer = pValue->intValue();
		pValue = pDict->valueForKey("nMaxMoney");
		m_nMaxMoney = pValue->intValue();
		pValue = pDict->valueForKey("nTodayMoney");
		m_nTodayMoney = pValue->intValue();
	}
#ifndef _WIN32
	ShowPay(PAY_Gift);//除了PAY_Buy外，其他数值是内部赋值
#endif // _WIN32
}

void GameLayer::ShowTips(char *pszText)
{
	if (pszText)
		m_pLabelTips->setString(pszText);
	else
		m_pLabelTips->setString("该功能暂未开放");

	m_pLabelTips->setVisible(true);
}


void GameLayer::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
{
	if (keyCode == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE
		|| keyCode == cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		CCLog("m_curGameTag=%d\n", m_curGameTag);
		switch (m_curGameTag)
		{
		case GameTag::GAME_PLAYING:
			break;
		case GameTag::GAME_OVER:
			g_pGameOverGUILayer->call_back(NULL, TOUCH_EVENT_ENDED);
			break;
		case GameTag::GAME_BEGAN:
			//if (g_pGameStartGUILayer->m_bShowShop)
			//	g_pGameStartGUILayer->shop_layer->m_bClose = true;
			//else
			//	g_pGameStartGUILayer->call_start(NULL, TOUCH_EVENT_ENDED);
			m_bQuitGame = true;
			ShowPay(PAY_Gift);//除了PAY_Buy外，其他数值是内部赋值

#ifdef FREE_GAME
//#ifdef UMENG
//			umeng::MobClickCpp::end();
//#endif
			Director::getInstance()->end();
			return;
#endif
			break;
		case GameTag::GAME_READY:
			m_curGameTag = GameTag::GAME_BEGAN;
			break;
		case GameTag::GAME_SHOP:
			g_pGameShopGUILayer->call_back_button(NULL, TOUCH_EVENT_ENDED);
			break;
		case GameTag::GAME_PAY:
//#ifdef NORMAL_VER_X
			g_pGamePayGUILayer->onBtnCancel(NULL, TOUCH_EVENT_ENDED);
//#endif	//NORMAL_VER_X
			break;
		case GameTag::GAME_TIME_BACK:
			break;
		case GameTag::HERO_PLAYDEAD:
			break;
		default:
			break;
		}
	}
}

void GameLayer::load_user_data()
{
    g_bMusicPlay=UserDefault::sharedUserDefault()->getBoolForKey("g_bMusicPlay", true);
    g_nGotPanty=UserDefault::sharedUserDefault()->getIntegerForKey("g_nGotPanty",0);
    g_bSeePanty=UserDefault::sharedUserDefault()->getBoolForKey("g_bSeePanty", true);
    
    for(int i=1;i<=100;i++)
    {
        g_vecHasHeroes.push_back(UserDefault::sharedUserDefault()->getBoolForKey(String::createWithFormat(KEY_HAS_HERO,i)->getCString(),false));
        if(i==1)
            g_vecHasHeroes[0]=true;
    }
    
    for(int i=1;i<=100;i++)
    {
        g_vecHasPanties.push_back(UserDefault::sharedUserDefault()->getBoolForKey(String::createWithFormat(KEY_HAS_PANTY,i)->getCString(),false));
    }
    
    for(int i=1;i<100;i++)
    {
        g_vecHasExchange.push_back(UserDefault::sharedUserDefault()->getBoolForKey(String::createWithFormat(KEY_HAS_EXCHANGE,i)->getCString(),false));
    }
}

void GameLayer::save_user_data()
{
    for(int i=1;i<=12;i++)
    {
        UserDefault::sharedUserDefault()->setBoolForKey(String::createWithFormat(KEY_HAS_HERO,i)->getCString(), g_vecHasHeroes[i-1]);
    }
    
    for(int i=1;i<=100;i++)
    {
        UserDefault::sharedUserDefault()->setBoolForKey(String::createWithFormat(KEY_HAS_PANTY,i)->getCString(), g_vecHasPanties[i-1]);
    }
    
    for(int i=1;i<100;i++)
    {
        UserDefault::sharedUserDefault()->setBoolForKey(String::createWithFormat(KEY_HAS_EXCHANGE,i)->getCString(), g_vecHasExchange[i-1]);
    }
    
    UserDefault::sharedUserDefault()->setBoolForKey("g_bSeePanty", g_bSeePanty);
    UserDefault::sharedUserDefault()->setIntegerForKey("g_nGotPanty", g_nGotPanty);
    
    UserDefault::sharedUserDefault()->flush();
}

void GameLayer::init_user_data()
{
    m_PantsDataValue->set_data(0);
	m_ScoreDataValue->set_data(0);
    int max=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_SCORE_MAX,0);
	m_ScoreMaxDataValue->set_data(max);
}

void GameLayer::sender()
{
	int score_vl=m_ScoreDataValue->get_data();
    bool get_panty=false;
    if(score_vl>4 && score_vl<=20)
    {
        if(CCRANDOM_0_1()>0.5)
        {
            if(CCRANDOM_0_1()>0.2)
            {
                int count=CCRANDOM_0_1()*(26-1)+1;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count-1]=true;
                }
            }else
            {
                int count=CCRANDOM_0_1()*(45-26)+26;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count-1]=true;
                }
            }
        }
    }else if(score_vl>20 && score_vl<=60)
    {
        if(CCRANDOM_0_1()>0.6)
        {
            float a=CCRANDOM_0_1();
            if(a<0.5)
            {
                int count=CCRANDOM_0_1()*(26-1)+1;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count]=true;
                }
            }else if(a>=0.5 && a<0.8)
            {
                int count=CCRANDOM_0_1()*(45-26)+26;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count]=true;
                }
            }else
            {
                int count=CCRANDOM_0_1()*(57-45)+45;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count]=true;
                }
            }
        }
    }else if(score_vl>60)
    {
        if(CCRANDOM_0_1()>0.6)
        {
            float a=CCRANDOM_0_1();
            if(a<0.3)
            {
                int count=CCRANDOM_0_1()*(26-1)+1;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count-1]=true;

                }
            }else if(a>=0.3 && a<0.7)
            {
                int count=CCRANDOM_0_1()*(45-26)+26;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count]=true;
                }
            }else
            {
                int count=CCRANDOM_0_1()*(57-45)+45;
                if(!g_vecHasPanties[count-1])
                {
                    get_panty=true;
                    g_vecHasPanties[count-1]=true;
                }
            }
        }
    }
    
    if(get_panty)
    {
		if (g_pGameOverGUILayer)
			g_pGameOverGUILayer->get_panty=true;
        g_bSeePanty=false;
        //break
    }
    
	int _score=m_ScoreDataValue->get_data();
    
	if (g_pGameOverGUILayer)
	{
		g_pGameOverGUILayer->m_PantsDataValue->set_data(UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS,0)+_score);
		g_pGameOverGUILayer->m_ScoreDataValue->set_data(_score);
		g_pGameOverGUILayer->m_ScoreMaxDataValue->set_data(m_ScoreMaxDataValue->get_data());
	}
    _score+=UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
    UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_PANTS,_score);
    
	if(m_ScoreMaxDataValue->get_data()<m_ScoreDataValue->get_data())
    {
		m_ScoreMaxDataValue->set_data(m_ScoreDataValue->get_data());
        UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_SCORE_MAX, m_ScoreMaxDataValue->get_data());
        
        //[[GameKitHelper shared_gameKit_helper] upload:score_max->get_data() forCategory:@"1234"];
		if (g_pGameOverGUILayer)
			g_pGameOverGUILayer->get_new_best=true;
    }
    save_user_data();
    UserDefault::sharedUserDefault()->flush();
	if (g_pGameOverGUILayer)
		g_pGameOverGUILayer->update_score();
}

void GameLayer::restart()
{
    m_fTime=0;
    m_bPlayDead=false;
    init_user_data();
    speed_hero=0;
    m_pHero->setPosition(cocos2d::Point(m_SceneSize.width/4, m_SceneSize.height/2));
	m_ScoreDataValue->set_data(-1);
    m_pHero->rotation=0;
    m_pHero->setRotation(m_pHero->rotation);
    update_score();
    m_bShow=false;
}

void GameLayer::obstacle_update()
{
    m_fTime++;
    if(m_fTime>=m_fObstaclCreateTimer)
    {
        m_fTime=0;
        add_obstacle();
    }
    int i=0;
    for(auto &a : list)
    {
        if(a->getPositionX()<-500)
        {
            a->removeFromParent();
            list.erase(i);
        }
        i++;
    }
    
    i=0;
    for(auto &a : score_list)
    {
        if(a->getPositionX()<=m_SceneSize.width/4)
        {
            a->removeFromParent();
            update_score();
            score_list.erase(i);
        }
        i++;
    }
    
    //m_pObstacle move
    for(auto &a : list)
    {
        a->setPositionX(a->getPositionX()-speed_obstacle);
    }
    //score node move
    for(auto &a : score_list)
    {
        a->setPositionX(a->getPositionX()-speed_obstacle);
    }
}

void GameLayer::update(float dt)
{
    game_began();
    game_ready();
    game_playing();
    game_playdead();
    game_over();
    game_timeBack();
    action_shake(dt);
	if (g_pGamePayGUILayer)
		g_pGamePayGUILayer->update(dt);

	static int s_nTipsTime = 0;
	if (m_pLabelTips && m_pLabelTips->isVisible())
	{
		if (++s_nTipsTime > 30)
		{
			s_nTipsTime = 0;
			m_pLabelTips->setVisible(false);
		}
	}
}

void GameLayer::action_shake(float dt)
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
        speed_tmp_2=0;
    }
}

void GameLayer::game_began()
{
    if(m_curGameTag!=GameTag::GAME_BEGAN)
        return;
    
    if(!m_bShowStart)
    {
        g_pGameStartGUILayer=GameStartGUILayer::create();
        this->addChild(g_pGameStartGUILayer,10);
        m_bShowStart=true;
	}
    
    if(m_bShowStart && g_pGameStartGUILayer && g_pGameStartGUILayer->close)
    {
        g_pGameStartGUILayer->removeFromParent();
		g_pGameStartGUILayer = NULL;
        m_pHero->setPosition(m_SceneSize.width/4,m_SceneSize.height/2);
        this->getChildByTag(TAG_SCORE)->setVisible(false);
        m_curGameTag = GameTag::GAME_READY;
        m_bShowStart=false;
    }
    
    //background move
    action_move_background();
}

void GameLayer::game_ready()
{
    if(m_curGameTag!=GameTag::GAME_READY)
        return;
    //[iAdHelper remove_iAdHelper];
	if (g_bMusicPlay)
	{
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	}
    if(!m_bShowWarn)
    {
        create_hero();
        m_bShowWarn=true;
        ready_view->setVisible(true);
        m_pHero->fly_tag=FlyTag::FLY_UP;
    }
    action_move_background();
    
    if(m_pHero->getPositionY()<m_SceneSize.height/2+30 && m_pHero->fly_tag==FlyTag::FLY_UP)
    {
        m_pHero->setPositionY(m_pHero->getPositionY()+2*RATIO);
        if(m_pHero->getPositionY()>=m_SceneSize.height/2+30*RATIO)
        {
            m_pHero->fly_tag=FlyTag::FLY_DOWN;
        }
    }else if(m_pHero->getPositionY()>m_SceneSize.height/2-50*RATIO && m_pHero->fly_tag==FlyTag::FLY_DOWN)
    {
        m_pHero->setPositionY(m_pHero->getPositionY()-3*RATIO);
        if(m_pHero->getPositionY()<=m_SceneSize.height/2-30*RATIO)
        {
            m_pHero->fly_tag=FlyTag::FLY_UP;
        }
    }
    for(auto _node : m_vecLifeIcons)
    {
        _node->setVisible(false);
    }
    
    //finger tip
    
    
}

void GameLayer::game_playing()
{
    if(m_curGameTag!=GameTag::GAME_PLAYING)
        return;

	int nSore = m_ScoreDataValue->get_data();
	if (nSore == 0)
		m_nSoreGift = 0;
	//if (m_nSoreGift != nSore && nSore > 0 && nSore % 5 == 0)
	if (m_nSoreGift != nSore && nSore > 0 && nSore == 10)
	{
		m_nSoreGift = nSore;
		ShowPay(PAY_Gift);//除了PAY_Buy外，其他数值是内部赋值
		return;
	}
    
    if(!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
    {
		playBackgroundMusic("superpanty.mp3");
    }
    m_bPlayOver=false;
    //obstacle_update;
    m_pObstacle->update(m_ScoreDataValue->get_data());
    //obstacle_update();
    
    //ground move
    action_move_background();
    
    cocos2d::Rect _hero_rect=cocos2d::Rect(m_pHero->getPositionX()-(HERO_RECT_W*RATIO)/2,
                                           m_pHero->getPositionY()-(HERO_RECT_H*RATIO)/2,
                                           (HERO_RECT_W*RATIO),
                                           (HERO_RECT_H*RATIO));
    for(auto &spt : list)
    {
        if(_hero_rect.intersectsRect(spt->getBoundingBox()))
        {
            if(spt->getTag()==TAG_OBSTACLE)
            {
                //CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
				CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                m_pHero->hit_point=m_pHero->getPosition();
                m_pHero->last_hit_speed=speed_hero;
                m_pHero->last_hit_speed_tmp=speed_tmp_1;
                m_pHero->last_hit_rotation=m_pHero->rotation;
                m_pHero->last_hit_state=m_pHero->fly_tag;
                m_bShowShake=true;
				playEffect("music_fx_impact.mp3");
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
				CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

				m_curGameTag = GameTag::HERO_PLAYDEAD;
                speed_tmp_1=0;
                speed_tmp_2=0;
                speed_hero=5;
            }
			else if(spt->getTag()==TAG_PANTS)
            {
                m_PantsDataValue->offset_data(1);
                spt->setTag(TAG_NUL);
                spt->removeFromParent();
            }
            
        }
    }
    
    ObstacleTag hitTag=ObstacleTag::NUL;
    hitTag=m_pObstacle->is_hit(_hero_rect);
    if(hitTag==ObstacleTag::OBSTACLE)
    {
        m_bShowShake=true;
        
#ifdef __IOS__
		Sprite *boom=Sprite::create("boom.png");
#else
		Sprite *boom=Sprite::create("ipad/boom.png");
#endif //__IOS__
        boom->setAnchorPoint(cocos2d::Point(0.5,0.5));
        boom->setPosition(m_pHero->getPosition());
        this->addChild(boom);
        boom->setScale(0);
        
        ScaleTo* scale_to=ScaleTo::create(0.1f, 1.2f);
        ScaleTo* scale_back=ScaleTo::create(0.05f, 0.85f);
        FadeOut* fade_out=FadeOut::create(0.3f);
        
        //action_remove_node
        boom->runAction(cocos2d::Sequence::create(scale_to,
                                                  scale_back,
                                                  fade_out,
                                                  CallFuncN::create(CC_CALLBACK_1(GameLayer::action_remove_node, this,boom)),
                                                  NULL));
        
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

		playEffect("music_fx_drop_1.mp3");
        m_pHero->hit_point=m_pHero->getPosition();
        m_pHero->last_hit_speed=speed_hero;
        m_pHero->last_hit_speed_tmp=speed_tmp_1;
        m_pHero->last_hit_rotation=m_pHero->rotation;
        m_pHero->last_hit_state=m_pHero->fly_tag;
        update_life_icon();
        m_curGameTag = GameTag::HERO_PLAYDEAD;
        //m_curGameTag = GameTag::GAME_OVER;//  [4/3/2014 Fisho]
        speed_tmp_1=0;
        speed_tmp_2=0;
        speed_hero=5;
    }
	else if(hitTag==ObstacleTag::PANTY)
    {
        m_PantsDataValue->offset_data(1);
    }
    
    
    if(m_pHero->fly_tag==FlyTag::FLY_UP)
    {
        m_pHero->action_up();
    }
    
    if(m_pHero->getPositionY()>0)
    {
        if(speed_tmp_1>0.18)
        {
            m_pHero->fly_tag=FlyTag::FLY_DOWN;
            m_pHero->action_down();
        }
        speed_tmp_1+=0.005f;
        speed_hero-=(speed_gravity+speed_tmp_1);
        m_pHero->setPositionY(m_pHero->getPositionY() + speed_hero*RATIO);
    }
	else
    {
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();		
		playEffect("music_fx_drop_1.mp3");
        update_life_icon();
        if(m_pHero->time_capsule>1)
        {
            m_pHero->time_capsule--;
            //m_pHero->is_time_back_hit_point=false;
            //speed_hero=-50;
            //speed_tmp_1=0.5;
            //speed_backtime=3.5;
            //m_curGameTag = GameTag::GAME_TIME_BACK;
            //m_bPlayDead=false;
			SetTimeBack();
        }
		else
        {
            m_curGameTag = GameTag::GAME_OVER;
            m_bPlayDead=false;
			ShowPayRelive();
        }
    }
}

void GameLayer::game_playdead()
{
    if(m_curGameTag!=GameTag::HERO_PLAYDEAD)
        return;
    
    action_move_background();
    m_pObstacle->update(m_ScoreDataValue->get_data());
    
    for(auto &a : list)
    {
        a->setPositionX(a->getPositionX()-speed_obstacle*RATIO);
    }
    
    if(m_bPlayDead)
    {
        if(m_pHero->getPositionY()>-50)
        {
            if(m_pHero->rotation<90)
            {
                m_pHero->rotation+=2.5;
                m_pHero->setRotation(m_pHero->rotation);
            }
            speed_tmp_1+=0.02f;
            if(speed_tmp_1>1)
                speed_tmp_2+=0.005f;
            speed_hero-=(speed_gravity+speed_tmp_1+speed_tmp_2);
            m_pHero->setPositionY(m_pHero->getPositionY() + speed_hero*RATIO);
        }
		else
        {
            if(m_pHero->time_capsule>1)
            {
                m_pHero->time_capsule--;
                //m_pHero->is_time_back_hit_point=false;
                //speed_hero=-50;
                //speed_tmp_1=0.5;
                //speed_backtime=3.5;
                //m_curGameTag = GameTag::GAME_TIME_BACK;
                //m_bPlayDead=false;
				SetTimeBack();
            }
			else
            {
                m_curGameTag = GameTag::GAME_OVER;
                m_bPlayDead=false;
				ShowPayRelive();
            }
        }
    }
	else
    {
        //if(paly_dead_time<0)
        //{
        //    paly_dead_time++;
        //}else
        //{
            m_bPlayDead=true;
            //paly_dead_time=0;
        //}
    }
}

void GameLayer::SetTimeBack()
{
	m_pHero->is_time_back_hit_point=false;
	speed_hero=-50;
	speed_tmp_1=0.5;
	speed_backtime=3.5;
	m_curGameTag = GameTag::GAME_TIME_BACK;
	m_bPlayDead=false;
}

void GameLayer::game_over()
{
    if(m_curGameTag!=GameTag::GAME_OVER)
        return;

    if(!m_bPlayOver)
    {
        m_nGameOverTimer-=1;
        if(m_nGameOverTimer<0)
        {
            m_bPlayOver=true;
            m_nGameOverTimer=40;
            return;
        }
    }
    
    
    if(!m_bShow)
    {
        m_pHero->setVisible(false);
        for(auto &spt : list)
        {
            spt->removeFromParent();
        }
        for(auto &sc : score_list)
        {
            sc->removeFromParent();
        }
        m_pObstacle->del_obstacle();
        list.clear();
        score_list.clear();
        m_bShow=true;
        g_pGameOverGUILayer=GameOverGUILayer::create();
        sender();
        this->getChildByTag(TAG_SCORE)->setVisible(false);
        for(auto _node : m_vecLifeIcons)
        {
            _node->setVisible(false);
        }
        g_pGameOverGUILayer->setTag(TAG_OVER_LAYER);
        this->addChild(g_pGameOverGUILayer,5);
    }
	else
    {
        if (g_pGameOverGUILayer && g_pGameOverGUILayer->close)
        {
            this->getChildByTag(TAG_OVER_LAYER)->removeFromParent();
            m_curGameTag = GameTag::GAME_READY;
            restart();
            m_pHero->setVisible(true);
        }
        else if (g_pGameOverGUILayer && g_pGameOverGUILayer->back)
        {
            this->getChildByTag(TAG_OVER_LAYER)->removeFromParent();
            m_curGameTag = GameTag::GAME_BEGAN;
            restart();
        }
    }

}

void GameLayer::game_timeBack()
{
    if(m_curGameTag != GameTag::GAME_TIME_BACK)
        return;
    action_moveBack_background(speed_backtime);
    m_pObstacle->update_back(speed_backtime*1.5);
    
    speed_tmp_1 += 0.005f;
    m_pHero->action_up();
    speed_hero+=speed_tmp_1;
    m_pHero->setPositionY(m_pHero->getPositionY()-speed_hero*RATIO);
    
    if(m_pHero->getPositionY()>=g_size.height*0.4 && speed_backtime>0)
    {
        speed_backtime -= 0.1f;
        if(speed_backtime<0)
        {
            speed_backtime=0;
        }
    }
    
    if(m_pHero->getPositionY()>=g_size.height*0.7)
    {
        m_pObstacle->del_obstacle();
        speed_hero=0;
        speed_tmp_1=0;
        m_curGameTag = GameTag::GAME_PLAYING;
    }
}

void GameLayer::onTouchesBegan(const std::vector<Touch*>& touches,Event *event)
{
    Touch *touch=touches[0];
    cocos2d::Point point=touch->getLocation();
    
    if(m_curGameTag == GameTag::GAME_PLAYING && m_pHero->getPositionY()<g_size.height)
    {
        m_pHero->fly_tag=FlyTag::FLY_UP;
        speed_hero=speed_hero_up;
        speed_tmp_1=0;
        speed_tmp_2=0;
		playEffect("music_fx_jump.mp3");
    }
	else if(m_curGameTag == GameTag::GAME_READY)
    {
        m_curGameTag = GameTag::GAME_PLAYING;
        if(m_bShowWarn)
        {
            ready_view->setVisible(false);
            m_bShowWarn=false;
        }
        this->getChildByTag(TAG_SCORE)->setVisible(true);
        for(auto _node : m_vecLifeIcons)
        {
            _node->setVisible(true);
        }
        m_pHero->fly_tag=FlyTag::FLY_UP;
        speed_hero=speed_hero_up;
        speed_tmp_1=0;
        speed_tmp_2=0;
    }
}

void GameLayer::onTouchesEnded(const std::vector<Touch *> &touches, Event *event)
{
    
}

void GameLayer::add_obstacle()
{
    if(m_ScoreDataValue->get_data()<10)
        m_ObstacleAddTag=ObstacleAddTag::ADD_NORMAL;
    else if(is_between(10, 20))
        m_ObstacleAddTag=ObstacleAddTag::ADD_MOVE;
    else if(is_between(20, 30))
        m_ObstacleAddTag=ObstacleAddTag::ADD_MOVEBY;
    else if(is_between(30, 40))
        m_ObstacleAddTag=ObstacleAddTag::ADD_LIMIT;
    
    
    add_type_normal();
    add_type_move();
    add_type_moveby();
    add_tyoe_limit();
    add_tyoe_fix();
    
    auto obstacle_0=Sprite::createWithSpriteFrameName(NAME_BUILD);
    auto obstacle_1=Sprite::createWithSpriteFrameName(NAME_BUILD);
    
    obstacle_0->setAnchorPoint(cocos2d::Point(0.5,0.5));
    obstacle_1->setAnchorPoint(cocos2d::Point(0.5,0.5));
    
    
    int max_y=2048+800;
    int min_y=1800;
    
    int y_0=CCRANDOM_0_1()*(max_y-min_y)+min_y;
    int y_1=y_0-600-obstacle_1->getBoundingBox().size.height;
    
    obstacle_0->setPosition(1536+obstacle_1->getContentSize().width,y_0);
    obstacle_1->setPosition(1536+obstacle_1->getContentSize().width,y_1);
    
    obstacle_0->setTag(TAG_OBSTACLE);
    obstacle_1->setTag(TAG_OBSTACLE);
    this->addChild(obstacle_0);
    this->addChild(obstacle_1);
    list.pushBack(obstacle_0);
    list.pushBack(obstacle_1);
    
    auto node=Node::create();
    node->setPosition(1536+obstacle_1->getContentSize().width,0);
    score_list.pushBack(node);
    
    if(CCRANDOM_0_1()>0.4f)
    {
        
        auto img_pants=Sprite::createWithSpriteFrameName(NAME_PANTS);
        img_pants->setAnchorPoint(cocos2d::Point(0.5f,0.5f));
        int y3=y_1+obstacle_1->getBoundingBox().size.height/2+300;
        img_pants->setPosition(1536+obstacle_1->getContentSize().width,y3);
        img_pants->setTag(TAG_PANTS);
        this->addChild(img_pants);
        list.pushBack(img_pants);
    }
    m_fObstaclCreateTimer=115;
}

void GameLayer::add_type_normal()
{
    if(m_ObstacleAddTag!=ObstacleAddTag::ADD_NORMAL)
        return;
    
    //create sprite
    auto obstacle_0=Sprite::createWithSpriteFrameName(NAME_BUILD);
    auto obstacle_1=Sprite::createWithSpriteFrameName(NAME_BUILD);
    
}

void GameLayer::action_move_background()
{
    float w=g_size.width-1;
    
    m_pForeground_0->setPositionX(m_pForeground_0->getPositionX()-speed_obstacle*1.2*RATIO);
    m_pForeground_1->setPositionX(m_pForeground_1->getPositionX()-speed_obstacle*1.2*RATIO);
#ifdef __BACKGROUND_HOUSE__
    m_pBackground_1_0->setPositionX(m_pBackground_1_0->getPositionX()-speed_background*RATIO);
    m_pBackground_1_1->setPositionX(m_pBackground_1_1->getPositionX()-speed_background*RATIO);
#endif //__BACKGROUND_HOUSE__
    
    if(m_pForeground_0->getPositionX()<-w)
        m_pForeground_0->setPositionX(m_pForeground_1->getPositionX()+w);
    if(m_pForeground_1->getPositionX()<-w)
        m_pForeground_1->setPositionX(m_pForeground_0->getPositionX()+w);
    
#ifdef __BACKGROUND_HOUSE__
    if(m_pBackground_1_0->getPositionX()<-w)
    {
        m_pBackground_1_0->setPositionX(m_pBackground_1_1->getPositionX()+w);
    }
	if(m_pBackground_1_1->getPositionX()<-w)
	{
		m_pBackground_1_1->setPositionX(m_pBackground_1_0->getPositionX()+w);
	}
#endif //__BACKGROUND_HOUSE__	
}

void GameLayer::action_moveBack_background(float lv)
{
    float w=g_size.width-1;
    m_pForeground_0->setPositionX(m_pForeground_0->getPositionX()+speed_obstacle*lv*1.2*RATIO);
    m_pForeground_1->setPositionX(m_pForeground_1->getPositionX()+speed_obstacle*lv*1.2*RATIO);
#ifdef __BACKGROUND_HOUSE__
    m_pBackground_1_0->setPositionX(m_pBackground_1_0->getPositionX()+speed_background*lv*RATIO);
	m_pBackground_1_1->setPositionX(m_pBackground_1_1->getPositionX()+speed_background*lv*RATIO);
#endif //__BACKGROUND_HOUSE__
    
    if(m_pForeground_0->getPositionX()>=w)
        m_pForeground_0->setPositionX(m_pForeground_1->getPositionX()-w);
    if(m_pForeground_1->getPositionX()>=w)
        m_pForeground_1->setPositionX(m_pForeground_0->getPositionX()-w);
    
#ifdef __BACKGROUND_HOUSE__
    if(m_pBackground_1_0->getPositionX()>=w)
        m_pBackground_1_0->setPositionX(m_pBackground_1_1->getPositionX()-w);
    if(m_pBackground_1_1->getPositionX()>=w)
        m_pBackground_1_1->setPositionX(m_pBackground_1_0->getPositionX()-w);
#endif //__BACKGROUND_HOUSE__    
}

void GameLayer::action_remove_node(cocos2d::Node *node_0,cocos2d::Node *node)
{
    node->removeFromParent();
}

void GameLayer::create_life_icon()
{
    for(int i=0;i<g_nHeroHP;i++)
    {
        auto life_icon=Sprite::createWithSpriteFrameName("ui_img_game_progress_life.png");
        life_icon->setAnchorPoint(cocos2d::Point(0.5,0));
        life_icon->setPosition(cocos2d::Point(m_SceneSize.width*0.9-i*100*RATIO,m_SceneSize.height*0.9-life_icon->getBoundingBox().size.height/2-10*RATIO));
        this->addChild(life_icon,4);
        m_vecLifeIcons.pushBack(life_icon);
    }
}

void GameLayer::update_life_icon()
{
    if(m_vecLifeIcons.size()>0)
    {
        m_vecLifeIcons.back()->removeFromParent();
        m_vecLifeIcons.popBack();
    }
}

void GameLayer::update_score()
{
    if(m_curGameTag == GameTag::HERO_PLAYDEAD)
        return;
    if(m_curGameTag == GameTag::GAME_PLAYING)
	{
		playEffect("music_fx_ding.mp3");
	}
	m_ScoreDataValue->offset_data(1);
    auto ui=(LabelTTF*)this->getChildByTag(TAG_SCORE);
    ui->retain();
    ui->removeFromParent();
    this->addChild(ui);
    ui->setString(String::createWithFormat("%d",m_ScoreDataValue->get_data())->getCString());
}

void GameLayer::SetPantys(int nPantys)
{
	m_PantsDataValue->set_data(nPantys);
	if (g_pGameStartGUILayer)
		g_pGameStartGUILayer->update_panty_score();
}

void GameLayer::add_pants(int i)
{
    ValueMap node=map_panty.at(String::createWithFormat("%d",i)->getCString()).asValueMap();
    int _panty_score=node.at("Score").asInt();
    m_PantsDataValue->offset_data(_panty_score);
}

bool GameLayer::is_between(int min,int max)
{
    bool ret=true;
    if(m_ScoreDataValue->get_data()<min)
        ret=false;
    if(m_ScoreDataValue->get_data()>max)
        ret=false;
    
    return ret;
}

void GameLayer::ShowShop()
{
	g_pGameShopGUILayer = GameShopGUILayer::create();
	addChild(g_pGameShopGUILayer,20);

	if (m_curGameTag != GameTag::GAME_SHOP)
	{
		m_oldGameTag = m_curGameTag;
		g_pGameShopGUILayer->SetPrevGameTag(m_oldGameTag);
	}
	m_curGameTag = GameTag::GAME_SHOP;
}


void GameLayer::ShowPay(int nPayWay, float fRMB, const char *pszPayCode, int nHero, int nPanty)
{
#ifdef FREE_GAME
	return;
#endif
	if (g_pGamePayGUILayer)
	{
		removeChild(g_pGamePayGUILayer);
		g_pGamePayGUILayer = NULL;
	}
	g_pGamePayGUILayer = GamePayGUILayer::create();
	addChild(g_pGamePayGUILayer,30);

	if (m_curGameTag != GameTag::GAME_PAY)
	{
		m_oldGameTag = m_curGameTag;
		g_pGamePayGUILayer->SetPrevGameTag(m_oldGameTag);
	}
	m_curGameTag = GameTag::GAME_PAY;

	g_pGamePayGUILayer->InitRMB(nPayWay, fRMB, pszPayCode, nHero, nPanty);
}

void GameLayer::ShowPayHero10()
{
	bool bHero10 = UserDefault::sharedUserDefault()->getBoolForKey(KEY_DATA_HERO10);
	if (!bHero10)
	{
		for (int i=4; i<11;++i)
		{
			if (g_vecHasHeroes[i] == true)
				continue;
#ifdef __IOS__
			std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_heroes_list.plist");
#else
			std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_heroes_list.plist");
#endif //__IOS__
			ValueMap map_heroes = FileUtils::getInstance()->getValueMapFromFile(fullPath);
			ValueMap node=map_heroes.at(String::createWithFormat("%d",i+1)->getCString()).asValueMap();
			std::string strPayCode = node.at("Paycode").asString();
			float fRMB = node.at("RMB").asFloat();
			ShowPay(PAY_Buy, fRMB, strPayCode.c_str(), i);
			return;
		}
	}
}

void GameLayer::ShowPayRelive()
{
	/*
	int nRelive = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_RELIVE);
	switch (nRelive)
	{
	case 0:
		{
			ShowPay(PAY_Relive, 0.1f, "13");
		}
		break;

	case 1:
		{
			bool bHero10 = UserDefault::sharedUserDefault()->getBoolForKey(KEY_DATA_HERO10);
			if (!bHero10)
				ShowPayHero10();
			else
				ShowPay(PAY_Relive, 0.1f, "13");
		}
		break;

	case 2:
		{
			bool bHero20 = UserDefault::sharedUserDefault()->getBoolForKey(KEY_DATA_HERO20);
			if (!bHero20)
				ShowPay(PAY_Buy, 20.0f, "10", 11);
		}
		break;

	default:
		ShowPay(PAY_Relive, 10.0f, "11");
		break;
	}
	*/

	//bool bHero10 = UserDefault::sharedUserDefault()->getBoolForKey(KEY_DATA_HERO10);
	//if (!bHero10)
	//{
	//	ShowPayHero10();
	//	return;
	//}

	//bool bHero20 = UserDefault::sharedUserDefault()->getBoolForKey(KEY_DATA_HERO20);
	//if (!bHero20)
	//{
	//	ShowPay(PAY_Buy, 20.0f, "10", 11);
	//	return;
	//}
	ShowPay(PAY_Relive, 10.0f, "11");
}
