#include "LoadingScene.h"

USING_NS_CC;

Scene* Loading::createScene()
{
    auto scene = Scene::create();
    
    auto layer = Loading::create();

    scene->addChild(layer);

    return scene;
}

bool Loading::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/loading_texture.plist");

	auto bg = TMXTiledMap::create("map/red_bg.tmx");
	this->addChild(bg);

	auto sprite = Sprite::createWithSpriteFrameName("loding4.png");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(sprite);

	// Loading������ʼ
	Animation * animation = Animation::create();
	for (int i = 1; i <= 4; i++)
	{
		__String * frameName = __String::createWithFormat("loding%d.png", i);
		SpriteFrame * spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName->getCString());
		animation->addSpriteFrame(spriteFrame);
	}

	animation->setDelayPerUnit(0.5f);
	animation->setRestoreOriginalFrame(true);

	Animate * action = Animate::create(animation);
	sprite->runAction(RepeatForever::create(action));
	//////////////////��������///////////////////

	// ��ʼ����������ͼƬ�ĸ���
	m_nNumberOfLoaded = 0;

	// ��������ͼƬ
	Director::getInstance()->getTextureCache()->addImageAsync("texture/home_texture.png", 
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/setting_texture.png",
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/gameplay_texture.png",
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));

    return true;
}

// ���������Ӧ��֡����
void Loading::loadingTextureCallback(Texture2D * texture)
{
	switch (m_nNumberOfLoaded++)
	{
	case 0:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/home_texture.plist", texture); break;
	case 1:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/setting_texture.plist", texture); break;
	case 2:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("texture/gameplay_texture.plist", texture); 
		this->schedule(schedule_selector(Loading::delayCall), 1, 1, 3);
		break;
	default:
		break;
	}
}

// ��ʱ���ú���,�������˵�
void Loading::delayCall(float dt)
{
	auto scene = HomeMenuLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

void Loading::onExit()
{
	Layer::onExit();

	// ���Loading��������֡�����������
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("texture/loading_texture.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("texture/loading_texture.png");

	this->unschedule(schedule_selector(Loading::delayCall));
}
