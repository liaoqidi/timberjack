#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	SimpleAudioEngine::getInstance()->end();
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	if(!glview) {
        glview = GLView::create("My Game");
        director->setOpenGLView(glview);
    }
	CCSize frameSize = glview->getFrameSize();
	//这填写的就是一般你作为背景图片的那种图片的大小，适配的原理就是放到和缩小，而以什么为参照，当然就是
	//以最大的那张图片为参照，什么图片最大，当然是背景图片了，以后美工做图的时候用的就是以下的这个尺寸
	CCSize winSize=CCSize(540,960);
	//将宽和高做一个比，通过这个比，来具体的调整逻辑分辨率的大小
	//float widthRate = frameSize.width/winSize.width;
	//float heightRate = frameSize.height/winSize.height;
	////如果是if中的语句，说明逻辑的高度有点大了，就把逻辑的高缩小到和宽度一样的比率
	//if (widthRate > heightRate)
	//{
	//	//里边传入的前俩个参数就是逻辑分辨率的大小，也就是通过getWinSize()得到的大小
	//	glview->setDesignResolutionSize(winSize.width,
	//	winSize.height*heightRate/widthRate, kResolutionNoBorder);
	//}
	//else
	//{
	//	glview->setDesignResolutionSize(winSize.width*widthRate/heightRate, winSize.height,
	//		kResolutionNoBorder);
	//}

    // turn on display FPS
   // director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
