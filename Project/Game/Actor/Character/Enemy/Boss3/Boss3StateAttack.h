#pragma once
#include "../EnemyStateAttack.h"
#include <memory>
#include <vector>
class Actor;
class AttackData;
class AttackBase;
class EnemyBase;
class BeamAttack;
class Boss3StateAttack :
	public EnemyStateAttack
{
public:
	Boss3StateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData);
	~Boss3StateAttack();
	void Update() override;
private:
	//フレームによる攻撃の更新
	void UpdateAttackFrame(std::shared_ptr<EnemyBase> owner)override;
	//攻撃作成
	void CreateAttack(std::shared_ptr<EnemyBase> owner)override;
	//移動
	void UpdateMove(std::shared_ptr<EnemyBase> owner, std::shared_ptr<Model> model) override;
	//攻撃位置の更新
	void UpdateAttackPos(std::shared_ptr<EnemyBase> owner)override;
private:
	//ビーム
	std::vector<std::weak_ptr<BeamAttack>> m_beams;
};

