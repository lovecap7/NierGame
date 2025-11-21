#pragma once
#include "../EnemyStateAttack.h"
class Actor;
class AttackData;
class AttackBase;
class EnemyBase;
class Boss4StateAttack :
	public EnemyStateAttack
{
public:
	Boss4StateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData);
	virtual ~Boss4StateAttack();
	void Init() override;
	void Update() override;
	//UŒ‚ì¬
	void CreateAttack(std::shared_ptr<EnemyBase> owner)override;
};

