#include "Boss4StateAttack.h"
#include "Boss4StateIdle.h"
#include "Boss4.h"
#include "../EnemyStateDeath.h"
#include "../../Player/Player.h"
#include "../../../../Attack/AttackBase.h"
#include "../../../../Attack/SwordAttack.h"
#include "../../../../Attack/AOEAttack.h"
#include "../../../../Attack/EnemyBulletAttack.h"
#include "../../../../../General/Math/MyMath.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/CSV/AttackData.h"
#include "../../../../../General/Collision/Rigidbody.h"

namespace
{
	//弾のばらつき
	constexpr int kBulletAngle = 10;
	//トリガー判定に変換
	constexpr int kTriggerFrame = 280;
}

Boss4StateAttack::Boss4StateAttack(std::weak_ptr<Actor> enemy, std::shared_ptr<AttackData> attackData) :
	EnemyStateAttack(enemy, attackData)
{
}

Boss4StateAttack::~Boss4StateAttack()
{
}

void Boss4StateAttack::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void Boss4StateAttack::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Boss4>(m_pOwner.lock());

	//強制待機状態へ
	if (m_isWait)
	{
		ChangeState(std::make_shared<Boss4StateIdle>(m_pOwner));
		return;
	}

	//ステータス
	auto status = owner->GetCharaStatus();
	//死亡
	if (status->IsDead())
	{
		ChangeState(std::make_shared<EnemyStateDeath>(m_pOwner));
		return;
	}

	//フレームをカウント
	CountFrame();

	//モデル
	auto model = owner->GetModel();

	//フレームによる攻撃の処理
	UpdateAttackFrame(owner);
	//攻撃移動更新
	UpdateMove(owner, model);
	//攻撃位置更新
	UpdateAttackPos(owner);

	//アニメーションが終了したら
	if (model->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<Boss4StateIdle>(m_pOwner));
		return;
	}
}


void Boss4StateAttack::CreateAttack(std::shared_ptr<EnemyBase> owner)
{
	if (!m_attackData)return;
	std::shared_ptr<AttackBase> attack;

	//腕による攻撃の場合
	int armIndex = m_attackData->GetParam4();
	//攻撃が発動するかをチェック
	bool isAttackable = true;

	//攻撃作成
	if (m_attackData->GetAttackType() == AttackData::AttackType::Sword)
	{
		attack = std::make_shared<SwordAttack>(m_attackData, owner);
	}
	else if (m_attackData->GetAttackType() == AttackData::AttackType::AOE)
	{
		attack = std::make_shared<AOEAttack>(m_attackData, owner);
	}
	else if (m_attackData->GetAttackType() == AttackData::AttackType::Bullet)
	{
		attack = std::make_shared<EnemyBulletAttack>(m_attackData, owner);
		//弾の初期位置と方向を設定
		auto bulletAttack = std::dynamic_pointer_cast<EnemyBulletAttack>(attack);
		auto model = owner->GetModel();

		//発射座標
		Vector3 bulletPos = MV1GetFramePosition(model->GetModelHandle(), static_cast<int>(m_attackData->GetParam1()));
		bulletAttack->SetPos(bulletPos);

		//モデルの向き
		Vector3 bulletDir = model->GetDir();

		//ターゲットがいるとき
		if (owner->GetTargetInfo().m_isFound)
		{
			auto player = std::dynamic_pointer_cast<Player>(owner->GetTargetInfo().m_pTarget.lock());
			Vector3 targetDir = player->GetCenterPos() - bulletPos;
			if (targetDir.SqMagnitude() > 0.0f)
			{
				targetDir = targetDir.Normalize();
			}
			bulletDir = targetDir;
		}
		//正規化
		if (bulletDir.SqMagnitude() > 0.0f)
		{
			bulletDir = bulletDir.Normalize();
		}
		//角度をランダムで回転
		bulletDir = Quaternion::AngleAxis(static_cast<float>(MyMath::GetRand(-kBulletAngle, kBulletAngle)) * MyMath::DEG_2_RAD, Vector3::Up().Cross(bulletDir)) * bulletDir;
		bulletDir = Quaternion::AngleAxis(static_cast<float>(MyMath::GetRand(-kBulletAngle, kBulletAngle)) * MyMath::DEG_2_RAD, Vector3::Up()) * bulletDir;

		//移動
		bulletAttack->SetMoveVec(bulletDir * m_attackData->GetParam2());

		//壊れるか
		bulletAttack->SetIsDestructible(m_attackData->GetParam3() != 0.0f);

		//弾を打った
		m_isShotBullet = true;
	}
	owner->SetAttack(attack);
	m_pAttack = attack;
	m_isAppearedAttack = true;
}
