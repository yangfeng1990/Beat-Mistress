#ifndef  __GAME_LAYER_H__
#define  __GAME_LAYER_H__

#include "cocos2d.h"
#include "FruitSprite.h"
USING_NS_CC;

/*********定义水果结构体*************/
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
	/*******开启定时器*********/
	void update(float dt) override;
	/*******利用定时器抛水果函数********/
	void throwFruit();
	void throwFruitX();
	//定义扔水果时间
	static float throwtime;
	//判定当前帧时间
	float currenttime;
	//每秒倒计时
	float deletetime;

	Label* timelabel;
	//初始化水果数据  相当于数据库
	void initFruit();
	//FRUIT FRUIT_DATA[6];
	FRUIT FRUIT_DATA[4];
	//存储水果精灵类
	Vector<FruitSprite*>  fruitVector;

	//添加触摸事件
	virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
	virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
	virtual void onTouchEnded(Touch *pTouch, Event *pEvent);

	Point begin_point;
	Point move_point;
	//创建返回按钮
	Sprite *backbar;
	//创建刀锋函数
	void creatflash();
	void selfremove(Ref* ref);
	//定时移除掉落的水果
	void checkRemove();
	//展示水果得分
	void showScoreTips(Vec2 vec,int num);
	//游戏结束处理
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
	//记录临时状态
	int currentstate;
	//记录漏掉的水果数目
	int dropfruit;

	//存放刀痕变量
	ParticleSystemQuad*  m_particle;

};

#endif //  __Game_LAYER_H__
