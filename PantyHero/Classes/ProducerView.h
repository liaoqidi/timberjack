//
//  ProducerView.h
//  PantsHero
//
//  Created by lyon on 14-3-24.
//
//

#ifndef __PantsHero__ProducerView__
#define __PantsHero__ProducerView__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class ProducerView : public cocos2d::Layer
{
public:
    ProducerView()
    {
        show_producer=true;
    };
    ~ProducerView(){};
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    bool init();
    bool show_producer;
    CREATE_FUNC(ProducerView);
};


#endif /* defined(__PantsHero__ProducerView__) */
