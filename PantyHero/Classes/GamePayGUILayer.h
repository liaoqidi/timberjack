//
//  GamePayGUILayer.h
//  PantsHero
//
//  Created by Fisho on 14-3-6.
//
//

#ifndef __PantsHero__GamePayGUILayer__
#define __PantsHero__GamePayGUILayer__

#include <iostream>
#include "cocos2d.h"
#include "CocosGUI.h"
#include "CocoStudio.h"
#include "GameShopGUILayer.h"
#include "global.h"
#include "ProducerView.h"
#include "NDKHelper.h"

//////////////////////////////////////////////////////////////////////////
// **********************************************************************
// 下面三个宏只能用其中某一个
#define NORMAL_VER_X		//文字提示清晰，点背景图不付费
//#define ABNORMAL_VER_XA	//文字提示不清晰，点背景图不会付费
//#define ABNORMAL_VER_XX	//文字提示不清晰，点背景图会付费
//#define ABNORMAL_VER_XN		//只复活界面文字提示清晰，点背景图会付费，关闭按钮变小
// **********************************************************************
//////////////////////////////////////////////////////////////////////////


using namespace cocos2d::gui;
using namespace cocostudio;
using namespace cocos2d;

enum PAY_WAY
{
	PAY_Gift,	//大礼包购买
	PAY_Relive,	//复活
	PAY_Buy,	//商城购买
};

class GamePayGUILayer : public Layer
{
public:
    GamePayGUILayer();
    ~GamePayGUILayer();

    bool init();
	void update(float delta);
    CREATE_FUNC(GamePayGUILayer);
	void InitRMB(int nPayWay, float fRMB=2, const char *pszPayCode="", int nHero=0, int nPanty=0);
	void SetPrevGameTag(GameTag curGameTag)	{ m_prevGameTag = curGameTag; };

    //function call
    void onBtnOK(Object *obj,TouchEventType type);
    void onBtnCancel(Object *obj,TouchEventType type);
	void SelectorPaySuccess(CCNode *sender, void *data);
	void SelectorPayError(CCNode *sender, void *data);
    
private:
	GameTag m_prevGameTag;
	bool	m_bCanBuy;
	Layout *m_layout;
	LabelTTF *m_pLabelMoney;
	Widget	*m_pGiftBG;
	Widget	*m_pGiftBG02;
	Widget	*m_pReliveBG;
	Widget	*m_pReliveBG02;
	Widget	*m_pBuyBG;
	ImageView	*m_pHero;
	int		m_nPayWay;
	int		m_nPanty;
	int		m_nHero;
	float	m_fRMB;
	std::string		m_strPayCode;
};

extern GamePayGUILayer *g_pGamePayGUILayer;

#endif /* defined(__PantsHero__GamePayGUILayer__) */
