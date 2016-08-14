#include "Bullet.h"

USING_NS_CC;

Bullet* Bullet::createWithSpriteFrameName(const char * spriteFrameName)
{
	Bullet * bullet = new Bullet();

	if (bullet && bullet->initWithSpriteFrameName(spriteFrameName))
	{
		bullet->autorelease();

		bullet->setVisible(false);

		auto body = PhysicsBody::createBox(bullet->getContentSize());

		body->setCategoryBitmask(0x01);
		body->setCollisionBitmask(0x02);
		body->setContactTestBitmask(0x01);

		bullet->setPhysicsBody(body);

		return bullet;
	}

	CC_SAFE_DELETE(bullet);
	
	return nullptr;
}

// 设置炮弹初始位置，并可见
void Bullet::shootBulletFromFighter(Fighter * fighter)
{
	this->setPosition(fighter->getPosition() + Vec2(0, fighter->getContentSize().height / 2));
	this->setVisible(true);
	this->unscheduleUpdate();
	this->scheduleUpdate();
}

// 根据速度修改炮弹位置，超出屏幕则移除
void Bullet::update(float dt)
{
	Size screenSize = Director::getInstance()->getVisibleSize();

	this->setPosition(Vec2(this->getPosition() + velocity * dt));

	if (this->getPosition().y > screenSize.height)
	{
		this->setVisible(false);
		this->unscheduleUpdate();
		this->removeFromParent();
	}
}