#pragma once
#include "EnemyStateBase.h"
class Actor;
class EnemyStateHit :
	public EnemyStateBase, public std::enable_shared_from_this<EnemyStateHit>
{
public:
	EnemyStateHit(std::weak_ptr<Actor> enemy);
	~EnemyStateHit();
	void Init()override;
	void Update() override;
};

