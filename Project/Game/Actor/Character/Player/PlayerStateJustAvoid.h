#pragma once
#include "PlayerStateBase.h"
class Actor;
class PlayerStateJustAvoid :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateJustAvoid>
{
public:
    PlayerStateJustAvoid(std::weak_ptr<Actor>  player);
    ~PlayerStateJustAvoid();
    void Init()override;
    void Update() override;
};
