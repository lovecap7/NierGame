#include "PlayerStateLightAttack.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateHeavyAttack.h"
#include "PlayerStateJump.h"
#include "PlayerStateFall.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	const std::wstring kFirstGroundAttackName = L"MainAttack1";
	const std::wstring kChargeName = L"MainAttackCharge";
	const std::wstring kJumpAttackName = L"RisingAttack";
	const std::wstring kJustAttackName = L"JustAttackMain";
	constexpr float kChargeFrame = 20.0f;
}

PlayerStateLightAttack::PlayerStateLightAttack(std::weak_ptr<Actor> player, bool isJump, bool isJust):
	PlayerStateAttackBase(player)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);

	//空中にいるなら
	if (!owner->IsFloor())
	{
		//重力を受けない
		owner->GetRb()->SetIsGravity(false);
		owner->SetIsAirAttacked(true);
		//縦の移動量をリセット
		owner->GetRb()->SetVecY(0.0f);
	}

	//武器を持つ
	owner->HaveLightSword();

	//ジャスト回避
	if (isJust)
	{
		//攻撃データ取得
		m_attackData = owner->GetAttackData(kJustAttackName);
		//上昇
		owner->GetRb()->SetVecY(m_attackData->m_param1);
		//この攻撃の場合重力を受ける
		owner->GetRb()->SetIsGravity(true);
	}
	//ジャンプ中なら
	else if (isJump)
	{
		//攻撃データ取得
		m_attackData = owner->GetAttackData(kJumpAttackName);
		//上昇
		owner->GetRb()->SetVecY(m_attackData->m_param1);
		//この攻撃の場合重力を受ける
		owner->GetRb()->SetIsGravity(true);
	}
	else
	{
		//攻撃データ取得
		m_attackData = owner->GetAttackData(kFirstGroundAttackName);
	}
	//アニメーション
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);
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
		if (m_isAppearedAttack && m_pAttack.expired())
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
	if (input.IsPress("X") && m_attackData->m_animName != kChargeName && owner->IsFloor())
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
			if (nextName != L"None")
			{
				//攻撃データ
				m_attackData = owner->GetAttackData(nextName);
				m_isAppearedAttack = false;

				//アニメーション
				model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);

				//フレームリセット
				m_frame = 0.0f;

				//攻撃削除
				DeleteAttack();

				//切り上げ攻撃以外なら重力を受けない
				if (nextName != kJumpAttackName)
				{
					//重力を受けない
					owner->GetRb()->SetIsGravity(false);
					//縦の移動量をリセット
					owner->GetRb()->SetVecY(0.0f);
				}

				return;
			}
		}
		//ジャンプ
		if (owner->IsJumpable() && input.IsBuffered("A"))
		{
			ChangeState(std::make_shared<PlayerStateJump>(m_pOwner));
			return;
		}
		//大剣攻撃
		if (input.IsBuffered("Y"))
		{
			ChangeState(std::make_shared<PlayerStateHeavyAttack>(m_pOwner));
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
	//空中にいるなら落下
	if (!owner->IsFloor())
	{
		ChangeState(std::make_shared<PlayerStateFall>(m_pOwner));
		return;
	}
	if (input.GetStickInfo().IsLeftStickInput())
	{
		ChangeState(std::make_shared<PlayerStateMoving>(m_pOwner, false));
	}
	else
	{
		ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
	}
}
