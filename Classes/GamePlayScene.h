#ifndef __GAME_PLAY_SCENE_H__
#define __GAME_PLAY_SCENE_H__

#include "cocos2d.h"

class GamePlayLayer : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(GamePlayLayer);
};

#endif