#ifndef __HELPLAYER_SCENE_H__
#define __HELPLAYER_SCENE_H__

#include "cocos2d.h"
#include "BaseLayer.h"
#include "MyUtility.h"
#include "SystemHeader.h"

class HelpLayer : public BaseLayer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(HelpLayer);
};

#endif