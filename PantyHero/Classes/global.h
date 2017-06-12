//
//  global.h
//  PantsHero
//
//  Created by lyon on 14-3-7.
//
//

#ifndef __PantsHero__global__
#define __PantsHero__global__

#include <iostream>
#include <vector>
#include "cocos2d.h"
#include "HanDou.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//#define UMENG
#endif

//#ifdef UMENG
//#include "libMobClickCpp/include/MobClickCpp.h"
//#endif //UMENG

#define HERO_RECT_W 110
#define HERO_RECT_H 160
/*  swich ratio */
//#define ipad_hd
//#define ipad
//#define iphone_h
#define iphone


#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifndef safeStrncpy		//必须确认strDest不是指针，否则只拷贝3个字节
#	define safeStrncpy(strDest, strSource)		strncpy((strDest), (strSource), _countof(strDest)-1)
//#	define safeStrncpy(strDest, strSource)		strncpy_s((strDest), min(_countof(strDest), strlen(strSource)+1), (strSource), _countof(strDest)-1)
#endif

// 安全删除和释放
#ifndef safeDelete
#	define safeDelete(ptr)		if ((ptr)) {delete (ptr); (ptr) = 0;}
#endif
#ifndef safeDeleteArray
#	define safeDeleteArray(ptr)	if ((ptr)) {delete[] (ptr); (ptr) = 0;}
#endif
#ifndef safeRelease
#	define safeRelease(ptr)		if ((ptr)) {(ptr)->Release(); (ptr) = 0;}
#endif


enum class GameTag
{
	GAME_PLAYING,
	GAME_OVER,
	GAME_BEGAN,
	GAME_READY,
	GAME_SHOP,
	GAME_PAY,
	GAME_TIME_BACK,
	HERO_PLAYDEAD,
};


extern int g_nHeroID;
extern int g_nHeroHP;
extern int g_nSource;
extern std::vector<bool> g_vecHasHeroes;
extern std::vector<bool> g_vecHasPanties;
extern std::vector<bool> g_vecHasExchange;
extern cocos2d::Size g_size;
extern int g_nGotPanty;
extern bool g_bSeePanty;
extern float RATIO;
extern float RATIO_V;
extern bool g_bMusicPlay;

extern std::string file_png_producer;
extern std::string file_plist_background;
extern std::string file_plist_cell;
extern std::string file_plist_hero;
extern std::string file_plist_panty;
extern std::string file_json_startgame;
extern std::string file_json_getready;
extern std::string file_json_gameover;
extern std::string file_json_shop;
extern std::string file_json_pay;


unsigned int playEffect(const char* pszFilePath, bool bLoop = false, float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);
void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);
void resumeBackgroundMusic();
void preloadBackgroundMusic(const char* pszFilePath);
void preloadEffect(const char* pszFilePath);
void DebugLog(const char *pText, ...);

#endif /* defined(__PantsHero__global__) */

