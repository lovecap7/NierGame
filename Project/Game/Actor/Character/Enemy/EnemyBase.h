#pragma once
#include "../CharacterBase.h"
#include "../../../../General/TargetInfo.h"
#include <memory>
#include <vector>
#include <string>
class ActorData;
class CharaStatusData;
class ActorManager;
class NormalEffect;
class Player;
class EnemyBase abstract:
    public CharacterBase
{
public:
	EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~EnemyBase();

	//更新処理
	virtual void Update() override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override {};
	//描画
	void Draw()const override;

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
	//エリア内か
	bool IsInArea() const { return m_isInArea; }
	void SetIsInArea(bool isInArea) { m_isInArea = isInArea; };
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
	//遠距離攻撃可能か
	bool IsEnableLongRangeAttack() const;
	//近接攻撃可能か
	bool IsEnableMeleeAttack() const;

	//今の距離から攻撃を返す(空も返す可能性もあるのでnullチェックが必要)
	std::shared_ptr<AttackData> GetAttackByDistance()const;

	//目が光るエフェクト
	void InitLightUpEyesEff();
protected:
	//攻撃クールタイム
	float m_attackCoolTime;
	//ロックオンされたときに見られる座標
	Vector3 m_lockOnViewPos;
	//活動
	bool m_isActive;
	//エリア内か
	bool m_isInArea;
	//ターゲット情報
	TargetInfo m_targetInfo;
	//警戒状態
	bool m_isAlerted;
	//攻撃のキー
	std::vector<std::wstring> m_meleeAttackKeys;	//近接攻撃のキー
	std::vector<std::wstring> m_longRangeAttackKeys;//遠距離攻撃のキー
	//右目のインデックス
	int m_rightEyeIndex;
	//左目のインデックス
	int m_leftEyeIndex;
	//近い目のインデックス
	int m_nearEyeIndex;
	//目のエフェクトの参照
	std::weak_ptr<NormalEffect> m_eyeEffect;
protected:
	//カウント
	void CountAttackCoolTime();
	//ロックオンされたときに見られる座標設定
	void UpdateLockOnViewPos();
	//ランダムに攻撃を取得
	std::shared_ptr<AttackData> GetRandomAttack(std::vector<std::wstring> keys) const;
	//攻撃のキーの初期化
	void InitAttackKey(CSVDataLoader& csvLoader, std::wstring path);
	//目の位置のインデックス
	void InitEyeIndex(CSVDataLoader& csvLoader, std::wstring path);
	//目の位置更新
	void UpdateEyeEffect();
};

