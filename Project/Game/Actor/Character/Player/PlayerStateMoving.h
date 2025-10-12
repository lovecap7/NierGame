#pragma once
#include "PlayerStateBase.h"
class Actor;
class Player;
class Input;
class PlayerStateMoving :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateMoving>
{
public:
    PlayerStateMoving(std::weak_ptr<Actor>  player,bool isDash);
    ~PlayerStateMoving();
    void Init()override;
    void Update() override;
private:
    //‘¬“x
    float m_speed;
    //ˆÚ“®
    void Move(std::shared_ptr<Player> owner, Input& input);
};

