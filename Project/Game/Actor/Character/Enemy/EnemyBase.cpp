#include "EnemyBase.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/Model.h"
#include "../Player/Player.h"
#include "../../../../General/MyDraw.h"
namespace
{
	//Œx‰úó‘Ô‚È‚çõ“G”ÍˆÍ‚ğL‚°‚é
	constexpr float kAlertedDisRate = 2.0f;
}

EnemyBase::EnemyBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager):
	CharacterBase(actorData, charaStatusData, Shape::Capsule, pActorManager),
	m_attackCoolTime(0.0f),
	m_isActive(true),
	m_isAlerted(true),
	m_isInArea(false)
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::Update()
{
	//UŒ‚ƒN[ƒ‹ƒ^ƒCƒ€XV
	CountAttackCoolTime();
	//‹¤’Êˆ—
	CharacterBase::Update();
	//ƒƒbƒNƒIƒ“‚ÌˆÊ’u
	UpdateLockOnViewPos();
}


void EnemyBase::Draw() const
{
	//”ñŠˆ“®’†‚Í•`‰æ‚µ‚È‚¢
	if (!m_isActive)return;
#if _DEBUG
	//Õ“Ë”»’è
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0x00ff00,
		0x00ff00,
		false
	);
	//À•W
	auto pos = m_rb->m_pos;
	//‹–ìŠp
	float viewingAngle = m_charaStatus->GetSearchAngle();
	//õ“G‹——£
	float searchDistance = m_charaStatus->GetSearchRange();

	//Œx‰ú
	if (m_isAlerted)
	{
		//’·‚­
		searchDistance *= kAlertedDisRate;
	}

	//õ“G”ÍˆÍ
	MyDraw::Draw3DCircle(pos, searchDistance, 36, 0x0000ff);
	//Œ©‚Ä‚é•ûŒü
	auto forward = m_model->GetDir();
	forward = forward * searchDistance;
	//‹–ìŠp
	auto viewDir1 = Quaternion::AngleAxis(viewingAngle / 2.0f, Vector3::Up()) * forward;
	auto viewDir2 = Quaternion::AngleAxis(-viewingAngle / 2.0f, Vector3::Up()) * forward;
	//•`‰æ
	DrawLine3D(pos.ToDxLibVector(), (pos + forward).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir1).ToDxLibVector(), 0xff0000);
	DrawLine3D(pos.ToDxLibVector(), (pos + viewDir2).ToDxLibVector(), 0xff0000);
#endif
	m_model->Draw();
}


bool EnemyBase::IsEnableAttack() const
{
	return m_targetInfo.m_isFound && m_attackCoolTime <= 0.0f;
}

void EnemyBase::UpdateLockOnViewPos()
{
	//ƒƒbƒNƒIƒ“À•W
	Vector3 start = m_rb->GetPos();
	Vector3 end = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos();
	m_lockOnViewPos = (start + end) * 0.5f;
}

void EnemyBase::SearchTarget(std::shared_ptr<Player> player)
{
	if (!player)return;

	//‘O‰ñ‚Ì”­Œ©ó‘Ô‚ğ•Û‘¶
	m_isAlerted = m_targetInfo.m_isFound;
	//‹–ìŠp“à‚É‚¢‚é‚©‚Ç‚¤‚©
	m_targetInfo.m_isFound = false;
	//õ“G‹——£
	float searchDis = m_charaStatus->GetSearchRange();

	//UŒ‚‚ğ‹ò‚ç‚Á‚½‚çŒx‰ú‚·‚é
	if (m_charaStatus->IsHit() || m_isAlerted)
	{
		//Œx‰ú
		m_isAlerted = true;
		//’·‚­
		searchDis *= kAlertedDisRate;
	}

	//ƒvƒŒƒCƒ„[‚ÌˆÊ’u
	auto targetPos = player->GetRb()->GetPos();

	//‹——£‚ğŠm”F
	auto myPos = m_rb->GetPos();
	auto dir = targetPos.XZ() - myPos.XZ();
	if (dir.Magnitude() <= searchDis)
	{
		//Œx‰úó‘Ô‚È‚ç–³ğŒ‚Å”­Œ©
		if (m_isAlerted)
		{
			m_targetInfo.m_pTarget = player;
			m_targetInfo.m_isFound = true;
			return;
		}

		//‹–ìŠp“à‚Éƒ^[ƒQƒbƒg‚ª‚¢‚é‚©
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (m_charaStatus->GetSearchAngle() / 2.0f))
		{
			m_targetInfo.m_pTarget = player;
			m_targetInfo.m_isFound = true;
			m_isAlerted = true;
		}
	}
}

Vector3 EnemyBase::GetToTargetVec() const
{
	{
		if (!m_targetInfo.m_isFound || m_targetInfo.m_pTarget.expired())
		{
			return Vector3::Zero();
		}
		auto target = m_targetInfo.m_pTarget.lock();
		Vector3 toTarget = target->GetRb()->GetPos() - this->GetRb()->GetPos();
		if (toTarget.SqMagnitude() > 0.0f)
		{
			toTarget = toTarget.Normalize();
		}
		return toTarget;
	}
}

bool EnemyBase::IsInMeleeRange() const
{
	if (!m_targetInfo.m_isFound || m_targetInfo.m_pTarget.expired())
	{
		return false;
	}
	auto target = m_targetInfo.m_pTarget.lock();
	Vector3 toTarget = target->GetRb()->GetPos() - this->GetRb()->GetPos();
	float distance = toTarget.Magnitude();
	if (distance < m_charaStatus->GetMeleeAttackRange())
	{
		return true;
	}
	return false;
}

void EnemyBase::CountAttackCoolTime()
{
	if (m_attackCoolTime <= 0.0f)return;
	m_attackCoolTime -= GetTimeScale();
}

bool EnemyBase::IsEnableLongRangeAttack() const
{
	//‰“‹——£UŒ‚ƒL[‚ª‚ ‚èA‚©‚Â‹ßÚ”ÍˆÍŠO‚È‚ç‰Â”\
	return (m_longRangeAttackKeys.size() > 0) && !IsInMeleeRange();
}

bool EnemyBase::IsEnableMeleeAttack() const
{
	//‹ßÚUŒ‚ƒL[‚ª‚ ‚èA‚©‚Â‹ßÚ”ÍˆÍ“à‚È‚ç‰Â”\
	return (m_meleeAttackKeys.size() > 0) && IsInMeleeRange();
}

std::shared_ptr<AttackData> EnemyBase::GetAttackByDistance() const
{
	//UŒ‚ƒf[ƒ^
	std::shared_ptr<AttackData> attackData;

	//‰“‹——£UŒ‚‰Â”\‚È‚ç
	if (IsEnableLongRangeAttack())
	{
		attackData = GetRandomAttack(m_longRangeAttackKeys);
	}
	//‹ßÚUŒ‚‰Â”\‚È‚ç
	else if (IsEnableMeleeAttack())
	{
		attackData = GetRandomAttack(m_meleeAttackKeys);
	}

	return attackData;
}
std::shared_ptr<AttackData> EnemyBase::GetRandomAttack(std::vector<std::wstring> keys) const
{
	int index = MyMath::GetRand(0, keys.size() - 1);
	return GetAttackData(keys[index]);
}

