#include "GameLight.h"

USING_NS_CC;

GameLight* GameLight::m_instance = nullptr;
ParticleSystemQuad* GameLight::m_particle = nullptr;

GameLight* GameLight::getInstance()
 {
   if(m_instance == nullptr)
       {
	     m_instance = new GameLight();
		 //�������Զ��ͷųأ���Ҫretainһ�� ��Ϊ����û����ӵ�addchild��
		// m_particle = ParticleFlower::create();
		 m_particle = ParticleSystemQuad::create("particles/particle_texture.plist");
		 m_particle->retain();

		 /***********����˵��****
		 һ��ʼ���õ���ParticleSystemQuad ������
		 ParticleFlower Ȼ����getLight()������ ����������һЩ����
		 ������  ��
		  m_particle = ParticleSystem::create("particles/particle_texture.plist");
		 ��������һ���� �������� ����or���� ������ ���ӱ༭���෴ 0��1 ��һ��
		 �ĺú�  �ֱ���  ˵ allocate ������Ŀ ������  ��Ϊû��ʹ�� batchNode
		 �ٶ�˵ ParticleSystemQuad::create("particles/particle_texture.plist"); 
		 OK  �ɹ���
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
