#ifndef __BULLET_SPRITE_H__
#define __BULLET_SPRITE_H__

#include "cocos2d.h"
#include "Fighter.h"

class Bullet : public cocos2d::Sprite
{
	CC_SYNTHESIZE(cocos2d::Vec2, velocity, Velocity);	// �ٶ�

public:
	static Bullet * createWithSpriteFrameName(const char * spriteFrameName); // ��̬��������

	void shootBulletFromFighter(Fighter * fighter);							 // �����ڵ�

	virtual void update(float dt);
};

#endif