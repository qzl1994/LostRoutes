#include "GamePlayScene.h"

USING_NS_CC;

// 创建具有物理世界的场景
Scene* GamePlayLayer::createScene()
{
	auto scene = Scene::createWithPhysics();

	PhysicsWorld * phyWorld = scene->getPhysicsWorld();

	// 设置重力，不受影响
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

// 初始化背景
void GamePlayLayer::initBG()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 添加背景地图
	auto bg = TMXTiledMap::create("map/blue_bg.tmx");
	this->addChild(bg, 0, GameSceneNodeBatchTagBackground);

	// 发光粒子背景
	ParticleSystem * ps = ParticleSystemQuad::create("particle/light.plist");
	ps->setPosition(Vec2(visibleSize.width, visibleSize.height) / 2);
	this->addChild(ps, 0, GameSceneNodeBatchTagBackground);

	// 背景精灵1
	auto sprite1 = Sprite::createWithSpriteFrameName("gameplay.bg.sprite-1.png");
	sprite1->setPosition(Vec2(-50, -50));
	addChild(sprite1, 0, GameSceneNodeBatchTagBackground);

	FiniteTimeAction * ac1 = (FiniteTimeAction*)MoveBy::create(20, Vec2(500, 600));
	FiniteTimeAction * ac2 = ((FiniteTimeAction*)ac1)->reverse();
	ActionInterval * as1 = Sequence::create(ac1, ac2, NULL);
	sprite1->runAction(RepeatForever::create(EaseSineInOut::create(as1)));

	// 背景精灵2
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

	// 初始化暂停按钮
	auto pauseSprite = Sprite::createWithSpriteFrameName("gameplay.button.pause.png");
	auto pauseMenuItem = MenuItemSprite::create(pauseSprite, pauseSprite, CC_CALLBACK_1(GamePlayLayer::menuPauseCallback, this));
	auto pauseMenu = Menu::create(pauseMenuItem, NULL);
	pauseMenu->setPosition(Vec2(30, visibleSize.height - 28));
	this->addChild(pauseMenu, 20, 999);

	// 添加陨石1
	auto stone1 = Enemy::createWithEnemyTypes(EnemyTypeStone);
	stone1->setVelocity(Vec2(0, -100));
	this->addChild(stone1, 10, GameSceneNodeBatchTagEnemy);

	// 添加行星
	auto planet = Enemy::createWithEnemyTypes(EnemyTypePlanet);
	planet->setVelocity(Vec2(0, -50));
	this->addChild(planet, 10, GameSceneNodeBatchTagEnemy);

	// 添加敌机1
	auto enemyFighter1 = Enemy::createWithEnemyTypes(EnemyTypeEnemy1);
	enemyFighter1->setVelocity(Vec2(0, -80));
	this->addChild(enemyFighter1, 10, GameSceneNodeBatchTagEnemy);

	// 添加敌机2
	auto enemyFighter2 = Enemy::createWithEnemyTypes(EnemyTypeEnemy2);
	enemyFighter2->setVelocity(Vec2(0, -100));
	this->addChild(enemyFighter2, 10, GameSceneNodeBatchTagEnemy);

	// 玩家飞机
	this->fighter = Fighter::createWithSpriteFrameName("gameplay.fighter.png");
	this->fighter->setHitPoints(GameSceneFighterLife);
	this->fighter->setPosition(Vec2(visibleSize.width / 2, 70));
	this->addChild(this->fighter, 10, GameSceneNodeTagFighter);

	// 注册触摸飞机事件监听器
	touchFighterListener = EventListenerTouchOneByOne::create();
	touchFighterListener->setSwallowTouches(true);

	// 移动飞机
	touchFighterListener->onTouchBegan = [](Touch * touch, Event * event)
	{
		return true;
	};

	touchFighterListener->onTouchMoved = [](Touch * touch, Event * event)
	{
		auto target = event->getCurrentTarget();
		target->setPosition(target->getPosition() + touch->getDelta());
	};

	// 注册接触事件监听器
	contactListener = EventListenerPhysicsContact::create();

	// 接触事件
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();

		// 飞机与敌人的接触
		Node * enemy1 = nullptr;
		
		if (spriteA->getTag() == GameSceneNodeTagFighter && spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			enemy1 = spriteB;
		}
		if (spriteA->getTag() == GameSceneNodeBatchTagEnemy && spriteB->getTag() == GameSceneNodeTagFighter)
		{
			enemy1 = spriteA;
		}
		if (enemy1 != nullptr)	// 发生碰撞
		{
			this->handleFighterCollidingWithEnemy((Enemy*)enemy1);
			return false;
		}

		// 炮弹与敌人的接触
		Node * enemy2 = nullptr;

		if (spriteA->getTag() == GameSceneNodeBatchTagBullet && spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			if (!spriteA->isVisible())
			{
				return false;
			}
			spriteA->setVisible(false);
			enemy2 = spriteB;
		}
		if (spriteA->getTag() == GameSceneNodeBatchTagEnemy && spriteB->getTag() == GameSceneNodeBatchTagBullet)
		{
			if (!spriteB->isVisible())
			{
				return false;
			}
			spriteB->setVisible(false);
			enemy2 = spriteA;
		}
		if (enemy2 != nullptr)	// 发生碰撞
		{
			this->handleBulletCollidingWithEnemy((Enemy*)enemy2);
			return false;
		}

		return false;
	};

	// 添加触摸飞机事件监听器
	EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchFighterListener, this->fighter);
	// 添加接触事件监听器
	eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);

	// 每0.2秒发射一个炮弹
	this->schedule(schedule_selector(GamePlayLayer::shootBullet), 0.2f);

	// 设置初始分数
	this->score = 0;
	this->scorePlaceholder = 0;

	//在状态栏中设置玩家的生命值
	this->updateStatusBarFighter();
	//在状态栏中显示得分
	this->updateStatusBarScore();
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

	// 停止定时器
	this->unschedule(schedule_selector(GamePlayLayer::shootBullet));

	// 注销事件监听器
	Director::getInstance()->getEventDispatcher()->removeEventListener(touchFighterListener);
	Director::getInstance()->getEventDispatcher()->removeEventListener(contactListener);

	// 循环遍历Node元素，移除不是背景标签的Node元素
	auto nodes = this->getChildren();
	for (const auto& node : nodes)
	{
		if (node->getTag() != GameSceneNodeBatchTagBackground)
		{
			this->removeChild(node);
		}
	}
}

// 暂停游戏
void GamePlayLayer::menuPauseCallback(Ref* pSender)
{
	log("menuPauseCallback");

	Size visibleSize = Director::getInstance()->getVisibleSize();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	// 暂停当前层中的node
	this->pause();

	for (const auto& node : this->getChildren())
	{
		node->pause();
	}

	// 返回主菜单
	auto backNormal = Sprite::createWithSpriteFrameName("gameplay.button.back.png");
	auto backSelected = Sprite::createWithSpriteFrameName("gameplay.button.back-on.png");
	auto backMenuItem = MenuItemSprite::create(backNormal, backSelected,
		CC_CALLBACK_1(GamePlayLayer::menuBackCallback, this));

	// 继续游戏
	auto resumeNormal = Sprite::createWithSpriteFrameName("gameplay.button.resume.png");
	auto resumeSelected = Sprite::createWithSpriteFrameName("gameplay.button.resume-on.png");
	auto resumeMenuItem = MenuItemSprite::create(resumeNormal, resumeSelected,
		CC_CALLBACK_1(GamePlayLayer::menuResumeCallback, this));

	menu = Menu::create(backMenuItem, resumeMenuItem, NULL);
	menu->alignItemsVertically();
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	this->addChild(menu, 20, 1000);
}

// 返回主菜单
void GamePlayLayer::menuBackCallback(Ref* pSender)
{
	log("menuBackCallback");

	Director::getInstance()->popScene();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
}

// 返回游戏
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

// 发射炮弹
void GamePlayLayer::shootBullet(float dt)
{
	if (fighter && fighter->isVisible())
	{
		Bullet * bullet = Bullet::createWithSpriteFrameName("gameplay.bullet.png");
		bullet->setVelocity(Vec2(0, GameSceneBulletVelocity));
		this->addChild(bullet, 0, GameSceneNodeBatchTagBullet);
		bullet->shootBulletFromFighter(fighter);
	}
}

// 处理炮弹与敌人碰撞
void GamePlayLayer::handleBulletCollidingWithEnemy(Enemy* enemy)
{
	// 敌人生命值减1
	enemy->setHitPoints(enemy->getHitPoints() - 1);

	if (enemy->getHitPoints() <= 0)
	{
		// 敌人被击毁时爆炸音效和粒子效果
		Node * node = this->getChildByTag(GameSceneNodeTagExplosionParticleSystem);
		if (node)
		{
			this->removeChild(node);
		}
		ParticleSystem * explosion = ParticleSystemQuad::create("particle/explosion.plist");
		explosion->setPosition(enemy->getPosition());
		this->addChild(explosion, 2, GameSceneNodeTagExplosionParticleSystem);
		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
		{
			SimpleAudioEngine::getInstance()->playEffect(sound_2);
		}

		// 根据击毁敌人增加分数
		switch (enemy->getEnemyType())
		{
		case EnemyTypeStone:
			score += EnemyStone_Score;
			scorePlaceholder += EnemyStone_Score;
			break;
		case EnemyTypeEnemy1:
			score += Enemy1_Score;
			scorePlaceholder += Enemy1_Score;
			break;
		case EnemyTypeEnemy2:
			score += Enemy2_Score;
			scorePlaceholder += Enemy2_Score;
			break;
		case EnemyTypePlanet:
			score += EnemyPlanet_Score;
			scorePlaceholder += EnemyPlanet_Score;
			break;
		default:
			break;
		}

		// 获得1000分，生命值加1
		if (scorePlaceholder >= 1000)
		{
			fighter->setHitPoints(fighter->getHitPoints() + 1);
			this->updateStatusBarFighter();
			scorePlaceholder -= 1000;
		}

		this->updateStatusBarScore();
		// 敌人消失
		enemy->setVisible(false);
		enemy->spawn();
	}
}

// 处理飞机与敌人碰撞
void GamePlayLayer::handleFighterCollidingWithEnemy(Enemy* enemy)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 飞机碰撞时的音效和爆炸粒子效果
	Node * node = this->getChildByTag(GameSceneNodeTagExplosionParticleSystem);
	if (node)
	{
		this->removeChild(node);
	}
	ParticleSystem * explosion = ParticleSystemQuad::create("particle/explosion.plist");
	explosion->setPosition(fighter->getPosition());
	this->addChild(explosion, 2, GameSceneNodeTagExplosionParticleSystem);
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_2);
	}

	// 设置敌人消失,重新生成
	enemy->setVisible(false);
	enemy->spawn();

	// 玩家生命值减1
	fighter->setHitPoints(fighter->getHitPoints() - 1);
	this->updateStatusBarFighter();

	// 死亡，游戏结束
	if (fighter->getHitPoints() <= 0)
	{
		log("GameOver");





	}
	else{	// 碰撞未死，重设飞机位置和动作效果
		fighter->setPosition(Vec2(visibleSize.width / 2, 70));
		auto ac1 = Show::create();
		auto ac2 = FadeIn::create(1.0f);
		auto seq = Sequence::create(ac1, ac2, NULL);
		fighter->runAction(seq);
	}
}

//	玩家飞机生命值显示
void GamePlayLayer::updateStatusBarFighter()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 移除上次的精灵
	Node * node1 = this->getChildByTag(GameSceneNodeTagStatusBarFighterNode);
	if (node1)
	{
		this->removeChildByTag(GameSceneNodeTagStatusBarFighterNode);
	}

	Sprite * fg = Sprite::createWithSpriteFrameName("gameplay.life.png");
	fg->setPosition(Vec2(visibleSize.width - 60, visibleSize.height - 28));
	this->addChild(fg, 20, GameSceneNodeTagStatusBarFighterNode);

	// 添加生命值
	Node * node2 = this->getChildByTag(GameSceneNodeTagStatusBarLifeNode);
	if (node2)
	{
		this->removeChildByTag(GameSceneNodeTagStatusBarLifeNode);
	}

	if (this->fighter->getHitPoints() < 0)
	{
		this->fighter->setHitPoints(0);
	}

	__String * life = __String::createWithFormat("x%d", this->fighter->getHitPoints());
	auto lblLife = Label::createWithTTF(life->getCString(), "fonts/hanyi.ttf", 18);
	lblLife->setPosition(fg->getPosition() + Vec2(30, 0));
	this->addChild(lblLife, 20, GameSceneNodeTagStatusBarLifeNode);
}

// 玩家得分情况显示
void GamePlayLayer::updateStatusBarScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// 移除上次的精灵
	Node * node = this->getChildByTag(GameSceneNodeTagStatusBarScore);
	if (node)
	{
		this->removeChildByTag(GameSceneNodeTagStatusBarScore);
	}
	// 分数不为负
	if (this->score < 0)
	{
		this->score = 0;
	}

	__String * score = __String::createWithFormat("%d", this->score);
	auto lblScore = Label::createWithTTF(score->getCString(), "fonts/hanyi.ttf", 18);
	lblScore->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 28));
	this->addChild(lblScore, 20, GameSceneNodeTagStatusBarScore);
}