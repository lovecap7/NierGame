#include "PlayerStateLightAttack.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	const std::wstring kFirstGroundAttackName = L"MainAttack1";
	const std::wstring kChargeName = L"MainAttackCharge";
	constexpr float kChargeFrame = 20.0f;
}

PlayerStateLightAttack::PlayerStateLightAttack(std::weak_ptr<Actor> player):
	PlayerStateAttackBase(player)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);

	//武器を持つ
	owner->HaveLightSword();
	//攻撃データ取得
	m_attackData = owner->GetAttackData(kFirstGroundAttackName);
	//アニメーション
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);

	//空中にいるなら
	if (!owner->IsFloor())
	{
		//重力を受けない
		owner->GetRb()->SetIsGravity(false);
	}
}

PlayerStateLightAttack::~PlayerStateLightAttack()
{
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	//重力を受ける
	owner->GetRb()->SetIsGravity(true);
	//攻撃削除
	DeleteAttack();
}

void PlayerStateLightAttack::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateLightAttack::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());

	auto& input = Input::GetInstance();
	//回避
	if (input.IsBuffered("B") && owner->IsAvoidable())
	{
		//回避
		ChangeState(std::make_shared<PlayerStateAvoid>(m_pOwner));
		return;
	}

	//空中の時
	bool isAir = !owner->IsFloor();
	if (isAir)
	{
		//縦の移動量をリセット
		owner->GetRb()->SetVecY(0.0f);
	}

	//フレームをカウント
	CountFrame();
	
	//武器
	if (owner->GetWeapon(PlayerAnimData::WeaponType::LightSword).expired())return;
	auto weapon = owner->GetWeapon(PlayerAnimData::WeaponType::LightSword).lock();

	//モデル
	auto model = owner->GetModel();

	//発生フレームになったら
	if (m_frame >= m_attackData->m_startFrame)
	{
		//多段ヒット攻撃の処理
		if (m_isAppearedAttack && m_pSwordAttack.expired())
		{
			if (m_attackData->m_isMultipleHit && m_attackData->m_nextAttackName != L"None")
			{
				//多段ヒット攻撃
				LoadNextMultipleHitAttack(owner);
			}
		}
		//まだ攻撃が発生していないなら発生
		if (!m_isAppearedAttack)
		{
			CreateAttack(owner, weapon);
		}
	}

	//長押ししているフレームをカウント
	if (input.IsPress("X") && m_attackData->m_animName != kChargeName && !isAir)
	{
		m_chargeCountFrame += owner->GetTimeScale();
	}
	else
	{
		m_chargeCountFrame = 0.0f;
	}
	
	//キャンセルフレーム
	if ((model->GetTotalAnimFrame() - m_attackData->m_cancelFrame) < m_frame)
	{
		//攻撃の条件
		bool isChargeAttack = m_chargeCountFrame >= kChargeFrame;
		bool isCombAttack = input.IsBuffered("X");

		//武器を持つ
		owner->HaveLightSword();

		//攻撃をするか
		if (isChargeAttack || isCombAttack)
		{
			//次の攻撃名
			auto nextName = m_attackData->m_nextAttackName;
			
			//チャージ攻撃をするなら
			if (isChargeAttack)
			{
				nextName = kChargeName;
			}
			//次の攻撃がない場合
			else if (nextName == L"None")
			{
				//空中にいないなら
				if (owner->IsFloor())
				{
					//最初の攻撃に戻る
					nextName = kFirstGroundAttackName;
				}
			}

			//攻撃データ
			m_attackData = owner->GetAttackData(nextName);
			m_isAppearedAttack = false;

			//アニメーション
			model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);

			//フレームリセット
			m_frame = 0.0f;

			DeleteAttack();

			return;
		}
	}
	//アニメーションが終了したら
	if (model->IsFinishAnim())
	{
		return ChangeToMoveOrIdle(owner,input);
	}
	//移動
	UpdateMove(owner, input, model);
	//攻撃位置の更新
	UpdateAttackPosition(owner, weapon);
}

void PlayerStateLightAttack::ChangeToMoveOrIdle(std::shared_ptr<Player> owner, Input& input)
{
	if (input.GetStickInfo().IsLeftStickInput())
	{
		ChangeState(std::make_shared<PlayerStateMoving>(m_pOwner, false));
	}
	else
	{
		ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
	}
}
