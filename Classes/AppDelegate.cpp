#include "AppDelegate.h"
#include "LoadingScene.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(320, 568);
static cocos2d::Size smallResolutionSize = cocos2d::Size(640, 1136);
static cocos2d::Size largeResolutionSize = cocos2d::Size(750, 1334);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// ��ʼ������
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("LostRoutes", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("LostRoutes");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);

    director->setAnimationInterval(1.0f / 60);

    // ��Ʒֱ��ʲ���
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

	// ��Ļ�ֱ���
    auto frameSize = glview->getFrameSize();

	std::vector<std::string> searchPaths;

	// �����Ļ�ֱ��ʸ߶ȴ���small�ߴ����Դ�ֱ��ʸ߶ȣ�ѡ��large��Դ
	if (frameSize.height > smallResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
		searchPaths.push_back("large");
	}
	// �����Ļ�ֱ��ʸ߶�С�ڻ����small�ߴ����Դ�ֱ��ʸ߶ȣ�ѡ��small��Դ
	else
	{
		director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
		searchPaths.push_back("small");
	}

	// ������Դ����·��
	FileUtils::getInstance()->setSearchPaths(searchPaths);

    register_all_packages();

    auto scene = Loading::createScene();

    director->runWithScene(scene);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_1);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(bg_music_2);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
