#include "AppDelegate.h"
#include "GameLayer.h"
#include "Setting.h"
#include "cocos2d.h"
#include "GUISetting.h"
#include "CocosGUI.h"
#include "global.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;


//#define __IOS__
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto eglView = EGLView::getInstance();
    director->setOpenGLView(eglView);
	eglView->setDesignResolutionSize(768 , 1024, ResolutionPolicy::SHOW_ALL);
    cocos2d::Size size_px=eglView->getVisibleSize();
    
    RATIO=0.5;
    RATIO_V=0.5;
    
#ifdef __IOS__
    file_plist_background="scene_background.plist";
    file_plist_cell="ui_ipad_normal_1024.plist";
    file_plist_hero="hero_ipad.plist";
    file_plist_panty="panty_ipad.plist";
    file_json_gameover="pantyhero_ipad_normal_gameover.ExportJson";
    file_json_getready="pantyhero_ipad_normal_getready.ExportJson";
    file_json_shop="pantyhero_ipad_normal_shop.ExportJson";
    file_json_startgame="pantyhero_ipad_normal_startgame.ExportJson";
    file_json_pay="pantyhero_ipad_normal_pay.ExportJson";
    file_png_producer="ipad_producer_view.png";
#else
    file_plist_background	= "ipad/scene_background.plist";
    file_plist_hero			= "ipad/hero_ipad.plist";
    file_plist_panty		= "ipad/panty_ipad.plist";
    file_png_producer		= "ipad/ipad_producer_view.png";
	file_plist_cell			= "ipad/ipad_ui/ui_ipad_normal_1024.plist";
	file_json_gameover		= "ipad/ipad_ui/pantyhero_ipad_normal_gameover.ExportJson";
	file_json_getready		= "ipad/ipad_ui/pantyhero_ipad_normal_getready.ExportJson";
	file_json_shop			= "ipad/ipad_ui/pantyhero_ipad_normal_shop.ExportJson";
	file_json_startgame		= "ipad/ipad_ui/pantyhero_ipad_normal_startgame.ExportJson";
    file_json_pay			= "ipad/ipad_ui/pantyhero_ipad_normal_pay.ExportJson";
#endif //__IOS__

    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    
    auto main_scene=Scene::create();
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file_plist_background.c_str());
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file_plist_cell.c_str());
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(file_plist_hero.c_str());
    
    g_pGameLayer = GameLayer::create();
    main_scene->addChild(g_pGameLayer);
    director->runWithScene(main_scene);

//#ifdef UMENG
//	DebugLog("MOBCLICKCPP_START_WITH_APPKEY 1");
//	MOBCLICKCPP_START_WITH_APPKEY_AND_CHANNEL("568cf5a267e58e1a370022dd", "MiniPi");
//	DebugLog("MOBCLICKCPP_START_WITH_APPKEY 2");
//#endif

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

//#ifdef UMENG
//	umeng::MobClickCpp::applicationDidEnterBackground();
//#endif

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

//#ifdef UMENG
//	umeng::MobClickCpp::applicationWillEnterForeground();
//#endif

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	resumeBackgroundMusic();
}

void AppDelegate::show_leader()
{
    //[[GameKitHelper shared_gameKit_helper] show_board_leader];
}
