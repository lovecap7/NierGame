#include "SwordAttack.h"
#include "../../General/Collision/CapsuleCollider.h"
#include "../../General/Collision/Rigidbody.h"

SwordAttack::SwordAttack(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner) :
	CapsuleAttackBase(attackData, pOwner)
{
}

SwordAttack::~SwordAttack()
{
}

void SwordAttack::Draw() const
{
#if _DEBUG
	//Õ“Ë”»’è
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff00ff,
		0xff00ff,
		false//’n–Ê‚É‚¢‚é‚Æ“h‚è‚Â‚Ô‚³‚ê‚é
	);
#endif
}

void SwordAttack::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
}
