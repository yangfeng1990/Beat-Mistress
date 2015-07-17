#include "WelcomeScene.h"
#include "WelcomeLayer.h"
#include "Audio.h"

USING_NS_CC;

bool WelcomeScene::init()
{
	// super init first
	if (!Scene::init())
	{
		return false;
	}
	// loading resource picture to cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("main.plist", "main.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("fruit.plist", "fruit.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("girl.plist", "girl.png");
	
	this->addChild(WelcomeLayer::create());
	Audio::getInstance()->playWelcome();
	return true;
}