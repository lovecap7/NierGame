#include "PlayerStateLightAttack.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateIdle.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"

PlayerStateLightAttack::PlayerStateLightAttack(std::weak_ptr<Actor> player, std::wstring attackName):
	PlayerStateBase(player),
	m_isAppearedAttack(false)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);

	//武器を持つ
	owner->HaveLightSword();
	//攻撃データ取得
	m_attackData = owner->GetAttackData(attackName);
	//アニメーション
	owner->GetModel()->SetAnim(owner->GetAnim(m_attackData->m_animName).c_str(), false);
}

PlayerStateLightAttack::~PlayerStateLightAttack()
{
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

	//フレームをカウント
	CountFrame();

	//発生フレームになったら
	if (m_frame >= m_attackData->m_startFrame && !m_isAppearedAttack)
	{
		//攻撃発生
		std::shared_ptr<SwordAttack> attack = std::make_shared<SwordAttack>(m_attackData, owner);

		//攻撃を入れる
		owner->SetAttack(attack);

		//参照
		m_pSwordAttack = attack;

		//攻撃が発生した
		m_isAppearedAttack = true;
	}

	//モデル
	auto model = owner->GetModel();

	//攻撃位置の更新
	if (!m_pSwordAttack.expired())
	{
		//攻撃
		auto swordAttack = m_pSwordAttack.lock();

		//武器
		auto weapon = owner->GetWeapon(PlayerAnimData::WeaponType::LightSword);
		if (!weapon.expired())
		{
			auto lightSword = weapon.lock();
			//始点
			swordAttack->SetStartPos(lightSword->GetStartPos());
			//終点
			swordAttack->SetEndPos(lightSword->GetEndPos(m_attackData->m_length));

			//テスト
			if (m_frame == m_attackData->m_startFrame)
			{
				lightSword->ThrowAndRoll(300.0f, model->GetDir(), owner->GetPos(), 60, 10.0f);
			}
		}
	}

	auto& input = Input::GetInstance();

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

				//攻撃の削除
				if (!m_pSwordAttack.expired())
				{
					m_pSwordAttack.lock()->Delete();
				}

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
	//移動量リセット
	owner->GetRb()->SetMoveVec(Vector3::Zero());
}
