#include "SettingScene.h"

USING_NS_CC;

Scene* SettingLayer::createScene()
{
	auto scene = Scene::create();

	auto layer = SettingLayer::create();

	scene->addChild(layer);

	return scene;
}

bool SettingLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto top = Sprite::createWithSpriteFrameName("setting-top.png");
	top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - top->getContentSize().height / 2));
	this->addChild(top);

	// ÒôÐ§
	auto soundOnSprite = Sprite::createWithSpriteFrameName("setting.check-on.png");
	auto soundOffSprite = Sprite::createWithSpriteFrameName("setting.check-off.png");
	auto soundOnMenuItem = MenuItemSprite::create(soundOnSprite, NULL);
	auto soundOffMenuItem = MenuItemSprite::create(soundOffSprite, NULL);
	auto soundToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingLayer::menuSoundToggleCallback, this),
		soundOnMenuItem, soundOffMenuItem, NULL);

	// ÒôÀÖ
	auto musicOnSprite = Sprite::createWithSpriteFrameName("setting.check-on.png");
	auto musicOffSprite = Sprite::createWithSpriteFrameName("setting.check-off.png");
	auto musicOnMenuItem = MenuItemSprite::create(musicOnSprite, NULL);
	auto musicOffMenuItem = MenuItemSprite::create(musicOffSprite, NULL);
	auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingLayer::menuSoundToggleCallback, this),
		musicOnMenuItem, musicOffMenuItem, NULL);

	auto menu = Menu::create(soundToggleMenuItem, musicToggleMenuItem, NULL);
	menu->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2) + Vec2(70, 50));
	menu->alignItemsVerticallyWithPadding(25.0f);
	this->addChild(menu, 1);

	

}

void SettingLayer::menuSoundToggleCallback(Ref* pSender)
{

}

void SettingLayer::menuMusicToggleCallback(Ref* pSender)
{

}