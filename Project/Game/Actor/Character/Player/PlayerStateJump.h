#pragma once
#include "PlayerStateBase.h"
class Actor;
class PlayerStateJump :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateJump>
{
public:
    PlayerStateJump(std::weak_ptr<Actor> player);
    ~PlayerStateJump();
    void Init()override;
    void Update() override;
private:
};

