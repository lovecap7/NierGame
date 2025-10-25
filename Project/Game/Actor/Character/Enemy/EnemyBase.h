#pragma once
#include "../CharacterBase.h"
#include "../../../../General/TargetInfo.h"
#include <memory>
class ActorData;
class CharaStatusData;
class ActorManager;
class Player;
class EnemyBase abstract:
    public CharacterBase
{
public:
	EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~EnemyBase();
	//攻撃クールタイム取得
	float GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃可能
	bool IsEnableAttack() const;
	//攻撃クールタイム設定
	void SetAttackCoolTime(float coolTime) { m_attackCoolTime = coolTime; }
	//ロックオンされたときに見られる座標取得
	Vector3 GetLockOnViewPos() const { return m_lockOnViewPos; }
	//活動取得
	bool IsActive() const { return m_isActive; }
	//活動設定
	void SetIsActive(bool isActive) { m_isActive = isActive; }
	//索敵
	void SearchTarget(std::shared_ptr<Player> player);
	//ターゲットへのベクトル
	Vector3 GetToTargetVec() const;
	//ターゲット情報取得
	TargetInfo GetTargetInfo() const { return m_targetInfo; }
	//近距離攻撃範囲
	bool IsInMeleeRange() const;
protected:
	//攻撃クールタイム
	float m_attackCoolTime;
	//ロックオンされたときに見られる座標
	Vector3 m_lockOnViewPos;
	//活動
	bool m_isActive;
	//ターゲット情報
	TargetInfo m_targetInfo;
	//警戒状態
	bool m_isAlerted;
protected:
	//カウント
	void CountAttackCoolTime();
	//ロックオンされたときに見られる座標設定
	void UpdateLockOnViewPos();
};

