//
//  GamePayGUILayer.cpp
//  PantsHero
//
//  Created by lj on 14-3-6.
//
//

#include "GamePayGUILayer.h"
#include "SimpleAudioEngine.h"
#include "global.h"
#include "GameLayer.h"
#include "GameStartGUILayer.h"
#include "autostring.h"

//#import <ShareSDK/ShareSDK.h>

#define TAG_PANTIES_VAL 100

GamePayGUILayer *g_pGamePayGUILayer = NULL;
bool	g_bFirstRelive = true;
char	*g_pSelectorGroup = (char*)"GamePayGUILayer";
int m_IsNormalVer = 1;
int m_nMaxMoney = 30;
int m_nTodayMoney = 0;

GamePayGUILayer::GamePayGUILayer()
{
	m_bCanBuy = true;
	m_nHero = 0;
	m_fRMB = 0;
//	m_strPayCode = "大礼包";
    m_strPayCode = "013";
}

GamePayGUILayer::~GamePayGUILayer()
{
	// 移除相关联的选择器，因为我们将要销毁当前实例
	NDKHelper::RemoveSelectorsInGroup(g_pSelectorGroup);
}

bool GamePayGUILayer::init()
{
    do
    {
        //load file
        m_layout = dynamic_cast<Layout*>(GUIReader::shareReader()->widgetFromJsonFile(file_json_pay.c_str()));
		if (!m_layout)
			return false;

        this->addChild(m_layout);
        
        //set button
        Button *pBtnOk = dynamic_cast<Button*>(m_layout->getChildByName("button_ok"));
        Button *pBtnCancel = dynamic_cast<Button*>(m_layout->getChildByName("button_cancel"));
		if (!pBtnOk || !pBtnCancel)
			return false;
        
		m_pGiftBG = m_layout->getChildByName("pay_gift");
		m_pGiftBG02 = m_layout->getChildByName("pay_gift_02");
		m_pReliveBG = m_layout->getChildByName("pay_relive");
		m_pReliveBG02 = m_layout->getChildByName("pay_relive_02");
		m_pBuyBG = m_layout->getChildByName("pay_buy");
		m_pHero = dynamic_cast<ImageView*>(m_layout->getChildByName("pay_hero"));
		m_pHero->loadTexture("hero_001_01.png", UI_TEX_TYPE_PLIST);

		m_pGiftBG->setVisible(true);
		m_pGiftBG02->setVisible(true);
		m_pReliveBG->setVisible(false);
		m_pReliveBG02->setVisible(false);
		m_pBuyBG->setVisible(false);
		m_pHero->setVisible(false);

		//set listener
        pBtnOk->addTouchEventListener(this, toucheventselector(GamePayGUILayer::onBtnOK));
        pBtnCancel->addTouchEventListener(this, toucheventselector(GamePayGUILayer::onBtnCancel));

		int nFontSize = 60;
		m_pLabelMoney=LabelTTF::create("花费10元购买道具“大礼包”。", GAME_FONT_NAME, nFontSize*RATIO);
		m_pLabelMoney->setColor(cocos2d::Color3B(130,130,130));
		m_pLabelMoney->setPosition(g_size.width*0.5f,g_size.height*0.07f);
		m_pLabelMoney->setHorizontalAlignment(TextHAlignment::CENTER);
		//m_pLabelMoney->setVisible(false);
		addChild(m_pLabelMoney, 2);

//#if defined(ABNORMAL_VER_XX) || defined(ABNORMAL_VER_XA) || defined(ABNORMAL_VER_XN)
//		pBtnOk->setScale(1.88f);
//
//#if defined(ABNORMAL_VER_XN)
//		pBtnCancel->setScale(0.48f);
//#else
//		pBtnCancel->setScale(0.98f);
//#endif
//		pBtnCancel->setColor(cocos2d::Color3B(38,50,66));
//		m_pLabelMoney->setColor(cocos2d::Color3B(28,40,56));
//		m_pLabelMoney->setFontSize(20);
//
//#if defined(ABNORMAL_VER_XX) || defined(ABNORMAL_VER_XN)
//		m_layout->addTouchEventListener(this, toucheventselector(GamePayGUILayer::onBtnOK));
//#endif	//ABNORMAL_VER_XX || ABNORMAL_VER_XN
//
//#endif	//ABNORMAL_VER_XX || ABNORMAL_VER_XA || ABNORMAL_VER_XN

		if (!m_IsNormalVer)
		{
			pBtnOk->setScale(1.88f);
			pBtnCancel->setScale(0.98f);
			pBtnCancel->setColor(cocos2d::Color3B(38, 50, 66));
			m_pLabelMoney->setColor(cocos2d::Color3B(28, 40, 56));
			m_pLabelMoney->setFontSize(20);
			m_layout->addTouchEventListener(this, toucheventselector(GamePayGUILayer::onBtnOK));
		}

		//pBtnOk->setScale(2.88f);

    } while (0);

    return true;
}

void GamePayGUILayer::update(float delta)
{
	if (m_nPayWay != PAY_Buy)
		return;

	static int s_nAnimTime = 0;
	static int s_nAnimFarme = 1;
	if (++s_nAnimTime > 10 && m_pHero)
	{
		m_pHero->loadTexture(String::createWithFormat("hero_%03d_%02d.png", m_nHero+1, s_nAnimFarme)->getCString(), UI_TEX_TYPE_PLIST);
		s_nAnimTime = 0;
		if (++s_nAnimFarme > 3)
			s_nAnimFarme = 1;
	}
}

void GamePayGUILayer::InitRMB(int nPayWay, float fRMB, const char *pszPayCode, int nHero, int nPanty)
{
	fRMB = 10.0f;	// 全部都是10元
	m_pGiftBG->setVisible(false);
	m_pGiftBG02->setVisible(false);
	m_pReliveBG->setVisible(false);
	m_pReliveBG02->setVisible(false);
	m_pBuyBG->setVisible(false);
	m_pHero->setVisible(false);

	m_nPayWay = nPayWay;
	m_nPanty = nPanty;
	if (nHero < 0 || nHero >= 12)
		nHero = 0;
	m_nHero = nHero;


#if defined(ABNORMAL_VER_XN)
	m_pLabelMoney->setColor(cocos2d::Color3B(28,40,56));
#endif
	if (!m_IsNormalVer)
		m_pLabelMoney->setColor(cocos2d::Color3B(28, 40, 56));

	autostring str;
	char szText[256];
	switch(nPayWay)
	{
	case PAY_Gift:
		{
			m_pGiftBG->setVisible(true);
			m_pGiftBG02->setVisible(true);
//			m_strPayCode = "大礼包";
            m_strPayCode = "013";
			fRMB = 10.0f;
			sprintf(szText, "花费10元购买道具“大礼包”。");
		}
		break;

	case PAY_Relive:
		{
#if defined(ABNORMAL_VER_XN)
			m_pLabelMoney->setColor(cocos2d::Color3B(130,130,130));
#endif

			m_pReliveBG->setVisible(true);
			m_pReliveBG02->setVisible(true);

			m_strPayCode = pszPayCode;
//			m_strPayCode = "复活一次";
//            m_strPayCode = "012";
			if (fRMB == 10.0f)
				sprintf(szText, "花费10元购买道具“复活一次”。");
			else
				sprintf(szText, "花费0.1元购买道具“复活一次”。");
		}
		break;

	case PAY_Buy:
		{
			m_pBuyBG->setVisible(true);
			m_pHero->setVisible(true);
			m_strPayCode = pszPayCode;

#ifdef __IOS__
			std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data_heroes_list.plist");
#else
			std::string fullPath = FileUtils::getInstance()->fullPathForFilename("data/data_heroes_list.plist");
#endif //__IOS__
			ValueMap map_heroes = FileUtils::getInstance()->getValueMapFromFile(fullPath);
			ValueMap node = map_heroes.at(String::createWithFormat("%d", m_nHero+1)->getCString()).asValueMap();
			std::string strName = node.at("LanguageName").asString();

			sprintf(szText, "花费%.0f元购买英雄：%s。", fRMB, str.UTF8ToANSI(strName.c_str()));
//			m_strPayCode = "购买英雄:";
//			m_strPayCode += strName;
		}
		break;

	default:
		break;
	}
	m_fRMB = fRMB;
	m_pLabelMoney->setString(str.ANSIToUTF8(szText));
	m_pLabelMoney->setVisible(true);
}

void GamePayGUILayer::onBtnCancel(Object *obj,TouchEventType type)
{    
	if (type!=TOUCH_EVENT_ENDED || !g_pGamePayGUILayer)
		return;

	if (m_nPayWay == PAY_Gift && m_prevGameTag == GameTag::GAME_PLAYING)
		g_pGameLayer->SetTimeBack();
	else
		g_pGameLayer->SetCurGameTag((GameTag)m_prevGameTag);
	removeFromParent();
	g_pGamePayGUILayer = NULL;
	
	if (g_pGameLayer->m_bQuitGame)
	{
//#ifdef UMENG
//		umeng::MobClickCpp::end();
//#endif
		Director::getInstance()->end();
	}
}

void GamePayGUILayer::onBtnOK(Object *obj,TouchEventType type)
{
	if(type!=TOUCH_EVENT_ENDED || !m_bCanBuy || !g_pGamePayGUILayer)
		return;

	if (/*m_IsNormalVer && */m_nTodayMoney >= m_nMaxMoney)
	{
		SelectorPayError(NULL, NULL);
		return;
	}

	// 免费复活一次
	//if (g_bFirstRelive && m_nPayWay == PAY_Relive)
	//{
	//	g_bFirstRelive = false;
	//	SelectorPaySuccess(NULL, NULL);
	//	return;
	//}

	// 移除相关联的选择器，因为我们将要销毁当前实例
	NDKHelper::RemoveSelectorsInGroup(g_pSelectorGroup);
	//注册一个selector，那么当我们的native环境（java环境）调用这个字符串名字的函数（此处为名字为SelectorPay的函数）的时候，这个selector可以响应请求   
	//注意：设置组名是为了在析构函数中能够清除掉selector   
	NDKHelper::AddSelector("GamePayGUILayer", "SelectorPaySuccess", callfuncND_selector(GamePayGUILayer::SelectorPaySuccess), this);  
	NDKHelper::AddSelector("GamePayGUILayer", "SelectorPayError", callfuncND_selector(GamePayGUILayer::SelectorPayError), this);  

	// 为这个将要传递到native（也就是Java）语言中的消息设置参数   
	// 在c++中发送要调用的函数名   
	CCDictionary* prms = CCDictionary::create();  
	prms->setObject(CCString::create(m_strPayCode), "PayCode");  
	//prms->setObject(__Integer::create(m_fRMB), "Money");  
	char szRMB[64];
	sprintf(szRMB, "%f", m_fRMB);
	prms->setObject(CCString::create(szRMB), "Money");  

	// 最后在当前环境（c++）中调用native方法   
	SendMessageWithParams(string("SelectorPayRequest"), prms);
	m_bCanBuy = false;
	g_pGameLayer->m_bQuitGame = false;

#ifdef _WIN32
	SelectorPaySuccess(NULL, NULL);
#endif
}


// 一个回调选择器，native语言（java）将会调用它   
void GamePayGUILayer::SelectorPaySuccess(CCNode *sender, void *data)
{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	__android_log_print(ANDROID_LOG_DEBUG, "EasyNDK-for-cocos2dx", "run success 1");
//#endif
	if (!g_pGamePayGUILayer || !m_pLabelMoney)
		return;

	m_bCanBuy = true;
	m_nTodayMoney += m_fRMB;
	//m_pLabelMoney->setString("支付成功");
	char szOperate[32] = "";

	switch(m_nPayWay)
	{
	case PAY_Gift:
		{
			safeStrncpy(szOperate, "购买大礼包");
			int nPantys = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_PANTS);
			nPantys += 1000;
			UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_PANTS, nPantys);
			UserDefault::sharedUserDefault()->flush();
			g_pGameLayer->SetPantys(nPantys);
			if (m_prevGameTag == GameTag::GAME_PLAYING)
				g_pGameLayer->SetTimeBack();
		}
		break;

	case PAY_Relive:
		{
			safeStrncpy(szOperate, "购买复活");
			g_pGameLayer->SetTimeBack();

			if (m_fRMB == 0.1f)
			{
				int nRelive = UserDefault::sharedUserDefault()->getIntegerForKey(KEY_DATA_RELIVE, 0);
				UserDefault::sharedUserDefault()->setIntegerForKey(KEY_DATA_RELIVE, nRelive+1);
				UserDefault::sharedUserDefault()->flush();
			}
			removeFromParent();
			g_pGamePayGUILayer = NULL;
			return;
		}
		break;

	case PAY_Buy:
		{
			if (m_nHero < 0)		m_nHero = 0;
			if (m_nHero > 11)		m_nHero = 11;
			g_nHeroID = m_nHero + 1;
			g_vecHasHeroes[m_nHero] = true;
			for(int i=1;i<=12;i++)
				UserDefault::sharedUserDefault()->setBoolForKey(String::createWithFormat(KEY_HAS_HERO,i)->getCString(), g_vecHasHeroes[i-1]);
			if (m_fRMB == 10.0f)
				UserDefault::sharedUserDefault()->setBoolForKey(KEY_DATA_HERO10, true);
			if (m_fRMB == 20.0f)
				UserDefault::sharedUserDefault()->setBoolForKey(KEY_DATA_HERO20, true);
			UserDefault::sharedUserDefault()->setIntegerForKey(KEY_USED_HERO_ID,g_nHeroID);
			UserDefault::sharedUserDefault()->flush();
			if (g_pGameShopGUILayer)
				g_pGameShopGUILayer->UpdateGUI(m_nHero);
			if (g_pGameStartGUILayer)
				g_pGameStartGUILayer->update_play_button();
			sprintf(szOperate, "购买英雄 %d", m_nHero);
		}
		break;

	default:
		break;
	}
//#ifdef UMENG
//	DebugLog("Pay success,RMB=%f,PayCode=%s", m_fRMB, m_strPayCode.c_str());
//	//const char *pszPayCode = m_strPayCode.c_str();
//	umeng::MobClickCpp::pay((double)m_fRMB, g_nSource, m_strPayCode.c_str(), 1, 1);
//	umeng::MobClickCpp::buy(m_strPayCode.c_str(), 1, (double)m_fRMB);
//#endif

	onBtnCancel(NULL, TOUCH_EVENT_ENDED);
}

// 一个回调选择器，native语言（java）将会调用它   
void GamePayGUILayer::SelectorPayError(CCNode *sender, void *data)
{
	if (!g_pGamePayGUILayer || !m_pLabelMoney)
		return;

	m_bCanBuy = true;

	const char *strError = "支付失败";
	CCDictionary *pDict = (CCDictionary*)data;
	if (pDict)
	{
		const __String *pValueChannel = pDict->valueForKey("strError");
		strError = pValueChannel->getCString();
	}

	m_pLabelMoney->setString(strError);
	//onBtnCancel(NULL, TOUCH_EVENT_ENDED);
}
