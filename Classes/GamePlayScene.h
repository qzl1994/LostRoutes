#ifndef __GAME_PLAY_SCENE_H__
#define __GAME_PLAY_SCENE_H__

#define GameSceneNodeTagStatusBarFighterNode		301
#define GameSceneNodeTagStatusBarLifeNode			302
#define GameSceneNodeTagStatusBarScore				303

#define GameSceneNodeBatchTagBackground				800
#define GameSceneNodeTagFighter						900
#define GameSceneNodeTagExplosionParticleSystem		901
#define GameSceneNodeBatchTagBullet					902
#define GameSceneNodeBatchTagEnemy					903

// 发射炮弹的速度
#define GameSceneBulletVelocity						300
// 初始玩家生命值
#define GameSceneFighterLife                        5

#include "cocos2d.h"
#include "Enemy.h"
#include "Fighter.h"
#include "Bullet.h"
#include "GameOverScene.h"

#include "SystemHeader.h"

// 敌人被击毁时获得的分数
typedef enum
{
	EnemyStone_Score = 5,
	Enemy1_Score = 10,
	Enemy2_Score = 15,
	EnemyPlanet_Score = 20
}EnemyScores;

class GamePlayLayer : public cocos2d::Layer
{
private:
	Fighter * fighter;
	cocos2d::Menu* menu;

	int score;				// 分数
	int scorePlaceholder;	// 记录0~999分数

	cocos2d::EventListenerTouchOneByOne * touchFighterListener;
	cocos2d::EventListenerPhysicsContact * contactListener;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();

	// 初始化背景
	void initBG();
	// 发射炮弹
	void shootBullet(float dt);
	// 更新分数
	void updateStatusBarScore();
	// 更新生命值
	void updateStatusBarFighter();

	void menuPauseCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);
	void menuResumeCallback(cocos2d::Ref* pSender);

	// 炮弹与敌人的碰撞检测
	void handleBulletCollidingWithEnemy(Enemy* enemy);
	// 飞机与敌人的碰撞检测
	void handleFighterCollidingWithEnemy(Enemy* enemy);

	CREATE_FUNC(GamePlayLayer);
};

#endif