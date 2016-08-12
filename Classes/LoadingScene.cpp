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

	auto logo = Sprite::createWithSpriteFrameName("logo.png");
	logo->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(logo);

	auto sprite = Sprite::createWithSpriteFrameName("loding4.png");
	sprite->setPosition(logo->getPosition() - Vec2(0, logo->getContentSize().height / 2 + 30));
	this->addChild(sprite);

	// Loading动画开始
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
	//////////////////动画结束///////////////////

	// 初始化加载纹理图片的个数
	m_nNumberOfLoaded = 0;

	// 加载纹理图片
	Director::getInstance()->getTextureCache()->addImageAsync("texture/home_texture.png", 
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/setting_texture.png",
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));
	Director::getInstance()->getTextureCache()->addImageAsync("texture/gameplay_texture.png",
		CC_CALLBACK_1(Loading::loadingTextureCallback, this));
	
	// 创建一个线程
	_loadingAudioThread = new std::thread(&Loading::loadingAudio, this);

    return true;
}

// 加载纹理对应的帧缓存
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

// 延时调用函数
void Loading::delayCall(float dt)
{
	
}

// 预处理声音
void Loading::loadingAudio()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_1);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_2);
}

void Loading::onExit()
{
	Layer::onExit();
	
	// 合并子线程到主线程
	_loadingAudioThread->join();

	// 释放变量内存
	CC_SAFE_DELETE(_loadingAudioThread);

	// 清除Loading场景精灵帧缓存和纹理缓存
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("texture/loading_texture.plist");
	Director::getInstance()->getTextureCache()->removeTextureForKey("texture/loading_texture.png");

	this->unschedule(schedule_selector(Loading::delayCall));
}
