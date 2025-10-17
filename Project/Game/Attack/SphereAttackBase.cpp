#include "SphereAttackBase.h"
#include "../../General/Collision/Rigidbody.h"
#include "../../General/Collision/SphereCollider.h"

SphereAttackBase::SphereAttackBase(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner) :
	AttackBase(Shape::Sphere, attackData, pOwner)
{
}

void SphereAttackBase::SetPos(const Vector3& pos)
{
	m_rb->m_pos = pos;
}

void SphereAttackBase::SetRadius(float radius)
{
	std::dynamic_pointer_cast<SphereCollider>(m_collisionData)->SetRadius(radius);
}
