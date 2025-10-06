#pragma once
#include "../CharacterBase.h"
#include <memory>
#include <string>
#include "../../../../General/Model.h"
class Player;
class HitPoints;
//敵の階級
enum class EnemyGrade
{
	Normal,   //雑魚敵
	Elite,    //中ボス
	Boss      //ボス
};
class EnemyBase abstract :
    public CharacterBase
{
public:
    EnemyBase(Shape shape, EnemyGrade grade);
    virtual ~EnemyBase() {};
	//更新
	virtual void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//描画
	void Draw() const override;
	//攻撃のクールタイムを更新
	virtual void UpdateAttackCoolTime();
	//攻撃のクールタイムを取得
	int GetAttackCoolTime() const { return m_attackCoolTime; }
	//攻撃のクールタイムをセット
	void SetAttackCoolTime(int coolTime) { m_attackCoolTime = coolTime; }
	//敵の階級
	EnemyGrade GetEnemyGrade()const { return m_enemyGrade; };
	//行動可能か
	bool IsActive()const { return m_isActive; };
	void SetActive(bool isActive);
	//警戒
	bool IsWarning()const { return m_isWarning; };
	void SetIsWarning(bool isWarning) { m_isWarning = isWarning; };
	//名前
	std::wstring GetName()const { return m_name; };
protected:
	//攻撃できるまでのクールタイム
	int m_attackCoolTime;
	//自分の階級
	EnemyGrade m_enemyGrade;
	//行動開始
	bool m_isActive;
	//プレイヤーとの距離が遠いときに当たり判定を停止する
	bool IsStopActiveCollision(const std::weak_ptr<ActorManager> actorManager);
	//警戒している
	bool m_isWarning;
	//名前
	std::wstring m_name;
};

