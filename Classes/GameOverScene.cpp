#include "GameOverScene.h"

USING_NS_CC;

GameOverLayer::GameOverLayer(int score)
{
	this->score = score;
}

GameOverLayer* GameOverLayer::createWithScore(int score)
{
	GameOverLayer * over = new GameOverLayer(score);

	if (over && over->init())
	{
		over->autorelease();

		return over;
	}

	CC_SAFE_DELETE(over);

	return nullptr;
}

bool GameOverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Ìí¼Ó±³¾°µØÍ¼.
	auto bg = TMXTiledMap::create("map/blue_bg.tmx");
	this->addChild(bg);

	// ·¢¹âÁ£×Ó±³¾°
	ParticleSystem *ps = ParticleSystemQuad::create("particle/light.plist");
	ps->setPosition(Vec2(visibleSize.width, visibleSize.height - 200) / 2);
	this->addChild(ps);

	// ¾«Áé
	auto top = Sprite::createWithSpriteFrameName("gameover.top.png");
	top->setAnchorPoint(Vec2(0, 0));
	top->setPosition(Vec2(0, visibleSize.height - top->getContentSize().height));
	this->addChild(top);

	// ´æ´¢×î¸ß·Ö
	UserDefault *defaults = UserDefault::getInstance();
	int highScore = defaults->getIntegerForKey(HIGHSCORE_KEY);
	if (highScore < score) {
		highScore = score;
		defaults->setIntegerForKey(HIGHSCORE_KEY, highScore);
	}

	__String *text = __String::createWithFormat("%i points", highScore);

	auto lblHighScore = Label::createWithTTF(MyUtility::getUTF8Char("lblHighScore"), "fonts/hanyi.ttf", 25);
	lblHighScore->setAnchorPoint(Vec2(0, 0));
	lblHighScore->setPosition(Vec2(60, top->getPosition().y - 30));
	addChild(lblHighScore);

	auto lblScore = Label::createWithTTF(text->getCString(), "fonts/hanyi.ttf", 24);
	lblScore->setColor(Color3B(75, 255, 255));
	lblScore->setAnchorPoint(Vec2(0, 0));
	lblScore->setPosition(lblHighScore->getPosition() - Vec2(0, 40));
	addChild(lblScore);

	auto text2 = Label::createWithTTF("Tap the Screen to Play", "fonts/hanyi.ttf", 24);
	text2->setAnchorPoint(Vec2(0, 0));
	text2->setPosition(lblScore->getPosition() - Vec2(10, 45));
	addChild(text2);

	// ×¢²á ´¥ÃþÊÂ¼þ¼àÌýÆ÷
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = [](Touch* touch, Event* event){
		Director::getInstance()->popScene();

		if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY)) {
			SimpleAudioEngine::getInstance()->playEffect(sound_1);
		}
		return false;
	};

	// Ìí¼Ó ´¥ÃþÊÂ¼þ¼àÌýÆ÷
	EventDispatcher* eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameOverLayer::onExit()
{
	Layer::onExit();
	Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
}