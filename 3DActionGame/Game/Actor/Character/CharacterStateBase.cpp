#include "CharacterStateBase.h"
#include "../../../General/Collision/Physics.h"
#include "../../../General/Collision/Collidable.h"
#include "../../Attack/AttackBase.h"

CharacterStateBase::CharacterStateBase(std::weak_ptr<Actor> owner):
	m_pOwner(owner),
	m_frame(0.0f)
{
}

CharacterStateBase::~CharacterStateBase()
{
}

void CharacterStateBase::ChangeState(std::shared_ptr<CharacterStateBase> nextState)
{
	//状態変化
	m_nextState = std::move(nextState);
}

void CharacterStateBase::CountFrame()
{
	if (m_pOwner.expired())return;
	//タイムスケールに合わせてカウント
	m_frame += m_pOwner.lock()->GetTimeScale();
}
