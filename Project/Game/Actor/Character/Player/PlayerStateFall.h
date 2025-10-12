#pragma once
#include "PlayerStateBase.h"
class Actor;
class CharaStatus;
class Player;
class PlayerStateFall :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateFall>
{
public:
    PlayerStateFall(std::weak_ptr<Actor>  player);
    ~PlayerStateFall();
    void Init()override;
    void Update() override;
private:
    //—Ž‰º’†‚ÌˆÚ“®
    void MoveFall(Input& input, std::shared_ptr<Player> owner, std::shared_ptr<CharaStatus> status);
};

