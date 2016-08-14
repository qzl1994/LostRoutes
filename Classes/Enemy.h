#ifndef __ENEMY_SPRITE_H__
#define __ENEMY_SPRITE_H__

#include "cocos2d.h"

// 定义敌人名称，也就是精灵帧的名字
#define Enemy_Stone "gameplay.stone1.png"
#define Enemy_1 "gameplay.enemy-1.png"
#define Enemy_2 "gameplay.enemy-2.png"
#define Enemy_Planet "gameplay.enemy.planet.png"

// 定义敌人类型
typedef enum
{
	EnemyTypeStone = 0,		// 陨石
	EnemyTypeEnemy1,		// 敌机1
	EnemyTypeEnemy2,		// 敌机2
	EnemyTypePlanet			// 行星
}EnemyTypes;

class Enemy : public cocos2d::Sprite
{
	CC_SYNTHESIZE(EnemyTypes, enemyType, EnemyType);			// 敌人类型
	CC_SYNTHESIZE(int, initialHitPoints, InitialHitPoints);		// 初始生命值
	CC_SYNTHESIZE(int, hitPoints, HitPoints);					// 当前生命值
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);			// 速度

public:
	Enemy(EnemyTypes enemyType);								// 构造函数
	void spawn();												// 产生敌人函数
	virtual void update(float dt);								
	static Enemy* createWithEnemyTypes(EnemyTypes enemyType);	// 静态创建敌人函数
};

#endif