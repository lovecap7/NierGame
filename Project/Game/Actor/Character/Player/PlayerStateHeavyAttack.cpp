#include "PlayerStateHeavyAttack.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateLightAttack.h"
#include "PlayerStateJump.h"
#include "PlayerStateHit.h"
#include "PlayerStateDeath.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

namespace
{
	const std::wstring kFirstGroundAttackName = L"SubAttack1";
	const std::wstring kStartAirAttackName = L"SubAttackAirStart";
	const std::wstring kEndAirAttackName = L"SubAttackAirEnd";
	const std::wstring kDashAttackName = L"DashAttack";
	const std::wstring kChargeAttackName = L"SubAttack3";
	const std::wstring kChargeName = L"SubAttackCharge";
	const std::wstring kJustAttackName = L"JustAttackSub";
}

PlayerStateHeavyAttack::PlayerStateHeavyAttack(std::weak_ptr<Actor> player,bool isDash, bool isJust):
	PlayerStateAttackBase(player),
	m_nextAttackName(),
	m_update(&PlayerStateHeavyAttack::GroundUpdate),
	m_isJust(isJust)
{
	if (m_pOwner.expired()) return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);
	owner->HaveBigSword();


	//ジャスト回避
	if (isJust)
	{
		m_isJust = isJust;
		//攻撃データ取得
		m_attackData = owner->GetAttackData(kJustAttackName);
	}
	else if (!owner->IsFloor())
	{
		//空中か地上かで初期攻撃設定
		m_attackData = owner->GetAttackData(kStartAirAttackName);
		m_update = &PlayerStateHeavyAttack::AirUpdate;
		owner->SetCollState(CollisionState::Fall);
		owner->GetRb()->SetVec(Vector3::Zero());
		owner->SetIsAirAttacked(true);
	}
	else
	{
		//ダッシュ状態なら
		if (isDash)
		{
			m_attackData = owner->GetAttackData(kDashAttackName);
		}
		else
		{
			m_attackData = owner->GetAttackData(kFirstGroundAttackName);
		}
	}
	//アニメーション
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);

	//アーマー
	owner->ChangeArmor(m_attackData->m_armor);
}

PlayerStateHeavyAttack::~PlayerStateHeavyAttack()
{
	if (m_pOwner.expired()) return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->GetRb()->SetIsGravity(true);
	DeleteAttack();
	owner->GetCharaStatus()->SetIsNoDamage(false);

	//アーマーをもとに戻す
	owner->InitArmor();
}

void PlayerStateHeavyAttack::Init()
{
	ChangeState(shared_from_this());
	//重力を受けない
	if (m_pOwner.expired()) return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	// 空中か地上かで初期攻撃設定
	if (!owner->IsFloor())
	{
		owner->GetRb()->SetIsGravity(false);
	}
}

void PlayerStateHeavyAttack::Update()
{
	if (m_pOwner.expired()) return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto& input = Input::GetInstance();
	//死亡
	if (owner->GetCharaStatus()->IsDead())
	{
		ChangeState(std::make_shared<PlayerStateDeath>(m_pOwner));
		return;
	}
	//やられ
	if (owner->GetCharaStatus()->IsHitReaction())
	{
		ChangeState(std::make_shared<PlayerStateHit>(m_pOwner));
		return;
	}
	//無敵
	if( m_isJust)
	{
		owner->GetCharaStatus()->SetIsNoDamage(true);
	}

	//状態ごとの更新処理
	(this->*m_update)(owner, input);
}

void PlayerStateHeavyAttack::UpdateStartFrame(std::shared_ptr<Player> owner, std::shared_ptr<Weapon> weapon)
{
	if (m_frame >= m_attackData->m_startFrame)
	{
		//持続終了時に今の攻撃が多段ヒット攻撃なら次の攻撃を読み込む
		if (m_isAppearedAttack && m_pAttack.expired())
		{
			if (m_attackData->m_isMultipleHit && m_attackData->m_nextAttackName != L"None")
			{
				LoadNextMultipleHitAttack(owner,weapon);
				//空中攻撃の時
				if (m_update == &PlayerStateHeavyAttack::AirUpdate)
				{
					//落下していく
					owner->GetRb()->SetIsGravity(true);
					owner->GetRb()->SetVecY(m_attackData->m_param1);
				}
				return;
			}
		}
		//攻撃作成
		if (!m_isAppearedAttack)
		{
			CreateAttack(owner, weapon);
		}
	}
}


bool PlayerStateHeavyAttack::LoadNextChargeOrCombo(std::shared_ptr<Player> owner, Input& input, std::shared_ptr<Model> model)
{
	//キャンセルフレームの間
	if ((model->GetTotalAnimFrame() - m_attackData->m_cancelFrame) <= m_frame)
	{
		//武器を持つ
		owner->HaveBigSword();
		// チャージ処理
		if (input.IsPress("Y"))
		{
			if (m_chargeCountFrame <= 0.0f)
			{
				m_nextAttackName = m_attackData->m_nextAttackName;
				m_attackData = owner->GetAttackData(kChargeName);
				model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), true);
				DeleteAttack();
			}
			//チャージ
			m_chargeCountFrame += owner->GetTimeScale();
			return true;	//チャージ中はtrueを返す
		}
		//ジャンプ
		if (owner->IsJumpable() && input.IsBuffered("A"))
		{
			ChangeState(std::make_shared<PlayerStateJump>(m_pOwner));
			return true;
		}
		//片手剣攻撃
		if (input.IsBuffered("X"))
		{
			ChangeState(std::make_shared<PlayerStateLightAttack>(m_pOwner));
			return true;
		}
		// チャージ完了または未完了による攻撃遷移
		if(input.IsRelease("Y"))
		{
			//チャージ完了かどうか
			bool isCharged = ((m_chargeCountFrame >= m_attackData->m_param1) && 
				m_attackData->m_attackType == AttackData::AttackType::None);
			if (isCharged)
			{
				m_attackData = owner->GetAttackData(kChargeAttackName);
			}
			else
			{
				if (m_nextAttackName == L"")
				{
					m_nextAttackName = m_attackData->m_nextAttackName;
				}
				m_attackData = owner->GetAttackData(m_nextAttackName);
			}
			m_isAppearedAttack = false;
			model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);
			m_frame = 0.0f;
			m_chargeCountFrame = 0.0f;
			DeleteAttack();
			return false;
		}
	}
	return false;
}

void PlayerStateHeavyAttack::ChangeToMoveOrIdle(std::shared_ptr<Player> owner, Input& input)
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


void PlayerStateHeavyAttack::GroundUpdate(std::shared_ptr<Player> owner, Input& input)
{
	// 回避
	if (input.IsBuffered("B") && owner->IsAvoidable())
	{
		return ChangeState(std::make_shared<PlayerStateAvoid>(m_pOwner));
	}
	//フレームのカウント
	CountFrame();
	auto model = owner->GetModel();
	if (owner->GetWeapon(AnimData::WeaponType::BigSword).expired())return;
	auto weapon = owner->GetWeapon(AnimData::WeaponType::BigSword).lock();

	// 攻撃発生処理
	UpdateStartFrame(owner, weapon);
	//位置更新
	UpdateAttackPosition(owner, weapon);
	// 移動
	UpdateMove(owner, input, model);

	//多段ヒット攻撃中はキャンセル攻撃をしない
	if (!m_attackData->m_isMultipleHit)
	{
		//チャージ攻撃関連の処理(チャージ中なら早期リターン)
		if (LoadNextChargeOrCombo(owner, input, model))return;
	}

	// アニメーション終了時
	if (model->IsFinishAnim())
	{
		return ChangeToMoveOrIdle(owner, input);
	}
}



void PlayerStateHeavyAttack::AirUpdate(std::shared_ptr<Player> owner, Input& input)
{
	//武器を持つ
	owner->HaveBigSword();
	//フレームのカウント
	CountFrame();

	auto model = owner->GetModel();
	if (owner->GetWeapon(AnimData::WeaponType::BigSword).expired())return;
	auto weapon = owner->GetWeapon(AnimData::WeaponType::BigSword).lock();

	// 攻撃発生処理
	UpdateStartFrame(owner, weapon);
	//移動と攻撃位置更新
	UpdateMove(owner, input, model);
	UpdateAttackPosition(owner, weapon);

	// 着地処理
	if (owner->IsFloor() && model->IsFinishAnim())
	{
		//着地時のアニメーション読み込み
		if (m_attackData->m_animName != m_attackData->m_nextAttackName)
		{
			m_attackData = owner->GetAttackData(m_attackData->m_nextAttackName);
			m_isAppearedAttack = false;
			model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);
			m_frame = 0.0f;
			DeleteAttack();
			return;
		}

		//着地アニメーションが終了後
		return ChangeToMoveOrIdle(owner, input);
	}
}
