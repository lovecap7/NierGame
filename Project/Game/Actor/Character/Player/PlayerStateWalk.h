#pragma once
#include "PlayerStateBase.h"
class Actor;
class PlayerStateWalk :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateWalk>
{
public:
    PlayerStateWalk(std::weak_ptr<Actor>  player);
    ~PlayerStateWalk();
    void Init()override;
    void Update() override;
private:
};

