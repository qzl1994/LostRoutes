#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "SystemHeader.h"
#include "HomeScene.h"

class Loading : public cocos2d::Layer
{
private:
	void loadingTextureCallback(cocos2d::Texture2D* texture);
	void delayCall(float dt);
	int m_nNumberOfLoaded; // 纹理图片的个数
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onExit();
    
    CREATE_FUNC(Loading);
};

#endif
