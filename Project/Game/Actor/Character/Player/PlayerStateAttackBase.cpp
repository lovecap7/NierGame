#include "PlayerStateAttackBase.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "Player.h"

PlayerStateAttackBase::PlayerStateAttackBase(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_isAppearedAttack(false),
	m_attackData(),
	m_pSwordAttack(),
	m_chargeCountFrame(0.0f)
{
}

PlayerStateAttackBase::~PlayerStateAttackBase()
{
	//çUåÇÇÃçÌèú
	DeleteAttack();
}

void PlayerStateAttackBase::DeleteAttack()
{
	if (m_pSwordAttack.expired())return;
	auto attack = m_pSwordAttack.lock();
	attack->Delete();
}

void PlayerStateAttackBase::CreateAttack(std::shared_ptr<Player> owner, std::shared_ptr<Weapon> weapon)
{
	if (!m_attackData)return;
	//çUåÇçÏê¨
	auto attack = std::make_shared<SwordAttack>(m_attackData, owner);
	owner->SetAttack(attack);
	m_pSwordAttack = attack;
	m_isAppearedAttack = true;

	auto model = owner->GetModel();
	//ìäÇ∞Ç‡ÇÃçUåÇÇÃèàóù
	if (m_attackData->m_attackType == AttackData::AttackType::Throw)
	{
		//ìäÇ∞Ç¬Ç¬âÒì]
		weapon->ThrowAndRoll(
			m_attackData->m_param1, model->GetDir(),
			owner->GetPos(), m_attackData->m_keepFrame, m_attackData->m_param2);
	}
	else
	{
		//âÒì]èIóπ
		weapon->FinisiThrowAndRoll();
	}
}

void PlayerStateAttackBase::UpdateAttackPosition(std::shared_ptr<Player> owner, std::shared_ptr<Weapon> weapon)
{
	if (!m_attackData)return;
	if (!weapon)return;
	if (m_pSwordAttack.expired())return;
	//çUåÇÇÃà íuçXêV
	auto attack = m_pSwordAttack.lock();
	attack->SetStartPos(weapon->GetStartPos());
	attack->SetEndPos(weapon->GetEndPos(m_attackData->m_length));
}

void PlayerStateAttackBase::LoadNextMultipleHitAttack(std::shared_ptr<Player> owner)
{
	if (!m_attackData)return;
	// ëΩíiÉqÉbÉgèàóù
	m_attackData = owner->GetAttackData(m_attackData->m_nextAttackName);
	m_isAppearedAttack = false;
}

void PlayerStateAttackBase::UpdateMove(std::shared_ptr<Player> owner, Input& input, std::shared_ptr<Model> model)
{
	if (!m_attackData)return;
	Vector3 moveVec = Vector3::Zero();
	if (m_frame < m_attackData->m_moveFrame)
	{
		model->SetDir(InputMoveVec(owner, input).XZ());
		moveVec = model->GetDir() * m_attackData->m_moveSpeed;
	}

	owner->GetRb()->SetMoveVec(moveVec);
}
