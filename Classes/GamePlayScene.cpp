#include "GamePlayScene.h"

USING_NS_CC;

// ����������������ĳ���
Scene* GamePlayLayer::createScene()
{
	auto scene = Scene::createWithPhysics();

	PhysicsWorld * phyWorld = scene->getPhysicsWorld();

	// ��������������Ӱ��
	phyWorld->setGravity(Vect(0, 0));

	auto layer = GamePlayLayer::create();

	scene->addChild(layer);

	return scene;
}

bool GamePlayLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	this->initBG();

	return true;
}

void GamePlayLayer::initBG()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// ��ӱ�����ͼ
	auto bg = TMXTiledMap::create("map/blue_bg.tmx");
	this->addChild(bg, 0, GameSceneNodeBatchTagBackground);

	// �������ӱ���
	ParticleSystem * ps = ParticleSystemQuad::create("particle/light.plist");
	ps->setPosition(Vec2(visibleSize.width, visibleSize.height) / 2);
	this->addChild(ps, 0, GameSceneNodeBatchTagBackground);

	// ��������1
	auto sprite1 = Sprite::createWithSpriteFrameName("gameplay.bg.sprite-1.png");
	sprite1->setPosition(Vec2(-50, -50));
	addChild(sprite1, 0, GameSceneNodeBatchTagBackground);

	FiniteTimeAction * ac1 = (FiniteTimeAction*)MoveBy::create(20, Vec2(500, 600));
	FiniteTimeAction * ac2 = ((FiniteTimeAction*)ac1)->reverse();
	ActionInterval * as1 = Sequence::create(ac1, ac2, NULL);
	sprite1->runAction(RepeatForever::create(EaseSineInOut::create(as1)));

	// ��������2
	auto sprite2 = Sprite::createWithSpriteFrameName("gameplay.bg.sprite-2.png");
	sprite2->setPosition(Vec2(visibleSize.width, 0));
	addChild(sprite2, 0, GameSceneNodeBatchTagBackground);

	FiniteTimeAction * ac3 = (FiniteTimeAction*)MoveBy::create(10, Vec2(-500, 600));
	FiniteTimeAction * ac4 = ((FiniteTimeAction*)ac3)->reverse();
	ActionInterval * as2 = Sequence::create(ac3, ac4, NULL);
	sprite2->runAction(RepeatForever::create(EaseExponentialInOut::create(as2)));
}

void GamePlayLayer::onEnter()
{
	Layer::onEnter();

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// ��ʼ����ͣ��ť
	auto pauseSprite = Sprite::createWithSpriteFrameName("gameplay.button.pause.png");
	auto pauseMenuItem = MenuItemSprite::create(pauseSprite, pauseSprite, CC_CALLBACK_1(GamePlayLayer::menuPauseCallback, this));
	auto pauseMenu = Menu::create(pauseMenuItem, NULL);
	pauseMenu->setPosition(Vec2(30, visibleSize.height - 28));
	this->addChild(pauseMenu, 20, 999);

	// �����ʯ1
	auto stone1 = Enemy::createWithEnemyTypes(EnemyTypeStone);
	stone1->setVelocity(Vec2(0, -100));
	this->addChild(stone1, 10, GameSceneNodeBatchTagEnemy);

	// �������
	auto planet = Enemy::createWithEnemyTypes(EnemyTypePlanet);
	planet->setVelocity(Vec2(0, -50));
	this->addChild(planet, 10, GameSceneNodeBatchTagEnemy);

	// ��ӵл�1
	auto enemyFighter1 = Enemy::createWithEnemyTypes(EnemyTypeEnemy1);
	enemyFighter1->setVelocity(Vec2(0, -80));
	this->addChild(enemyFighter1, 10, GameSceneNodeBatchTagEnemy);

	// ��ӵл�2
	auto enemyFighter2 = Enemy::createWithEnemyTypes(EnemyTypeEnemy2);
	enemyFighter2->setVelocity(Vec2(0, -100));
	this->addChild(enemyFighter2, 10, GameSceneNodeBatchTagEnemy);

	// ��ҷɻ�
	this->fighter = Fighter::createWithSpriteFrameName("gameplay.fighter.png");
	this->fighter->setHitPoints(5);
	this->fighter->setPosition(Vec2(visibleSize.width / 2, 70));
	this->addChild(this->fighter, 10, GameSceneNodeTagFighter);

	// ע�ᴥ���ɻ��¼�������
	touchFighterListener = EventListenerTouchOneByOne::create();
	touchFighterListener->setSwallowTouches(true);

	// �ƶ��ɻ�
	touchFighterListener->onTouchBegan = [](Touch * touch, Event * event)
	{
		return true;
	};

	touchFighterListener->onTouchMoved = [](Touch * touch, Event * event)
	{
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() + touch->getDelta());
	};

	// ��Ӵ����ɻ��¼�������
	EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchFighterListener, this->fighter);



	

}

void GamePlayLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();

	UserDefault * defaults = UserDefault::getInstance();

	if (defaults->getBoolForKey(MUSIC_KEY))
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_2, true);
	}
}

void GamePlayLayer::onExit()
{
	Layer::onExit();


	// ע���¼�������
	Director::getInstance()->getEventDispatcher()->removeEventListener(touchFighterListener);

	// ѭ������NodeԪ�أ��Ƴ����Ǳ�����ǩ��NodeԪ��
	auto nodes = this->getChildren();
	for (const auto& node : nodes)
	{
		if (node->getTag() != GameSceneNodeBatchTagBackground)
		{
			this->removeChild(node);
		}
	}
}

void GamePlayLayer::menuPauseCallback(Ref* pSender)
{
	log("menuPauseCallback");

	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	// ��ͣ��ǰ���е�node
	this->pause();

	for (const auto& node : this->getChildren())
	{
		node->pause();
	}

	// �������˵�
	auto backNormal = Sprite::createWithSpriteFrameName("gameplay.button.back.png");
	auto backSelected = Sprite::createWithSpriteFrameName("gameplay.button.back-on.png");
	auto backMenuItem = MenuItemSprite::create(backNormal, backSelected,
		CC_CALLBACK_1(GamePlayLayer::menuBackCallback, this));

	// ������Ϸ
	auto resumeNormal = Sprite::createWithSpriteFrameName("gameplay.button.resume.png");
	auto resumeSelected = Sprite::createWithSpriteFrameName("gameplay.button.resume-on.png");
	auto resumeMenuItem = MenuItemSprite::create(resumeNormal, resumeSelected,
		CC_CALLBACK_1(GamePlayLayer::menuResumeCallback, this));

	menu = Menu::create(backMenuItem, resumeMenuItem, NULL);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	addChild(menu, 20, 1000);
}

void GamePlayLayer::menuBackCallback(Ref* pSender)
{
	log("menuBackCallback");

	Director::getInstance()->popScene();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
}

void GamePlayLayer::menuResumeCallback(Ref* pSender)
{
	log("menuResumeCallback");

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	this->resume();

	for (const auto& node : this->getChildren())
	{
		node->resume();
	}

	this->removeChild(menu);
}