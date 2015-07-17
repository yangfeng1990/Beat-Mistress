#include "GameLight.h"

USING_NS_CC;

GameLight* GameLight::m_instance = nullptr;
ParticleSystemQuad* GameLight::m_particle = nullptr;

GameLight* GameLight::getInstance()
 {
   if(m_instance == nullptr)
       {
	     m_instance = new GameLight();
		 //下面是自动释放池，需要retain一下 因为这里没有添加到addchild中
		// m_particle = ParticleFlower::create();
		 m_particle = ParticleSystemQuad::create("particles/particle_texture.plist");
		 m_particle->retain();

		 /***********过程说明****
		 一开始我用的是ParticleSystemQuad 的子类
		 ParticleFlower 然后在getLight()方法中 重新配置了一些参数
		 后来我  用
		  m_particle = ParticleSystem::create("particles/particle_texture.plist");
		 两个错误：一个是 粒子类型 重力or辐射 好像与 粒子编辑器相反 0和1 换一下
		 改好后  又报错  说 allocate 粒子数目 超标了  因为没有使用 batchNode
		 百度说 ParticleSystemQuad::create("particles/particle_texture.plist"); 
		 OK  成功了
		 *********/
		 

	    }
   return m_instance;
 
 }

ParticleSystemQuad* GameLight::getLight()
{
    
  /*  m_particle->setTexture( Director::getInstance()->getTextureCache()->addImage("stars.png") );
	m_particle->setLife(1);
	m_particle->setEndSize(10);
	m_particle->setSpeed(150);
	m_particle->setRadialAccel(0.5);
	m_particle->setTotalParticles(150);*/

	return m_particle;

}
