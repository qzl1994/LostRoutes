#include "HelpScene.h"

USING_NS_CC;

Scene* HelpLayer::createScene()
{
	auto scene = Scene::create();
	
	auto layer = HelpLayer::create();

	scene->addChild(layer);

	return scene;
}

bool HelpLayer::init()
{
	if (!BaseLayer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto top = Sprite::createWithSpriteFrameName("help-top.png");
	top->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - top->getContentSize().height / 2));
	this->addChild(top);

	// 设置"测试"文字
	auto txtTest = Label::createWithTTF(MyUtility::getUTF8Char("test"), "fonts/hanyi.ttf", 18);
	txtTest->setColor(Color3B(14, 83, 204));
	txtTest->setPosition(95, top->getPosition().y - 70);
	txtTest->setAnchorPoint(Vec2(0, 0));
	this->addChild(txtTest, 1);

	// "测试"下文字
	auto test = Label::createWithTTF("Mr.quan", "fonts/hanyi.ttf", 18);
	test->setAnchorPoint(Vec2(0, 0));
	test->setColor(Color3B(198, 12, 0));
	test->setPosition(txtTest->convertToWorldSpace(Vec2(0, -20)));
	this->addChild(test, 1);

	// 设置"音乐"文字
	auto txtMusic = Label::createWithTTF(MyUtility::getUTF8Char("music"), "fonts/hanyi.ttf", 18);
	txtMusic->setColor(Color3B(14, 83, 204));
	txtMusic->setAnchorPoint(Vec2(0, 0));
	txtMusic->setPosition(test->convertToWorldSpace(Vec2(0, -40)));
	this->addChild(txtMusic, 1);

	// "音乐"下文字
	auto music = Label::createWithTTF("Mr.quan", "fonts/hanyi.ttf", 18);
	music->setAnchorPoint(Vec2(0, 0));
	music->setColor(Color3B(198, 12, 0));
	music->setPosition(txtMusic->convertToWorldSpace(Vec2(0, -20)));
	this->addChild(music);

	// 设置"音效"文字
	auto txtSound = Label::createWithTTF(MyUtility::getUTF8Char("sound"), "fonts/hanyi.ttf", 18);
	txtSound->setColor(Color3B(14, 83, 204));
	txtSound->setAnchorPoint(Vec2(0, 0));
	txtSound->setPosition(music->convertToWorldSpace(Vec2(0, -40)));
	this->addChild(txtSound, 1);

	// "音效"下文字
	auto sound = Label::createWithTTF("Mr.quan", "fonts/hanyi.ttf", 18);
	sound->setColor(Color3B(198, 12, 0));
	sound->setAnchorPoint(Vec2(0, 0));
	sound->setPosition(txtSound->convertToWorldSpace(Vec2(0, -20)));
	this->addChild(sound, 1);

	// 设置"信息与服务"文字
	auto txtInfoService = Label::createWithTTF(MyUtility::getUTF8Char("info_service"), "fonts/hanyi.ttf", 18);
	txtInfoService->setColor(Color3B(14, 83, 204));
	txtInfoService->setAnchorPoint(Vec2(0, 0));
	txtInfoService->setPosition(sound->convertToWorldSpace(Vec2(0, -40)));
	this->addChild(txtInfoService, 1);

	// "信息与服务"下文字
	auto infoService = Label::createWithTTF("cocos2d-x-3.12", "fonts/hanyi.ttf", 18);
	infoService->setColor(Color3B(198, 12, 0));
	infoService->setAnchorPoint(Vec2(0, 0));
	infoService->setPosition(txtInfoService->convertToWorldSpace(Vec2(0, -20)));
	this->addChild(infoService, 1);

	return true;
}