#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "SystemHeader.h"

class Loading : public cocos2d::Layer
{
private:
	void loadingTextureCallback(cocos2d::Texture2D* texture);
	void delayCall(float dt);
	void loadingAudio();
	int m_nNumberOfLoaded; // ����ͼƬ�ĸ���
	std::thread * _loadingAudioThread; // �첽��������������߳�
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onExit();
    
    CREATE_FUNC(Loading);
};

#endif
