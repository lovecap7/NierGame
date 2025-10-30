#include "EnemyStateBase.h"

EnemyStateBase::EnemyStateBase(std::weak_ptr<Actor> enemy):
	CharacterStateBase(enemy)
{
}

EnemyStateBase::~EnemyStateBase()
{
}
