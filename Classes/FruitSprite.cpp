#include "FruitSprite.h"

USING_NS_CC;

bool FruitSprite::init()
{
	if (!Sprite::init())
	{
		return false;
	}

	this->num = 0;

	return true;
}

FruitSprite* FruitSprite::createFruit(int type, String name)
{
	FruitSprite* ret = FruitSprite::create();
	ret->initWithSpriteFrameName(name.getCString());
	ret->setNum(type);

	return ret;
}