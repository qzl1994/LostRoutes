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

// ��ʼ������
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
	this->fighter->setHitPoints(GameSceneFighterLife);
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

	// ע��Ӵ��¼�������
	contactListener = EventListenerPhysicsContact::create();

	// �Ӵ��¼�
	contactListener->onContactBegin = [this](PhysicsContact& contact)
	{
		auto spriteA = contact.getShapeA()->getBody()->getNode();
		auto spriteB = contact.getShapeB()->getBody()->getNode();

		// �ɻ�����˵ĽӴ�
		Node * enemy1 = nullptr;
		
		if (spriteA->getTag() == GameSceneNodeTagFighter && spriteB->getTag() == GameSceneNodeBatchTagEnemy)
		{
			enemy1 = spriteB;
		}
		if (spriteA->getTag() == GameSceneNodeBatchTagEnemy && spriteB->getTag() == GameSceneNodeTagFighter)
		{
			enemy1 = spriteA;
		}
		if (enemy1 != nullptr)	// ������ײ
		{
			this->handleFighterCollidingWithEnemy((Enemy*)enemy1);
			return false;
		}

		// �ڵ�����˵ĽӴ�
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
		if (enemy2 != nullptr)	// ������ײ
		{
			this->handleBulletCollidingWithEnemy((Enemy*)enemy2);
			return false;
		}

		return false;
	};

	// ��Ӵ����ɻ��¼�������
	EventDispatcher * eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(touchFighterListener, this->fighter);
	// ��ӽӴ��¼�������
	eventDispatcher->addEventListenerWithFixedPriority(contactListener, 1);

	// ÿ0.2�뷢��һ���ڵ�
	this->schedule(schedule_selector(GamePlayLayer::shootBullet), 0.2f);

	// ���ó�ʼ����
	this->score = 0;
	this->scorePlaceholder = 0;

	//��״̬����������ҵ�����ֵ
	this->updateStatusBarFighter();
	//��״̬������ʾ�÷�
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

	// ֹͣ��ʱ��
	this->unschedule(schedule_selector(GamePlayLayer::shootBullet));

	// ע���¼�������
	Director::getInstance()->getEventDispatcher()->removeEventListener(touchFighterListener);
	Director::getInstance()->getEventDispatcher()->removeEventListener(contactListener);

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

// ��ͣ��Ϸ
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

	this->addChild(menu, 20, 1000);
}

// �������˵�
void GamePlayLayer::menuBackCallback(Ref* pSender)
{
	log("menuBackCallback");

	Director::getInstance()->popScene();

	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}
}

// ������Ϸ
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

// �����ڵ�
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

// �����ڵ��������ײ
void GamePlayLayer::handleBulletCollidingWithEnemy(Enemy* enemy)
{
	// ��������ֵ��1
	enemy->setHitPoints(enemy->getHitPoints() - 1);

	if (enemy->getHitPoints() <= 0)
	{
		// ���˱�����ʱ��ը��Ч������Ч��
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

		// ���ݻ��ٵ������ӷ���
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

		// ���1000�֣�����ֵ��1
		if (scorePlaceholder >= 1000)
		{
			fighter->setHitPoints(fighter->getHitPoints() + 1);
			this->updateStatusBarFighter();
			scorePlaceholder -= 1000;
		}

		this->updateStatusBarScore();
		// ������ʧ
		enemy->setVisible(false);
		enemy->spawn();
	}
}

// ����ɻ��������ײ
void GamePlayLayer::handleFighterCollidingWithEnemy(Enemy* enemy)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// �ɻ���ײʱ����Ч�ͱ�ը����Ч��
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

	// ���õ�����ʧ,��������
	enemy->setVisible(false);
	enemy->spawn();

	// �������ֵ��1
	fighter->setHitPoints(fighter->getHitPoints() - 1);
	this->updateStatusBarFighter();

	// ��������Ϸ����
	if (fighter->getHitPoints() <= 0)
	{
		log("GameOver");





	}
	else{	// ��ײδ��������ɻ�λ�úͶ���Ч��
		fighter->setPosition(Vec2(visibleSize.width / 2, 70));
		auto ac1 = Show::create();
		auto ac2 = FadeIn::create(1.0f);
		auto seq = Sequence::create(ac1, ac2, NULL);
		fighter->runAction(seq);
	}
}

//	��ҷɻ�����ֵ��ʾ
void GamePlayLayer::updateStatusBarFighter()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// �Ƴ��ϴεľ���
	Node * node1 = this->getChildByTag(GameSceneNodeTagStatusBarFighterNode);
	if (node1)
	{
		this->removeChildByTag(GameSceneNodeTagStatusBarFighterNode);
	}

	Sprite * fg = Sprite::createWithSpriteFrameName("gameplay.life.png");
	fg->setPosition(Vec2(visibleSize.width - 60, visibleSize.height - 28));
	this->addChild(fg, 20, GameSceneNodeTagStatusBarFighterNode);

	// �������ֵ
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

// ��ҵ÷������ʾ
void GamePlayLayer::updateStatusBarScore()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// �Ƴ��ϴεľ���
	Node * node = this->getChildByTag(GameSceneNodeTagStatusBarScore);
	if (node)
	{
		this->removeChildByTag(GameSceneNodeTagStatusBarScore);
	}
	// ������Ϊ��
	if (this->score < 0)
	{
		this->score = 0;
	}

	__String * score = __String::createWithFormat("%d", this->score);
	auto lblScore = Label::createWithTTF(score->getCString(), "fonts/hanyi.ttf", 18);
	lblScore->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 28));
	this->addChild(lblScore, 20, GameSceneNodeTagStatusBarScore);
}