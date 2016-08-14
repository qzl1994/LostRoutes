#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(EnemyTypes enemyType)
{
	this->enemyType = enemyType;
	this->initialHitPoints = 1;
	this->velocity = Vec2::ZERO;
}

Enemy* Enemy::createWithEnemyTypes(EnemyTypes enemyType)
{
	Enemy * enemy = new Enemy(enemyType);

	const char * enemyFrameName = Enemy_Stone;

	switch (enemyType)
	{
	case EnemyTypeStone:
		enemyFrameName = Enemy_Stone;
		enemy->initialHitPoints = 3;
		break;
	case EnemyTypeEnemy1:
		enemyFrameName = Enemy_1;
		enemy->initialHitPoints = 5;
		break;
	case EnemyTypeEnemy2:
		enemyFrameName = Enemy_2;
		enemy->initialHitPoints = 10;
		break;
	case EnemyTypePlanet:
		enemyFrameName = Enemy_Planet;
		enemy->initialHitPoints = 15;
		break;
	default:
		break;
	}

	if (enemy && enemy->initWithSpriteFrameName(enemyFrameName))
	{
		// �����˶�������ڴ��ͷų���
		enemy->autorelease();

		// Ϊ���˶��������������֧��
		auto body = PhysicsBody::create();

		if (enemyType == EnemyTypeStone || enemyType == EnemyTypePlanet)
		{
			body->addShape(PhysicsShapeCircle::create(enemy->getContentSize().width / 2 - 5));
		}
		else if (enemyType == EnemyTypeEnemy1)
		{
			Vec2 verts[] = {
				Vec2(-2.5, -45.75),
				Vec2(-29.5, -27.25),
				Vec2(-53, -0.25),
				Vec2(-34, 43.25),
				Vec2(28, 44.25),
				Vec2(55, -2.25)};
			// �����������״
			body->addShape(PhysicsShapePolygon::create(verts, 6));
		}
		else if (enemyType == EnemyTypeEnemy2)
		{
			Vec2 verts[] = {
				Vec2(1.25, 32.25),
				Vec2(36.75, -4.75),
				Vec2(2.75, -31.75),
				Vec2(-35.75, -3.25)
			};
			body->addShape(PhysicsShapePolygon::create(verts, 4));
		}

		body->setCategoryBitmask(0x01);		// ���������������
		body->setCollisionBitmask(0x02);	// ����������ײ����
		body->setContactTestBitmask(0x01);	// ���ýӴ�����

		enemy->setPhysicsBody(body);

		enemy->setVisible(false);
		enemy->spawn();
		enemy->unscheduleUpdate();
		enemy->scheduleUpdate();

		return enemy;
	}

	CC_SAFE_DELETE(enemy);

	return nullptr;
}

void Enemy::update(float dt)
{
	// ������ʯ��������ת
	switch (enemyType)
	{
	case EnemyTypeStone:
		this->setRotation(this->getRotation() - 0.5);
		break;
	case EnemyTypePlanet:
		this->setRotation(this->getRotation() + 1);
		break;
	default:
		break;
	}

	// ���㱾�ε����ƶ�����
	Vec2 moveLen = velocity * dt;

	this->runAction(Place::create(this->getPosition() + moveLen));

	// �жϵ����˶�����Ļ֮��
	if (this->getPosition().y + this->getContentSize().height / 2 < 0)
	{
		this->spawn();
	}
}

// �������ɵ���
void Enemy::spawn()
{
	Size screenSize = Director::getInstance()->getVisibleSize();

	float yPos = screenSize.height + this->getContentSize().height / 2;
	float xPos = CCRANDOM_0_1() * (screenSize.width - this->getContentSize().width) + this->getContentSize().width / 2;

	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->runAction(Place::create(Vec2(xPos, yPos)));
	
	hitPoints = initialHitPoints;
	this->setVisible(true);
}