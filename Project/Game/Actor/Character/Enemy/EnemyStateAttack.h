#pragma once
#include "EnemyStateBase.h"
#include <memory>
class Actor;
class AttackData;
class AttackBase;
class EnemyBase;
class EnemyStateAttack :
	public EnemyStateBase, public std::enable_shared_from_this<EnemyStateAttack>
{
public:
	EnemyStateAttack(std::weak_ptr<Actor> enemy);
	virtual ~EnemyStateAttack();
	virtual void Init() override;
	virtual void Update() override;
private:
	//UŒ‚ŠJnXV
	void UpdateStartAttack(std::shared_ptr<EnemyBase>& owner);
	//UŒ‚íœ
	void DeleteAttack();
	//UŒ‚ì¬
	void CreateAttack(std::shared_ptr<EnemyBase> owner);
	//‘½’iƒqƒbƒgUŒ‚“Ç‚İ‚İ
	void LoadNextMultipleHitAttack(std::shared_ptr<EnemyBase> owner);
	//UŒ‚ˆÚ“®XV
	void UpdateMove(std::shared_ptr<EnemyBase> owner, std::shared_ptr<Model> model);
	//UŒ‚ˆÊ’u‚ÌXV
	void UpdateAttackPos(std::shared_ptr<EnemyBase> owner);
private:
	//UŒ‚ƒf[ƒ^
	std::shared_ptr<AttackData> m_attackData;
	//UŒ‚”­¶
	bool m_isAppearedAttack;
	//UŒ‚‚ÌQÆ
	std::weak_ptr<AttackBase> m_pAttack;
};

