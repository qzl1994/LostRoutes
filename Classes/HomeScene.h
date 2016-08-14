#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "cocos2d.h"
#include "SystemHeader.h"
#include "SettingScene.h"
#include "HelpScene.h"
#include "GamePlayScene.h"

typedef enum
{
	MenuItemStart,
	MenuItemSetting,
	MenuItemHelp
}ActionType;

class HomeMenuLayer :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void onEnterTransitionDidFinish();


	void menuItemCallback(cocos2d::Ref* pSender);
	CREATE_FUNC(HomeMenuLayer);
};

#endif