#ifndef __GAME_PLAY_SCENE_H__
#define __GAME_PLAY_SCENE_H__


#define GameSceneNodeBatchTagBackground				800
#define GameSceneNodeTagFighter						900

#define GameSceneNodeBatchTagEnemy					903




#include "cocos2d.h"
#include "Enemy.h"
#include "Fighter.h"
#include "Bullet.h"

#include "SystemHeader.h"

class GamePlayLayer : public cocos2d::Layer
{
private:

	Fighter * fighter;

	cocos2d::EventListenerTouchOneByOne * touchFighterListener;


public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	//virtual void onEnterTransitionDidFinish();

	// ≥ı ºªØ±≥æ∞
	void initBG();

	CREATE_FUNC(GamePlayLayer);
};

#endif