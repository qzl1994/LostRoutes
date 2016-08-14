#ifndef __FIGHTER_SPRITE_H__
#define __FIGHTER_SPRITE_H__

#include "cocos2d.h"

class Fighter : public cocos2d::Sprite
{
	CC_SYNTHESIZE(int, hitPoints, HitPoints);		// ����ֵ

public:
	static Fighter* createWithSpriteFrameName(const char * spriteFrameName);	// ��̬��������
	void setPosition(const cocos2d::Vec2& newPosition);							// ����λ��
};

#endif