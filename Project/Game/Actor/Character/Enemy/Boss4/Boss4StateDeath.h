#pragma once
#include "../EnemyStateBase.h"
class Actor;
class Boss4StateDeath :
	public EnemyStateBase, public std::enable_shared_from_this<Boss4StateDeath>
{
public:
	Boss4StateDeath(std::weak_ptr<Actor> enemy);
	~Boss4StateDeath();
	void Init()override;
	void Update() override;
};


