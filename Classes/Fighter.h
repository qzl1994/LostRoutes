#ifndef __FIGHTER_SPRITE_H__
#define __FIGHTER_SPRITE_H__

#include "cocos2d.h"

class Fighter : public cocos2d::Sprite
{
	CC_SYNTHESIZE(int, hitPoints, HitPoints);		// 生命值

public:
	static Fighter* createWithSpriteFrameName(const char * spriteFrameName);	// 静态创建函数
	void setPosition(const cocos2d::Vec2& newPosition);							// 设置位置
};

#endif