#include "BulletAttack.h"
#include "../../General/Collision/SphereCollider.h"
#include "../../General/Collision/Rigidbody.h"

BulletAttack::BulletAttack(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner):
	SphereAttackBase(attackData,pOwner),
	m_moveVec(Vector3::Zero())
{
}

BulletAttack::~BulletAttack()
{
}

void BulletAttack::Update()
{
	//“–‚½‚Á‚½
	if (m_isHit)
	{
		m_keepFrame = 0.0f;
		return;
	}
	AttackBase::Update();

	//ˆÚ“®
	m_rb->SetVec(m_moveVec);
}

void BulletAttack::Draw() const
{
	auto coll = std::dynamic_pointer_cast<SphereCollider>(m_collisionData);
	DrawSphere3D(m_rb->m_pos.ToDxLibVector(),
		coll->GetRadius(), 16, 0xff0000, 0xffffff, true);
}

void BulletAttack::OnCollide(const std::shared_ptr<Collidable> other)
{
	AttackBase::OnCollide(other);
}
