//
//  global.cpp
//  PantsHero
//
//  Created by lyon on 14-3-7.
//
//

#include "global.h"
#include "SimpleAudioEngine.h"

int g_nHeroID;
int g_nSource = 5;
int g_nHeroHP;
int g_nGotPanty;
bool g_bSeePanty;
float RATIO;
float RATIO_V;
bool g_bMusicPlay;
cocos2d::Size g_size;
std::vector<bool> g_vecHasHeroes;
std::vector<bool> g_vecHasPanties;
std::vector<bool> g_vecHasExchange;
std::string file_plist_background;
std::string file_plist_cell;
std::string file_plist_hero;
std::string file_plist_panty;
std::string file_json_startgame;
std::string file_json_getready;
std::string file_json_gameover;
std::string file_json_shop;
std::string file_json_pay;
std::string file_png_producer;

//#define __IOS__
using namespace cocos2d;

unsigned int playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain)
{
	if (g_bMusicPlay)
	{
#ifdef __IOS__
		return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
#else
		char szFilePath[256] = "music/fx/";
		strcat(szFilePath, pszFilePath);

		return CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(szFilePath, bLoop, pitch, pan, gain);
#endif //__IOS__
	}

	return 0;
}

void playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
	if (!g_bMusicPlay)
		return;

#ifdef __IOS__
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(pszFilePath, bLoop);
#else
	char szFilePath[256] = "music/background/";
	strcat(szFilePath, pszFilePath);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(szFilePath, bLoop);
#endif //__IOS__
}

void resumeBackgroundMusic()
{
	if (g_bMusicPlay)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

void preloadBackgroundMusic(const char* pszFilePath)
{
#ifdef __IOS__
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(pszFilePath);
#else
	char szFilePath[256] = "music/background/";
	strcat(szFilePath, pszFilePath);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(szFilePath);
#endif //__IOS__
}

void preloadEffect(const char* pszFilePath)
{
#ifdef __IOS__
	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(pszFilePath);
#else
	char szFilePath[256] = "music/fx/";
	strcat(szFilePath, pszFilePath);

	CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect(szFilePath);
#endif //__IOS__
}


void DebugLog(const char *pText, ...)
{
	char szBuf[1024];
	int	nBuf = 0;
	va_list	ap;

	memset(szBuf, 0, 1024);
	try
	{
		va_start(ap, pText);
		nBuf = vsprintf(szBuf, pText, ap);
		szBuf[1024 - 1] = 0;
		va_end(ap);
	}
	catch (...)
	{
		//
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	__android_log_print(ANDROID_LOG_DEBUG, "MiniPi", szBuf);
#else
	//char szLine[16];
	//_snprintf(szLine, 15, "(%d): ", __LINE__);
	//OutputDebugStringA(__FILE__);
	//OutputDebugStringA(szLine);
	OutputDebugStringA("MiniPi ");
	OutputDebugStringA(szBuf);
	OutputDebugStringA("\n");
#endif
}
