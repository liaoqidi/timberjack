//
//  MainScene.cpp
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#include "MainScene.h"

MainScene::MainScene(){};
MainScene::~MainScene(){};
using namespace cocos2d;

bool MainScene::init()
{
    bool ret=false;
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(FILE_PLIST_BACKGROUND);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(FILE_PLIST_HERO);
    
    //music init
	preloadBackgroundMusic("superpanty.mp3");
	preloadBackgroundMusic("start.mp3");
	preloadEffect("music_fx_back.mp3");
	preloadEffect("music_fx_drop.mp3");
	preloadEffect("music_fx_jump.mp3");
	preloadEffect("music_fx_select.mp3");
	preloadEffect("music_fx_ding.mp3");
	preloadEffect("music_fx_drop_1.mp3");
	preloadEffect("music_fx_over.mp3");
	preloadEffect("music_fx_getnew.mp3");
	preloadEffect("music_fx_getnew_1.mp3");
    
    CocosDenshion::SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    GameLayer *game_layer=GameLayer::create();
    this->addChild(game_layer);

    ret=true;
    return ret;
}

void MainScene::update(float dt)
{
    
}