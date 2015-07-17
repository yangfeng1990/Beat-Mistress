#ifndef __FRUIT_SPRITE_H__
#define __FRUIT_SPRITE_H__

#include "cocos2d.h"

USING_NS_CC;

class FruitSprite : public Sprite
{
public:
	CREATE_FUNC(FruitSprite);
	virtual bool init();
	CC_SYNTHESIZE(int, num, Num);

	static FruitSprite* createFruit(int type, String name);

};

#endif // __FRUIT_SPRITE_H__