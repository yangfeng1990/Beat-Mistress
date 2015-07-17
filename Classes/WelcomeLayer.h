#ifndef __WELCOME_LAYER_H__
#define __WELCOME_LAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class WelcomeLayer: public cocos2d::Layer
{
public:
	CREATE_FUNC(WelcomeLayer);
	virtual bool init();

	Vector<Sprite*> rings;
	Vector<Sprite*> fruits;

	// add touch events
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	// del self method
	void selfremove(Ref* ref);
	// add slide flash
	void createflash();
	Point begin_point;
	Point move_point;
	// enter game
	void goMainScene(float dt);
	// action testing
	RotateBy* mook;
};

#endif // __WELCOME_LAYER_H__