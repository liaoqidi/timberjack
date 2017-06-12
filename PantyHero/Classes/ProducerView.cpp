//
//  ProducerView.cpp
//  PantsHero
//
//  Created by lyon on 14-3-24.
//
//

#include "ProducerView.h"
#include "global.h"

bool ProducerView::init()
{
    bool ret=false;
    auto _producer=cocos2d::Sprite::create(file_png_producer.c_str());
    if(_producer == NULL)
        return ret;
    
    _producer->setAnchorPoint(cocos2d::Point(0,0));
    _producer->setPosition(0, 0);
    this->addChild(_producer, 1);
    
    
    // touch
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesEnded = CC_CALLBACK_2(ProducerView::onTouchesEnded, this);
    listener->onTouchesBegan = CC_CALLBACK_2(ProducerView::onTouchesBegan, this);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    ret=true;
    return ret;
}

void ProducerView::onTouchesBegan(const std::vector<Touch*>& touches,Event *event)
{
}

void ProducerView::onTouchesEnded(const std::vector<Touch *> &touches, Event *event)
{
    if(show_producer)
    {
        show_producer=false;
    }
}