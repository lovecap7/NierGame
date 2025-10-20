#include "PlayerStateAttackBase.h"
#include "Weapon/Weapon.h"
#include "../../../Attack/SwordAttack.h"
#include "../../../Attack/AOEAttack.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "Player.h"

PlayerStateAttackBase::PlayerStateAttackBase(std::weak_ptr<Actor> player):
	PlayerStateBase(player),
	m_isAppearedAttack(false),
	m_attackData(),
	m_pAttack(),
	m_chargeCountFrame(0.0f)
{
}

PlayerStateAttackBase::~PlayerStateAttackBase()
{
	//UŒ‚‚Ìíœ
	DeleteAttack();
}

void PlayerStateAttackBase::DeleteAttack()
{
	if (m_pAttack.expired())return;
	auto attack = m_pAttack.lock();
	attack->Delete();
}

void PlayerStateAttackBase::CreateAttack(std::shared_ptr<Player> owner, std::shared_ptr<Weapon> weapon)
{
	if (!m_attackData)return;
	std::shared_ptr<AttackBase> attack;
	//UŒ‚ì¬
	if (m_attackData->m_attackType == AttackData::AttackType::Sword ||
		m_attackData->m_attackType == AttackData::AttackType::Throw)
	{
		attack = std::make_shared<SwordAttack>(m_attackData, owner);
	}
	else if (m_attackData->m_attackType == AttackData::AttackType::AOE)
	{
		attack = std::make_shared<AOEAttack>(m_attackData, owner);
	}
	owner->SetAttack(attack);
	m_pAttack = attack;
	m_isAppearedAttack = true;

	auto model = owner->GetModel();
	//“Š‚°‚à‚ÌUŒ‚‚Ìˆ—
	if (m_attackData->m_attackType == AttackData::AttackType::Throw)
	{
		//“Š‚°‚Â‚Â‰ñ“]
		weapon->ThrowAndRoll(
			m_attackData->m_param1, model->GetDir(),
			owner->GetPos(), m_attackData->m_keepFrame, m_attackData->m_param2);
	}
	else
	{
		//‰ñ“]I—¹
		weapon->FinisiThrowAndRoll();
	}
}

void PlayerStateAttackBase::UpdateAttackPosition(std::shared_ptr<Player> owner, std::shared_ptr<Weapon> weapon)
{
	if (!m_attackData)return;
	if (!weapon)return;
	if (m_pAttack.expired())return;
	//UŒ‚‚ÌˆÊ’uXV
	if (m_attackData->m_attackType == AttackData::AttackType::Sword ||
		m_attackData->m_attackType == AttackData::AttackType::Throw)
	{
		auto attack = std::dynamic_pointer_cast<SwordAttack>(m_pAttack.lock());
		attack->SetStartPos(weapon->GetStartPos());
		attack->SetEndPos(weapon->GetEndPos(m_attackData->m_length));
	}
	else if (m_attackData->m_attackType == AttackData::AttackType::AOE)
	{
		auto attack = std::dynamic_pointer_cast<AOEAttack>(m_pAttack.lock());
		attack->SetPos(weapon->GetStartPos());
	}
}

void PlayerStateAttackBase::LoadNextMultipleHitAttack(std::shared_ptr<Player> owner)
{
	if (!m_attackData)return;
	// ‘½’iƒqƒbƒgˆ—
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
