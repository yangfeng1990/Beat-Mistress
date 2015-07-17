#ifndef __GAME_LIGHT_H__
#define __GAME_LIGHT_H__

#include "cocos2d.h"
USING_NS_CC;

class GameLight
{
public:
	static GameLight* getInstance();
	ParticleSystemQuad* getLight();
private:
	static GameLight* m_instance;
	//static ParticleFlower* m_particle;
	static ParticleSystemQuad* m_particle;
};

#endif // __GAME_LIGHT_H__