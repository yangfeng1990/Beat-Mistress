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

	/**********���ʵ�尴������************/
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased,this); 
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent) 
{
	// ���ؼ�
	if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		this->keyBackClicked();
	} 
	//��Ӧ�˵���
	else if (keyCode == EventKeyboard::KeyCode::KEY_MENU)     
	{              
		this->keyMenuClicked();
	} 
	//  Director::getInstance()->end();  okע��������ʱһ��ok ��������OK
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
