#include "WelcomeLayer.h"
#include "Audio.h"
#include "GameScene.h"

USING_NS_CC;

bool WelcomeLayer::init()
{
	// super init first
	if (!Layer::init())
	{
		return false;
	}
	// init background
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* background = Sprite::create("background.jpg");
	background->setPosition(visibleSize.width/2, visibleSize.height/2);
	this->addChild(background, -1);
	// add greybar
	Sprite* greybar = Sprite::create("greybar.png");
	greybar->setAnchorPoint(Vec2(0, 1));
	this->addChild(greybar);
	greybar->setPositionY(visibleSize.height);
	// add jijia.png in greybar
	Sprite* logo = Sprite::createWithSpriteFrameName("logo.png");
	greybar->addChild(logo);
	logo->setAnchorPoint(Vec2(0, 0));
	logo->setPosition(-50, 90);
	logo->runAction(EaseBounceOut::create(MoveTo::create(1, Vec2(50, 90))));   
	// add jijia.png in greybar
	Sprite* ninja = Sprite::createWithSpriteFrameName("ninja.png");
	greybar->addChild(ninja);
	ninja->setAnchorPoint(Vec2(0, 0));
	ninja->setPosition(420, 120);
	// add bord
	Sprite* homedesc = Sprite::createWithSpriteFrameName("home-desc.png");
	this->addChild(homedesc);
	homedesc->setAnchorPoint(Vec2(0, 0));
	homedesc->setPosition(150, 280);
	homedesc->runAction(EaseBounceOut::create(MoveTo::create(1, Vec2(150, 240))));
	/**********************finish title**************************/
	// add ring and fruit
	Sprite* dojoRing = Sprite::createWithSpriteFrameName("dojo.png");
	this->addChild(dojoRing);
	dojoRing->setPosition(150, 150);
	Sprite* setting = Sprite::createWithSpriteFrameName("setting.png");
	dojoRing->addChild(setting);
	setting->setPosition(dojoRing->getContentSize().width/2, dojoRing->getContentSize().height/2);

	Sprite* newgameRing = Sprite::createWithSpriteFrameName("new-game.png");
	this->addChild(newgameRing);
	newgameRing->setPosition(450, 230);
	Sprite* start = Sprite::createWithSpriteFrameName("start.png");
	newgameRing->addChild(start);
	start->setPosition(newgameRing->getContentSize().width/2, newgameRing->getContentSize().height/2);


	Sprite* quitRing = Sprite::createWithSpriteFrameName("quit.png");
	this->addChild(quitRing);
	quitRing->setPosition(650, 120);
	//Sprite* quitboom = Sprite::createWithSpriteFrameName("boom.png");
	Sprite* quitgame = Sprite::createWithSpriteFrameName("quitgame.png");
	quitRing->addChild(quitgame);
	quitgame->setPosition(quitRing->getContentSize().width/2, quitRing->getContentSize().height/2);
	/********************finish option****************************/
	rings.pushBack(dojoRing);
	rings.pushBack(newgameRing);
	rings.pushBack(quitRing);

	mook = RotateBy::create(4, -360);

	newgameRing->runAction(mook);
	quitRing->runAction(mook->clone());
	// dojoRing->runAction(mook->clone());

	fruits.pushBack(setting);
	fruits.pushBack(start);
	fruits.pushBack(quitgame);
	// the action of parent node is effect the child nodes
	// so the rotate is 180, not 90
	for (auto obj : fruits)
	{
		obj->runAction(RepeatForever::create(RotateBy::create(1, 180)));
	}
	// add touch event
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(WelcomeLayer::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(WelcomeLayer::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(WelcomeLayer::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	// set tags
	newgameRing->setTag(60);
	quitRing->setTag(70);
	setting->setTag(100);
	start->setTag(110);
	quitgame->setTag(120);

	return true;
}

bool WelcomeLayer::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	begin_point = pTouch->getLocation();
	return true;
}

void WelcomeLayer::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	move_point = pTouch->getLocation();
	Sprite* newgameRing = (Sprite*)this->getChildByTag(60);
	Rect sandiaRect = newgameRing->getChildByTag(110)->getBoundingBox();
	//getBoundingBox()获取的矩形坐标是相对于父坐标的  获取真实的bindBox坐标！
	Vec2 point = newgameRing->getPosition();
	//位置坐标这样才算对啊  //rect 是（a,a,b,b） a,a是点左下角 bb是宽高 //画图理解
	sandiaRect.origin.x =point.x  -sandiaRect.size.width/2;
	sandiaRect.origin.y =point.y  -sandiaRect.size.height/2;
	/***********如果进入西瓜界面，进入系游戏*********/
	if(sandiaRect.containsPoint(move_point))
	{
		auto sandia = newgameRing->getChildByTag(110);
		//ScaleTo 第一个参数是时间  第二个参数是缩放比例，x，y 值都一样 等比例缩放
		sandia->runAction(ScaleTo::create(0,0));
		//将对象缩放到0相当于隐藏了，这个方法不错啊
		//经过分析，getrotatiion()是相对于父节点的转角的
		float rotation = sandia->getRotation();
		float rotation1= newgameRing->getRotation();
		//auto sandia1 = Sprite::createWithSpriteFrameName("sandia-1.png");
		//auto sandia2 = Sprite::createWithSpriteFrameName("sandia-2.png");
		auto sandia1 = Sprite::createWithSpriteFrameName("start.png");
		auto sandia2 = Sprite::createWithSpriteFrameName("start.png");
		sandia1->setRotation(rotation+rotation1);
		sandia2->setRotation(rotation+rotation1);

		this->addChild(sandia1);sandia1->setPosition(point);
		this->addChild(sandia2);sandia2->setPosition(point);
		//播放切西瓜效果
		//Audio::getInstance()->playSplatter();
		auto path1 =Sequence::create(MoveTo::create(0.2, ccp(point.x - 50.0, point.y+ 50.0)),CallFuncN::create(CC_CALLBACK_1(WelcomeLayer::selfremove,this)), nullptr);
		auto path2 =Sequence::create(MoveTo::create(0.2, ccp(point.x + 50.0, point.y- 50.0)),CallFuncN::create(CC_CALLBACK_1(WelcomeLayer::selfremove,this)), nullptr);
		sandia1->runAction(path1);
		sandia2->runAction(path2);

		scheduleOnce(schedule_selector(WelcomeLayer::goMainScene), 0.5f);
	}
	
	Sprite* quitRing = (Sprite*)this->getChildByTag(70);
	Rect quitBoom =quitRing->getChildByTag(120)->getBoundingBox();
	quitBoom.origin.x=quitRing->getPosition().x-quitBoom.size.width/2;
	quitBoom.origin.y=quitRing->getPosition().y-quitBoom.size.height/2;
	
	if(quitBoom.containsPoint(move_point))
	{
		//Audio::getInstance()->playBoom();
		Director::getInstance()->end();
	}
	/*************调用刀锋效果***************/
	this->createflash();
	begin_point = move_point;
}

void WelcomeLayer::selfremove(Ref* ref)
{
	Sprite* a = (Sprite*)ref;
	a->removeFromParent();
}

void WelcomeLayer::createflash()
{
	Sprite* flash = Sprite::create("flash.png");
	//计算斜率
	auto k = (move_point.y - begin_point.y)/(move_point.x - begin_point.x);
	//计算弧度
	auto angle = (atan(k)/(M_PI))*180;
	flash->setRotation(-angle);
	this->addChild(flash);
	flash->setPosition(move_point);

	auto path = Sequence::create(DelayTime::create(0.3f), CallFuncN::create(CC_CALLBACK_1(WelcomeLayer::selfremove, this)), nullptr);
	flash->runAction(path);
}

void WelcomeLayer::goMainScene(float dt)
{
	log("go main scene");
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	Director::getInstance()->replaceScene(GameScene::create());
}

void WelcomeLayer::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	log("end ref count:%d",mook->getReferenceCount());
}