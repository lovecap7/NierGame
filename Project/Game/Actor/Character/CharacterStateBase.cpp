#include "CharacterStateBase.h"
#include "../../../General/Collision/Physics.h"

CharacterStateBase::CharacterStateBase(std::weak_ptr<Actor> owner):
	m_owner(owner)
{
}

CharacterStateBase::~CharacterStateBase()
{
}

void CharacterStateBase::ChangeState(std::shared_ptr<CharacterStateBase> nextState)
{
	//ó‘Ô•Ï‰»
	m_nextState = move(nextState);
}