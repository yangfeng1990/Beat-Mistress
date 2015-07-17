#ifndef  __GAME_LAYER_H__
#define  __GAME_LAYER_H__

#include "cocos2d.h"
#include "FruitSprite.h"
USING_NS_CC;

/*********����ˮ���ṹ��*************/
struct FRUIT 
{
	int num ;
	int score;
	String fruitImage;
	String cutImage[2];
};

class GameLayer: public cocos2d::Layer
{
public:

	CREATE_FUNC(GameLayer);
	virtual bool init();  
	/*******������ʱ��*********/
	void update(float dt) override;
	/*******���ö�ʱ����ˮ������********/
	void throwFruit();
	void throwFruitX();
	//������ˮ��ʱ��
	static float throwtime;
	//�ж���ǰ֡ʱ��
	float currenttime;
	//ÿ�뵹��ʱ
	float deletetime;

	Label* timelabel;
	//��ʼ��ˮ������  �൱�����ݿ�
	void initFruit();
	//FRUIT FRUIT_DATA[6];
	FRUIT FRUIT_DATA[4];
	//�洢ˮ��������
	Vector<FruitSprite*>  fruitVector;

	//��Ӵ����¼�
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	Point begin_point;
	Point move_point;
	//�������ذ�ť
	Sprite *backbar;
	//�������溯��
	void creatflash();
	void selfremove(Ref* ref);
	//��ʱ�Ƴ������ˮ��
	void checkRemove();
	//չʾˮ���÷�
	void showScoreTips(Vec2 vec,int num);
	//��Ϸ��������
	void gameOver();
	void hello();

	Label* labelGameScore;
	int gamescore;
	int bestscore;

	enum gamestae
	{
		start=0,
		end=1
	};

	enum color{
		GREEN,
		RED,
		YELLOW,
		PURPLE,
		BLUE
	};
	//��¼��ʱ״̬
	int currentstate;
	//��¼©����ˮ����Ŀ
	int dropfruit;

	//��ŵ��۱���
	ParticleSystemQuad*  m_particle;

};

#endif //  __Game_LAYER_H__
