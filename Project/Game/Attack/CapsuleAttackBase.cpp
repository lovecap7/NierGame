#include "CapsuleAttackBase.h"
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Collision/CapsuleCollider.h"

CapsuleAttackBase::CapsuleAttackBase(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner):
	AttackBase(Shape::Capsule, attackData, pOwner)
{
}

void CapsuleAttackBase::SetStartPos(const Vector3& pos)
{
	m_rb->m_pos = pos;
}

void CapsuleAttackBase::SetEndPos(const Vector3& pos)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(pos);
}

void CapsuleAttackBase::SetRadius(float radius)
{
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetRadius(radius);
}

