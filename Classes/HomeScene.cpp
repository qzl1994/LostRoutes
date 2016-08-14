#include "HomeScene.h"

USING_NS_CC;

Scene* HomeMenuLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = HomeMenuLayer::create();

	scene->addChild(layer);

	return scene;
}

bool HomeMenuLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto bg = TMXTiledMap::create("map/red_bg.tmx");
	this->addChild(bg);

	auto top = Sprite::createWithSpriteFrameName("home-top.png");
	top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - top->getContentSize().height / 2));
	this->addChild(top);

	auto end = Sprite::createWithSpriteFrameName("home-end.png");
	end->setPosition(Vec2(visibleSize.width / 2, end->getContentSize().height / 2));
	this->addChild(end);

	// 开始菜单
	auto startSpriteNormal = Sprite::createWithSpriteFrameName("home.button.start.png");
	auto startSpriteSelected = Sprite::createWithSpriteFrameName("home.button.start-on.png");
	auto startMenuItem = MenuItemSprite::create(startSpriteNormal, startSpriteSelected,
		CC_CALLBACK_1(HomeMenuLayer::menuItemCallback, this));
	startMenuItem->setTag(ActionType::MenuItemStart);

	// 设置菜单
	auto settingSpriteNormal = Sprite::createWithSpriteFrameName("home.button.setting.png");
	auto settingSpriteSelected = Sprite::createWithSpriteFrameName("home.button.setting-on.png");
	auto settingMenuItem = MenuItemSprite::create(settingSpriteNormal, settingSpriteSelected,
		CC_CALLBACK_1(HomeMenuLayer::menuItemCallback, this));
	settingMenuItem->setTag(ActionType::MenuItemSetting);

	// 帮助菜单
	auto helpSpriteNormal = Sprite::createWithSpriteFrameName("home.button.help.png");
	auto helpSpriteSelected = Sprite::createWithSpriteFrameName("home.button.help-on.png");
	auto helpMenuItem = MenuItemSprite::create(helpSpriteNormal, helpSpriteSelected,
		CC_CALLBACK_1(HomeMenuLayer::menuItemCallback, this));
	helpMenuItem->setTag(ActionType::MenuItemHelp);

	auto menu = Menu::create(startMenuItem, settingMenuItem, helpMenuItem, NULL);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	menu->alignItemsVerticallyWithPadding(12);
	this->addChild(menu);

	return true;
}

void HomeMenuLayer::menuItemCallback(Ref* pSender)
{
	if (UserDefault::getInstance()->getBoolForKey(SOUND_KEY))
	{
		SimpleAudioEngine::getInstance()->playEffect(sound_1);
	}

	Scene * scene = nullptr;

	MenuItem * menuItem = (MenuItem*)pSender;

	switch (menuItem->getTag())
	{
	case ActionType::MenuItemStart:
		log("StartCallback");
		scene = TransitionFade::create(1.0f, GamePlayLayer::createScene());
		break;
	case ActionType::MenuItemSetting:
		log("SettingCallback");
		scene = TransitionFade::create(1.0f, SettingLayer::createScene());
		break;
	case ActionType::MenuItemHelp:
		log("HelpCallback");
		scene = TransitionFade::create(1.0f, HelpLayer::createScene());
		break;
	default:
		break;
	}

	if (scene)
	{
		Director::getInstance()->pushScene(scene);
	}
}

void HomeMenuLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	log("HomeMenuLayer onEnterTransitionDidFinish");

	//SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);

	UserDefault * defaults = UserDefault::getInstance();
	if (defaults->getBoolForKey(MUSIC_KEY))
	{
		log("HomeMenuLayer onEnterTransitionDidFinish");
		SimpleAudioEngine::getInstance()->playBackgroundMusic(bg_music_1, true);
	}
}