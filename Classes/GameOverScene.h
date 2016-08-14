#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"
#include "SystemHeader.h"
#include "MyUtility.h"

class GameOverLayer : public cocos2d::Layer
{
	// 当前得分
	int score;
public:
	GameOverLayer(int score);

	virtual bool init();
	virtual void onExit();

	static GameOverLayer* createWithScore(int score);
};

#endif