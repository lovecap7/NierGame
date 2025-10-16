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
	const std::wstring kFirstAttackName = L"MainAttack1";
}

PlayerStateLightAttack::PlayerStateLightAttack(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_isAppearedAttack(false)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);

	//武器を持つ
	owner->HaveLightSword();
	//攻撃データ取得
	m_attackData = owner->GetAttackData(kFirstAttackName);
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

	//フレームをカウント
	CountFrame();
	
	//武器
	auto weapon = owner->GetWeapon(PlayerAnimData::WeaponType::LightSword);
	if (weapon.expired())return;
	auto lightSword = weapon.lock();

	//モデル
	auto model = owner->GetModel();

	//発生フレームになったら
	if (m_frame >= m_attackData->m_startFrame)
	{
		if (m_isAppearedAttack)
		{
			//発生した後に攻撃が消失したら
			if (m_pSwordAttack.expired())
			{
				//多段ヒット攻撃なら次の攻撃データを読み込む
				if (m_attackData->m_isMultipleHit && m_attackData->m_nextAttackName != L"None")
				{
					//攻撃データ
					m_attackData = owner->GetAttackData(m_attackData->m_nextAttackName);
					//もう一度攻撃判定を出す
					m_isAppearedAttack = false;
				}
			}
		}
		//まだ攻撃が発生していないなら発生
		if (!m_isAppearedAttack)
		{
			//攻撃発生
			std::shared_ptr<SwordAttack> attack = std::make_shared<SwordAttack>(m_attackData, owner);

			//攻撃を入れる
			owner->SetAttack(attack);

			//参照
			m_pSwordAttack = attack;

			//攻撃が発生した
			m_isAppearedAttack = true;

			//刀を投げる攻撃の時
			if (m_attackData->m_attackType == AttackData::AttackType::Throw)
			{
				lightSword->ThrowAndRoll(m_attackData->m_param1, model->GetDir(), owner->GetPos(), m_attackData->m_keepFrame, m_attackData->m_param2);
			}
			else
			{
				lightSword->FinisiThrowAndRoll();
			}
		}
	}

	//攻撃位置の更新
	if (!m_pSwordAttack.expired())
	{
		//攻撃
		auto swordAttack = m_pSwordAttack.lock();

		//攻撃の位置更新
		//始点
		swordAttack->SetStartPos(lightSword->GetStartPos());
		//終点
		swordAttack->SetEndPos(lightSword->GetEndPos(m_attackData->m_length));
	}
	
	//キャンセルフレーム
	if ((model->GetTotalAnimFrame() - m_attackData->m_cancelFrame) < m_frame)
	{
		//攻撃
		if (input.IsBuffered("X"))
		{
			owner->HaveLightSword();
			//次の攻撃
			if (m_attackData->m_nextAttackName != L"None")
			{
				//攻撃データ
				m_attackData = owner->GetAttackData(m_attackData->m_nextAttackName);
				m_isAppearedAttack = false;

				//アニメーション
				model->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);

				//フレームリセット
				m_frame = 0.0f;

				DeleteAttack();

				return;
			}
		}
	}
	//アニメーションが終了したら
	if (model->IsFinishAnim())
	{
		if (input.GetStickInfo().IsLeftStickInput())
		{
			//移動
			ChangeState(std::make_shared<PlayerStateMoving>(m_pOwner, false));
			return;
		}
		else
		{
			//待機
			ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
			return;
		}
	}

	//移動量
	Vector3 moveVec = Vector3::Zero();
	if (m_frame <= m_attackData->m_moveFrame)
	{
		//モデルの向き
		owner->GetModel()->SetDir(InputMoveVec(owner, input).XZ());

		//前進
		moveVec = model->GetDir()* m_attackData->m_moveSpeed;
	}
	//移動量リセット
	owner->GetRb()->SetVec(moveVec);
}

void PlayerStateLightAttack::DeleteAttack()
{
	//攻撃の削除
	if (!m_pSwordAttack.expired())
	{
		m_pSwordAttack.lock()->Delete();
	}
}
