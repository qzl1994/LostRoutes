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

// �����ڵ����ٶ�
#define GameSceneBulletVelocity						300
// ��ʼ�������ֵ
#define GameSceneFighterLife                        5

#include "cocos2d.h"
#include "Enemy.h"
#include "Fighter.h"
#include "Bullet.h"
#include "GameOverScene.h"

#include "SystemHeader.h"

// ���˱�����ʱ��õķ���
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

	int score;				// ����
	int scorePlaceholder;	// ��¼0~999����

	cocos2d::EventListenerTouchOneByOne * touchFighterListener;
	cocos2d::EventListenerPhysicsContact * contactListener;

public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	virtual void onEnter();
	virtual void onExit();
	virtual void onEnterTransitionDidFinish();

	// ��ʼ������
	void initBG();
	// �����ڵ�
	void shootBullet(float dt);
	// ���·���
	void updateStatusBarScore();
	// ��������ֵ
	void updateStatusBarFighter();

	void menuPauseCallback(cocos2d::Ref* pSender);
	void menuBackCallback(cocos2d::Ref* pSender);
	void menuResumeCallback(cocos2d::Ref* pSender);

	// �ڵ�����˵���ײ���
	void handleBulletCollidingWithEnemy(Enemy* enemy);
	// �ɻ�����˵���ײ���
	void handleFighterCollidingWithEnemy(Enemy* enemy);

	CREATE_FUNC(GamePlayLayer);
};

#endif