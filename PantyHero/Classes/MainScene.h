//
//  MainScene.h
//  胖次英雄
//
//  Created by lyon on 14-3-2.
//
//

#ifndef ________MainScene__
#define ________MainScene__

#include <iostream>
#include "GameLayer.h"
#include "Game.h"
using namespace cocos2d;

class MainScene : public Scene
{
public:
    MainScene();
    ~MainScene();
    bool init();
    CREATE_FUNC(MainScene);
    
    void update(float dt);
private:
    SpriteBatchNode *background_actors;
    SpriteBatchNode *hero_actors;
    SpriteBatchNode *gui_actors;
};


#endif /* defined(________MainScene__) */
