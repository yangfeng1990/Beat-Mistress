#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Scene
{
public:
	CREATE_FUNC(GameScene);
	virtual bool init();

	virtual void keyBackClicked();
	virtual void keyMenuClicked();

	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* pEvent);
};
#endif // __GAME_SCENE_H__