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

	//2.3添加返回按钮
	backbar = Sprite::create("back.png");
	addChild(backbar);
	backbar->setAnchorPoint(Vec2(1,0));
	backbar->setPosition(800,0);

	//3、水果数组分配数据
	initFruit();

	//初始化定时器需要的时间
	currenttime = 0;
	/*********开启定时器**********/
	scheduleUpdate();
	log("weizhang after scheduleupdate: %d", 0);
	/*****************开启触摸监听*****************/
	auto touchListener=EventListenerTouchOneByOne::create();  
	touchListener->onTouchBegan=CC_CALLBACK_2(GameLayer::onTouchBegan,this);
	touchListener->onTouchMoved=CC_CALLBACK_2(GameLayer::onTouchMoved,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(GameLayer::onTouchEnded,this);  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);


	//获取数据库内容
	UserDefault* user = UserDefault::getInstance();
	bestscore = user->getIntegerForKey("BestScore",0);
	//读取数据库最高分，显示在界面中   //一开始显示的游戏分数，内存没有初始化，随机数一个  注意啊
	auto labelBestScore = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Best:%d",bestscore)->getCString());
	labelBestScore->setColor(Color3B(0,255,0));
	labelBestScore->setAnchorPoint(Vec2(0,0));
	addChild(labelBestScore);
	labelBestScore->setPosition(0,420);

	//初始化游戏得分
	gamescore =0;
	labelGameScore = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Score:%d",gamescore)->getCString());
	labelGameScore->setColor(Color3B(0,255,0));
	labelGameScore->setAnchorPoint(Vec2(0,0));
	addChild(labelGameScore);
	labelGameScore->setPosition(0,380);

	//每秒倒计时
	deletetime =0;
	timelabel = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("Time:%d",deletetime)->getCString());
	timelabel->setColor(Color3B(0,255,0));
	//timelabel->setAnchorPoint(Vec2(0,0));
	addChild(timelabel);
	timelabel->setPosition(400,450);

	//初始化游戏状态
	currentstate =gamestae::start;

	//添加错误数
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
	/*********定时抛水果***********/
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
	/*********60秒倒计时***********/
	deletetime +=t;
	if(deletetime/1<60)
	{
		int temp = deletetime/1;
		log("time :%d",1.666/2);
		timelabel->setString(String::createWithFormat("Time:%d",60-temp)->getCString());
		//deletetime =0;

	}else
	{/***********判定游戏结束**********/
		deletetime =0;
		timelabel->setString(String::createWithFormat("Time:%d",0)->getCString());
		gameOver();

	}
	/**********定时移除掉落的水果*********/
	checkRemove();
}

void GameLayer::throwFruitX()
{

	int count = sizeof(FRUIT_DATA) / sizeof(FRUIT_DATA[0]);
	/***********初始化随机种子，随机获取一个水果或炸弹************/
	srand((int)time(0)+100);
	count = rand() % count;
	log("fruit num count:%d",count);
	/*************配置水果************/
	auto tempFruit = FruitSprite::createFruit(count,FRUIT_DATA[count].fruitImage);
	//水果抛掷起点
	auto x= 600+rand()%200;
	tempFruit->setPosition(Vec2(x, 50));
	this->addChild(tempFruit);

	//配置贝塞尔曲线
	auto topY = 350 + rand()%100;

	ccBezierConfig bezier1;
	bezier1.controlPoint_1 = Point(0, topY);//水果抛掷起点
	bezier1.controlPoint_2 = Point(800-2*x, topY);
	/*bezier1.endPosition = Point(800-2*x, -100);*/
	bezier1.endPosition = Point(800-2*x, 2000);
	auto bezierBy1 = BezierBy::create(3.0f, bezier1);

	//配置水果旋转角度
	auto rotateby =RotateBy::create(3.0f,rand()%360);
	//合并动画效果
	auto spawn = Spawn::create(rotateby,bezierBy1,nullptr);

	tempFruit->runAction(spawn);

	//管理水果精灵容器
	fruitVector.pushBack(tempFruit);
	//播放音效
	//Audio::getInstance()->playThrow();
}

//利用定时器每隔一段时间开始扔水果
void GameLayer::throwFruit()
{
	int count = sizeof(FRUIT_DATA) / sizeof(FRUIT_DATA[0]);
	/***********初始化随机种子，随机获取一个水果或炸弹************/
	srand((int)time(0));
	count = rand() % count;
	log("fruit num count:%d",count);
	/*************配置水果************/
	auto tempFruit = FruitSprite::createFruit(count,FRUIT_DATA[count].fruitImage);
	//水果抛掷起点
	auto x= 100+rand()%200;
	tempFruit->setPosition(Vec2(x, 50));
	this->addChild(tempFruit);

	//配置贝塞尔曲线
	auto topY = 350 + rand()%100;

	ccBezierConfig bezier1;
	bezier1.controlPoint_1 = Point(0, topY);//水果抛掷起点
	bezier1.controlPoint_2 = Point(800-2*x, topY);
	/*bezier1.endPosition = Point(800-2*x, -100);*/
	bezier1.endPosition = Point(800-2*x, 2000);
	auto bezierBy1 = BezierBy::create(3.0f, bezier1);

	//配置水果旋转角度
	auto rotateby =RotateBy::create(3.0f,rand()%360);
	//合并动画效果
	auto spawn = Spawn::create(rotateby,bezierBy1,nullptr);

	tempFruit->runAction(spawn);

	//管理水果精灵容器
	fruitVector.pushBack(tempFruit);
	//播放音效
	//Audio::getInstance()->playThrow();
}

//水果触摸事件处理
bool GameLayer::onTouchBegan(Touch *pTouch ,Event *pEvent )
{
	/************坑爹啊，感觉没有效果************/
	/************这里用来判断游戏是否结束枚举***********/
	if(currentstate==gamestae::end)
		return false;
	/**************游戏没有结束执行下面动作**************/
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


	/********************改变粒子刀痕位置**************************/
	m_particle->setPosition(move_point);

	for(auto obj :fruitVector)
		if(obj->getBoundingBox().containsPoint(move_point))
		{
			obj->stopAllActions();
			obj->setScale(0);

			//判断是否是炸弹
			if(obj->getNum()==5)
			{
				//改变游戏状态
				currentstate=gamestae::end;
				//播放爆炸音效
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
				/*************分数累加,，更新游戏分数************/
				gamescore += FRUIT_DATA[obj->getNum()].score;
				labelGameScore->setString(String::createWithFormat("Score:%d", gamescore)->getCString());

				/**********添加分段动画*********/
				Sprite* cut_sprite_0 =Sprite::createWithSpriteFrameName(FRUIT_DATA[obj->getNum()].cutImage[0].getCString());
				Sprite* cut_sprite_1 =Sprite::createWithSpriteFrameName(FRUIT_DATA[obj->getNum()].cutImage[1].getCString());
				/*********只有在层上添加了精灵后，后面才执行动作能*********/
				addChild(cut_sprite_0);
				addChild(cut_sprite_1);
				cut_sprite_0 ->setPosition(obj->getPosition());
				cut_sprite_0->setRotation(obj->getRotation());
				cut_sprite_1->setPosition(obj->getPosition());
				cut_sprite_1->setRotation(obj->getRotation());



				/**************创建水果掉落痕迹**********/
				ccBezierConfig bezier1;
				bezier1.controlPoint_1 = Point(obj->getPosition().x-200,obj->getPosition().y);//水果抛掷起点
				bezier1.controlPoint_2 =Point(obj->getPosition().x-200,obj->getPosition().y);
				bezier1.endPosition = Point(obj->getPosition().x-200,0);
				auto bezierBy1 = BezierTo::create(1.0f, bezier1);

				/**************创建水果掉落痕迹**********/
				ccBezierConfig bezier2;
				bezier2.controlPoint_1 = Point(obj->getPosition().x+200,obj->getPosition().y);//水果抛掷起点
				bezier2.controlPoint_2 = Point(obj->getPosition().x+200,obj->getPosition().y);
				bezier2.endPosition = Point(obj->getPosition().x+200,0);
				auto bezierBy2 = BezierTo::create(1.0f, bezier2);

				auto path1 =Sequence::create(bezierBy1,CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);
				auto path2 =Sequence::create(bezierBy2,CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);

				cut_sprite_0->runAction(path1);
				cut_sprite_1->runAction(path2);


			}

			//break 主要是为了加快运行效率

			obj->setPositionY(-30);//这样做事为了 让水果在后面 被 清除机自动 消除
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
	/*************下面的方法也可以学习一下****************/
	//auto label = LabelTTF::create(Value(num).asString(), "Arial", 30);

	auto  label = Label::createWithBMFont("fonts/futura-48.fnt", String::createWithFormat("+ %d",num)->getCString());

	label->setPosition(vec);
	addChild(label);
	label->setColor(Color3B(0, 255, 0));
	label->runAction(Sequence::create(DelayTime::create(0.8),CallFuncN::create(CC_CALLBACK_1( GameLayer::selfremove,this)),nullptr));
}

//游戏结束分析
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


/**************创建刀锋效果***************/
void GameLayer::creatflash()
{
	//Sprite* flash =Sprite::createWithSpriteFrameName("flash.png");
	Sprite* flash =Sprite::create("flash.png");
	//计算斜率
	auto k =(move_point.y-begin_point.y)/(move_point.x-begin_point.x);
	//计算弧度
	auto angle = (atan(k)/(M_PI))*180;
	flash->setRotation(-angle);
	//flash->setScale(0.5);
	this->addChild(flash);
	flash->setPosition(move_point);

	auto path = Sequence::create(DelayTime::create(0.3f),CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)), nullptr);
	flash->runAction(path);

}

//用于创建刀光的隐退效果
void GameLayer::selfremove(Ref* ref)
{
	Sprite* a = (Sprite*)ref;
	a->removeFromParent();
}
//定时器，检查水果是否已经落地，将其清除
void GameLayer::checkRemove()
{


	for(auto obj : fruitVector)
	{

		if(obj->getPositionY()/1 == -50)
		{

			//这个放这里，否则空指针了，因为被释放了
			if(obj->getNum() != 5)
			{
				dropfruit++;
				//创建颤抖效果
				auto move=	Sequence::create(MoveBy::create(0.05,Vec2(-10,0)),MoveBy::create(0.05,Vec2(+10,0)),MoveBy::create(0.05,Vec2(0,10)),MoveBy::create(0.05,Vec2(0,-10)),DelayTime::create(0.5),CallFuncN::create(CC_CALLBACK_1(GameLayer::selfremove,this)),nullptr);
				auto lose = Sprite::createWithSpriteFrameName("lose.png");
				lose->setPosition(700,30);
				addChild(lose);
				lose->runAction(move);

				/**********没切掉的水果进行统计**************/

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

			//移除掉下的水果大图,发现速度太快了
			//lose->removeFromParent();
		}  
		else /*********一开始没有加else 导致空指针*******/
			/***********这是被切掉的西瓜默认位置********/
			if(obj->getPositionY()/1 == -30)
			{
				fruitVector.eraseObject(obj);
				obj->removeFromParent();
			}

			break;
	}
}
//水果数组分配数据
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

	/**********炸弹图片************/
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

