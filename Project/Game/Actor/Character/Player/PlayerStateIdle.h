#pragma once
#include "PlayerStateBase.h"
class Actor;
class PlayerStateIdle :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateIdle>
{
public:
    PlayerStateIdle(std::weak_ptr<Actor>  player);
    ~PlayerStateIdle();
    void Init()override;
    void Update() override;
private:
};

