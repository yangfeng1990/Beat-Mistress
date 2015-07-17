#include "GameLayer.h"
#include "Audio.h"
#include "WelcomeScene.h"
#include "GameLight.h"

USING_NS_CC;

float GameLayer::throwtime = 1.5f;

bool GameLayer::init()
{

	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	//2.init background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* background = Sprite::create("background.jpg");
	background->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(background,-1);

	//2.3��ӷ��ذ�ť
	backbar = Sprite::create("back.png");
	addChild(backbar);
	backbar->setAnchorPoint(Vec2(1,0));
	backbar->setPosition(800,0);

	//3��ˮ�������������
	initFruit();

	//��ʼ����ʱ����Ҫ��ʱ��
	currenttime = 0;
	/*********������ʱ��**********/
	scheduleUpdate();
	log("weizhang after scheduleupdate: %d", 0);
	/*****************������������*****************/
	auto touchListener=EventListenerTouchOneByOne::create();  
	touchListener->onTouchBegan=CC_CALLBACK_2(GameLayer::onTouchBegan,this);
	touchListener->onTouchMoved=CC_CALLBACK_2(GameLayer::onTouchMoved,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(GameLayer::onTouchEnded,this);  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);


	//��ȡ���ݿ�����
	UserDefault* user = UserDefault::getInstance();
	bestscore = user->getIntegerForKey("BestScore",0);
	//��ȡ���ݿ���߷֣���ʾ�ڽ�����   //һ��ʼ��ʾ����Ϸ�������ڴ�û�г�ʼ���������һ��  ע�Ⱑ
	auto labelBestScore = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Best:%d",bestscore)->getCString());
	labelBestScore->setColor(Color3B(0,255,0));
	labelBestScore->setAnchorPoint(Vec2(0,0));
	addChild(labelBestScore);
	labelBestScore->setPosition(0,420);

	//��ʼ����Ϸ�÷�
	gamescore =0;
	labelGameScore = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Score:%d",gamescore)->getCString());
	labelGameScore->setColor(Color3B(0,255,0));
	labelGameScore->setAnchorPoint(Vec2(0,0));
	addChild(labelGameScore);
	labelGameScore->setPosition(0,380);

	//ÿ�뵹��ʱ
	deletetime =0;
	timelabel = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Time:%d",deletetime)->getCString());
	timelabel->setColor(Color3B(0,255,0));
	//timelabel->setAnchorPoint(Vec2(0,0));
	addChild(timelabel);
	timelabel->setPosition(400,450);

	//��ʼ����Ϸ״̬
	currentstate =gamestae::start;

	//��Ӵ�����
	dropfruit =0 ;
	auto x = Sprite::createWithSpriteFrameName("x.png");
	auto xx = Sprite::createWithSpriteFrameName("xx.png");
	auto xxx = Sprite::createWithSpriteFrameName("xxx.png");

	addChild(x);x->setPosition(720,450);
	addChild(xx);xx->setPosition(745,450);
	addChild(xxx);xxx->setPosition(775,450);

	return true;
}
void GameLayer::update(float t)
{
	/*********��ʱ��ˮ��***********/
	log("weizhang update t: %d", t);
	currenttime += t;
	if(currenttime>throwtime)
	{
		throwFruit();
		currenttime = 0;
	}
	if(currenttime>0.8f&&currenttime-0.8<0.02)
	{
		throwFruitX();

	}
	log("weizhang update currenttime: %d", currenttime);
	/*********60�뵹��ʱ***********/
	deletetime +=t;
	if(deletetime/1<60)
	{
		int temp = deletetime/1;
		log("time :%d",1.666/2);
		timelabel->setString(String::createWithFormat("Time:%d",60-temp)->getCString());
		//deletetime =0;

	}else
	{/***********�ж���Ϸ����**********/
		deletetime =0;
		timelabel->setString(String::createWithFormat("Time:%d",0)->getCString());
		gameOver();

	}
	/**********��ʱ�Ƴ������ˮ��*********/
	checkRemove();
}

void GameLayer::throwFruitX()
{

	int count = sizeof(FRUIT_DATA) / sizeof(FRUIT_DATA[0]);
	/***********��ʼ��������ӣ������ȡһ��ˮ����ը��************/
	srand((int)time(0)+100);
	count = rand() % count;
	log("fruit num count:%d",count);
	/*************����ˮ��************/
	auto tempFruit = FruitSprite::createFruit(count,FRUIT_DATA[count].fruitImage);
	//ˮ���������
	auto x= 600+rand()%200;
	tempFruit->setPosition(Vec2(x, 50));
	this->addChild(tempFruit);

	//���ñ���������
	auto topY = 350 + rand()%100;

	ccBezierConfig bezier1;
	bezier1.controlPoint_1 = Point(0, topY);//ˮ���������
	bezier1.controlPoint_2 = Point(800-2*x, topY);
	/*bezier1.endPosition = Point(800-2*x, -100);*/
	bezier1.endPosition = Point(800-2*x, 2000);
	auto bezierBy1 = BezierBy::create(3.0f, bezier1);

	//����ˮ����ת�Ƕ�
	auto rotateby =RotateBy::create(3.0f,rand()%360);
	//�ϲ�����Ч��
	auto spawn = Spawn::create(rotateby,bezierBy1,nullptr);

	tempFruit->runAction(spawn);

	//����ˮ����������
	fruitVector.pushBack(tempFruit);
	//������Ч
	//Audio::getInstance()->playThrow();
}

//���ö�ʱ��ÿ��һ��ʱ�俪ʼ��ˮ��
void GameLayer::throwFruit()
{
	int count = sizeof(FRUIT_DATA) / sizeof(FRUIT_DATA[0]);
	/***********��ʼ��������ӣ������ȡһ��ˮ����ը��************/
	srand((int)time(0));
	count = rand() % count;
	log("fruit num count:%d",count);
	/*************����ˮ��************/
	auto tempFruit = FruitSprite::createFruit(count,FRUIT_DATA[count].fruitImage);
	//ˮ���������
	auto x= 100+rand()%200;
	tempFruit->setPosition(Vec2(x, 50));
	this->addChild(tempFruit);

	//���ñ���������
	auto topY = 350 + rand()%100;

	ccBezierConfig bezier1;
	bezier1.controlPoint_1 = Point(0, topY);//ˮ���������
	bezier1.controlPoint_2 = Point(800-2*x, topY);
	/*bezier1.endPosition = Point(800-2*x, -100);*/
	bezier1.endPosition = Point(800-2*x, 2000);
	auto bezierBy1 = BezierBy::create(3.0f, bezier1);

	//����ˮ����ת�Ƕ�
	auto rotateby =RotateBy::create(3.0f,rand()%360);
	//�ϲ�����Ч��
	auto spawn = Spawn::create(rotateby,bezierBy1,nullptr);

	tempFruit->runAction(spawn);

	//����ˮ����������
	fruitVector.pushBack(tempFruit);
	//������Ч
	//Audio::getInstance()->playThrow();
}

//ˮ�������¼�����
bool GameLayer::onTouchBegan(Touch *pTouch ,Event *pEvent )
{
	/************�ӵ������о�û��Ч��************/
	/************���������ж���Ϸ�Ƿ����ö��***********/
	if(currentstate==gamestae::end)
		return false;
	/**************��Ϸû�н���ִ�����涯��**************/
	begin_point=pTouch->getLocation();
	if(backbar->getBoundingBox().containsPoint(begin_point))
	{
		backbar->runAction (ScaleTo::create(0.2,1.2));
		unscheduleAllSelectors();
		Director::getInstance()->replaceScene(WelcomeScene::create());

	}

	m_particle = GameLight::getInstance()->getLight();
	m_particle->setPosition(begin_point);
	m_particle->resetSystem();
	addChild(m_particle,10);

	return true;
}

void GameLayer::onTouchMoved(Touch *pTouch ,Event *pEvent )
{

	if(currentstate==gamestae::end)
		return ;

	//auto diff =	pTouch->getDelta();
	move_point=pTouch->getLocation();
	// move_point = begin_point+diff;
	//creatflash();


	/********************�ı����ӵ���λ��**************************/
	m_particle->setPosition(move_point);

	for(auto obj :fruitVector)
		if(obj->getBoundingBox().containsPoint(move_point))
		{
			obj->stopAllActions();
			obj->setScale(0);

			//�ж��Ƿ���ը��
			if(obj->getNum()==5)
			{
				//�ı���Ϸ״̬
				currentstate=gamestae::end;
				//���ű�ը��Ч
				//Audio::getInstance()->playBoom();
				auto light = Sprite::createWithSpriteFrameName(FRUIT_DATA[obj->getNum()].cutImage[0].getCString());
				addChild(light,10);
				light->setPosition(obj->getPosition());

				light->runAction(Sequence::create(
					Spawn::create(ScaleTo::create(1, 10),RotateBy::create(1, 360),nullptr),		
					CallFunc::create(CC_CALLBACK_0(GameLayer::gameOver,this)),
					//CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)),
					nullptr));


			}else
			{
				//Audio::getInstance()->playSplatter();
				showScoreTips(obj->getPosition(),FRUIT_DATA[obj->getNum()].score);
				/*************�����ۼ�,��������Ϸ����************/
				gamescore += FRUIT_DATA[obj->getNum()].score;
				labelGameScore->setString(String::createWithFormat("Score:%d", gamescore)->getCString());

				/**********��ӷֶζ���*********/
				Sprite* cut_sprite_0 =Sprite::createWithSpriteFrameName(FRUIT_DATA[obj->getNum()].cutImage[0].getCString());
				Sprite* cut_sprite_1 =Sprite::createWithSpriteFrameName(FRUIT_DATA[obj->getNum()].cutImage[1].getCString());
				/*********ֻ���ڲ�������˾���󣬺����ִ�ж�����*********/
				addChild(cut_sprite_0);
				addChild(cut_sprite_1);
				cut_sprite_0 ->setPosition(obj->getPosition());
				cut_sprite_0->setRotation(obj->getRotation());
				cut_sprite_1->setPosition(obj->getPosition());
				cut_sprite_1->setRotation(obj->getRotation());



				/**************����ˮ������ۼ�**********/
				ccBezierConfig bezier1;
				bezier1.controlPoint_1 = Point(obj->getPosition().x-200,obj->getPosition().y);//ˮ���������
				bezier1.controlPoint_2 =Point(obj->getPosition().x-200,obj->getPosition().y);
				bezier1.endPosition = Point(obj->getPosition().x-200,0);
				auto bezierBy1 = BezierTo::create(1.0f, bezier1);

				/**************����ˮ������ۼ�**********/
				ccBezierConfig bezier2;
				bezier2.controlPoint_1 = Point(obj->getPosition().x+200,obj->getPosition().y);//ˮ���������
				bezier2.controlPoint_2 = Point(obj->getPosition().x+200,obj->getPosition().y);
				bezier2.endPosition = Point(obj->getPosition().x+200,0);
				auto bezierBy2 = BezierTo::create(1.0f, bezier2);

				auto path1 =Sequence::create(bezierBy1,CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);
				auto path2 =Sequence::create(bezierBy2,CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);

				cut_sprite_0->runAction(path1);
				cut_sprite_1->runAction(path2);


			}

			//break ��Ҫ��Ϊ�˼ӿ�����Ч��

			obj->setPositionY(-30);//��������Ϊ�� ��ˮ���ں��� �� ������Զ� ����
			break;
		}
		begin_point	= move_point ;
}

void GameLayer::onTouchEnded(Touch *pTouch ,Event *pEvent )
{
	//begin_point=pTouch->getLocation();
	m_particle->removeFromParent();
}

void GameLayer::showScoreTips(Vec2 vec,int num)
{
	/*************����ķ���Ҳ����ѧϰһ��****************/
	//auto label = LabelTTF::create(Value(num).asString(), "Arial", 30);

	auto  label = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("+ %d",num)->getCString());

	label->setPosition(vec);
	addChild(label);
	label->setColor(Color3B(0, 255, 0));
	label->runAction(Sequence::create(DelayTime::create(0.8),CallFuncN::create(CC_CALLBACK_1( GameLayer::selfremove,this)),nullptr));
}

//��Ϸ��������
void GameLayer::gameOver()
{

	auto gameover = Sprite::createWithSpriteFrameName("game-over.png");
	gameover->setPosition(400,240);
	addChild(gameover,10);
	gameover->runAction(ScaleTo::create(0.5,1.5));
	if(gamescore>bestscore)
		UserDefault::getInstance()->setIntegerForKey("BestScore",gamescore);
	log("Game Over");
	this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create(this,callfunc_selector(GameLayer::hello)),nullptr));
}

void GameLayer::hello()
{
	Director::getInstance()->replaceScene(WelcomeScene::create());
}


/**************��������Ч��***************/
void GameLayer::creatflash()
{
	//Sprite* flash =Sprite::createWithSpriteFrameName("flash.png");
	Sprite* flash =Sprite::create("flash.png");
	//����б��
	auto k =(move_point.y-begin_point.y)/(move_point.x-begin_point.x);
	//���㻡��
	auto angle = (atan(k)/(M_PI))*180;
	flash->setRotation(-angle);
	//flash->setScale(0.5);
	this->addChild(flash);
	flash->setPosition(move_point);

	auto path = Sequence::create(DelayTime::create(0.3f),CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);
	flash->runAction(path);

}

//���ڴ������������Ч��
void GameLayer::selfremove(Ref* ref)
{
	Sprite* a = (Sprite*)ref;
	a->removeFromParent();
}
//��ʱ�������ˮ���Ƿ��Ѿ���أ��������
void GameLayer::checkRemove()
{


	for(auto obj : fruitVector)
	{

		if(obj->getPositionY()/1 == -50)
		{

			//�������������ָ���ˣ���Ϊ���ͷ���
			if(obj->getNum() != 5)
			{
				dropfruit++;
				//��������Ч��
				auto move=	Sequence::create(MoveBy::create(0.05,Vec2(-10,0)),MoveBy::create(0.05,Vec2(+10,0)),MoveBy::create(0.05,Vec2(0,10)),MoveBy::create(0.05,Vec2(0,-10)),DelayTime::create(0.5),CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)),nullptr);
				auto lose = Sprite::createWithSpriteFrameName("lose.png");
				lose->setPosition(700,30);
				addChild(lose);
				lose->runAction(move);

				/**********û�е���ˮ������ͳ��**************/

				if(dropfruit==1)
				{

					auto xf =Sprite::createWithSpriteFrameName("xf.png");
					xf->setPosition(720,450);
					addChild(xf);
					xf->runAction(Sequence::create(MoveBy::create(0.05,Vec2(-10,0)),MoveBy::create(0.05,Vec2(+10,0)),MoveBy::create(0.05,Vec2(0,10)),MoveBy::create(0.05,Vec2(0,-10)),nullptr));
				}
				if(dropfruit==2)
				{
					auto xxf =Sprite::createWithSpriteFrameName("xxf.png");
					xxf->setPosition(745,450);
					addChild(xxf);
					xxf->runAction(Sequence::create(MoveBy::create(0.05,Vec2(-10,0)),MoveBy::create(0.05,Vec2(+10,0)),MoveBy::create(0.05,Vec2(0,10)),MoveBy::create(0.05,Vec2(0,-10)),nullptr));
				}
				if(dropfruit==3)
				{
					auto xxxf =Sprite::createWithSpriteFrameName("xxxf.png");
					xxxf->setPosition(775,450);
					addChild(xxxf);
					currentstate=gamestae::end;
					xxxf->runAction(Sequence::create(MoveBy::create(0.05,Vec2(-10,0)),MoveBy::create(0.05,Vec2(+10,0)),MoveBy::create(0.05,Vec2(0,10)),MoveBy::create(0.05,Vec2(0,-10)),DelayTime::create(0.5),CallFunc::create(CC_CALLBACK_0(GameLayer::gameOver,this)),nullptr));

				}
			}
			fruitVector.eraseObject(obj);
			obj->removeFromParent();

			//�Ƴ����µ�ˮ����ͼ,�����ٶ�̫����
			//lose->removeFromParent();
		}  
		else /*********һ��ʼû�м�else ���¿�ָ��*******/
			/***********���Ǳ��е�������Ĭ��λ��********/
			if(obj->getPositionY()/1 == -30)
			{
				fruitVector.eraseObject(obj);
				obj->removeFromParent();
			}

			break;
	}
}
//ˮ�������������
void GameLayer::initFruit()
{
	//FRUIT_DATA[0] = (FRUIT){0,1,"apple.png",{"apple-1.png","apple-2.png"}};
	/*FRUIT_DATA[0].num=0;
	FRUIT_DATA[0].score=1;
	FRUIT_DATA[0].fruitImage="apple.png";
	FRUIT_DATA[0].cutImage[0] = "apple-1.png";
	FRUIT_DATA[0].cutImage[1] = "apple-2.png";

	FRUIT_DATA[1].num=1;
	FRUIT_DATA[1].score=2;
	FRUIT_DATA[1].fruitImage="banana.png";
	FRUIT_DATA[1].cutImage[0] = "banana-2.png";
	FRUIT_DATA[1].cutImage[1] = "banana-1.png";

	FRUIT_DATA[2].num=2;
	FRUIT_DATA[2].score=3;
	FRUIT_DATA[2].fruitImage="basaha.png";
	FRUIT_DATA[2].cutImage[0] = "basaha-1.png";
	FRUIT_DATA[2].cutImage[1] = "basaha-2.png";

	FRUIT_DATA[3].num=3;
	FRUIT_DATA[3].score=4;
	FRUIT_DATA[3].fruitImage="peach.png";
	FRUIT_DATA[3].cutImage[0] = "peach-1.png";
	FRUIT_DATA[3].cutImage[1] = "peach-2.png";

	FRUIT_DATA[4].num=4;
	FRUIT_DATA[4].score=5;
	FRUIT_DATA[4].fruitImage="sandia.png";
	FRUIT_DATA[4].cutImage[0] = "sandia-1.png";
	FRUIT_DATA[4].cutImage[1] = "sandia-2.png"; */

	/**********ը��ͼƬ************/
	/*FRUIT_DATA[5].num=5;
	FRUIT_DATA[5].score=6;
	FRUIT_DATA[5].fruitImage="boom.png";
	FRUIT_DATA[5].cutImage[0] = "boomlight1.png";
	FRUIT_DATA[5].cutImage[1] = "boomlight1.png";*/

	FRUIT_DATA[0].num=0;
	FRUIT_DATA[0].score=1;
	FRUIT_DATA[0].fruitImage="girl1.png";
	FRUIT_DATA[0].cutImage[0] = "ghost.png";
	FRUIT_DATA[0].cutImage[1] = "ghost.png";

	FRUIT_DATA[1].num=1;
	FRUIT_DATA[1].score=2;
	FRUIT_DATA[1].fruitImage="girl2.png";
	FRUIT_DATA[1].cutImage[0] = "ghost.png";
	FRUIT_DATA[1].cutImage[1] = "ghost.png";

	FRUIT_DATA[2].num=2;
	FRUIT_DATA[2].score=3;
	FRUIT_DATA[2].fruitImage="girl3.png";
	FRUIT_DATA[2].cutImage[0] = "ghost.png";
	FRUIT_DATA[2].cutImage[1] = "ghost.png";

	FRUIT_DATA[3].num=3;
	FRUIT_DATA[3].score=4;
	FRUIT_DATA[3].fruitImage="girl4.png";
	FRUIT_DATA[3].cutImage[0] = "ghost.png";
	FRUIT_DATA[3].cutImage[1] = "ghost.png";
}

