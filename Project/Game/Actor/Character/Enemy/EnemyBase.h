#pragma once
#include "../CharacterBase.h"
class ActorData;
class CharaStatusData;
class ActorManager;
class EnemyBase abstract:
    public CharacterBase
{
public:
	EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~EnemyBase();
	//攻撃クールタイム取得
	float GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃可能
	bool IsEnableAttack() const { return m_attackCoolTime <= 0.0f; }
	//攻撃クールタイム設定
	void SetAttackCoolTime(float coolTime) { m_attackCoolTime = coolTime; }
	//ロックオンされたときに見られる座標取得
	Vector3 GetLockOnViewPos() const { return m_lockOnViewPos; }
protected:
	//攻撃クールタイム
	float m_attackCoolTime;
	//ロックオンされたときに見られる座標
	Vector3 m_lockOnViewPos;
protected:
	//カウント
	void CountAttackCoolTime();
	//ロックオンされたときに見られる座標設定
	void UpdateLockOnViewPos();
};

