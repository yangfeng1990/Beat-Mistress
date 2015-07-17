#include "GameScene.h"
#include "Audio.h"
#include "GameLayer.h"

USING_NS_CC;

bool GameScene::init()
{
	// super init first
	if (!Scene::init())
	{
		return false;
	}

	 this->addChild(GameLayer::create());
	//Audio::getInstance()->playStart();

	/**********添加实体按键监听************/
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased,this); 
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) 
{
	// 返回键
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->keyBackClicked();
	} 
	//响应菜单键
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU)     
	{              
		this->keyMenuClicked();
	} 
	//  Director::getInstance()->end();  ok注销掉上面时一切ok 方法调用OK
};

void GameScene::keyBackClicked()
{
	log("keyBackClicked");
	Director::getInstance()->end(); 
}

void GameScene::keyMenuClicked()
{
	log("keyMenuClicked");
}
