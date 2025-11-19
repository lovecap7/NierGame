#pragma once
#include "../EnemyStateAttack.h"
class Actor;
class AttackData;
class AttackBase;
class EnemyBase;
class Boss3StateAttack :
	public EnemyStateAttack
{
public:
	Boss3StateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData);
	~Boss3StateAttack() {};
	void Update() override;
private:
	//ˆÚ“®
	void UpdateMove(std::shared_ptr<EnemyBase> owner, std::shared_ptr<Model> model) override;
};

