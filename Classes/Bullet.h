#ifndef __BULLET_SPRITE_H__
#define __BULLET_SPRITE_H__

#include "cocos2d.h"
#include "Fighter.h"

class Bullet : public cocos2d::Sprite
{
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);	// 速度

public:
	static Bullet * createWithSpriteFrameName(const char * spriteFrameName); // 静态创建函数

	void shootBulletFromFighter(Fighter * fighter);							 // 发射炮弹

	virtual void update(float dt);
};

#endif