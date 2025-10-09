#pragma once
#include "PlayerStateBase.h"
class Actor;
class PlayerStateAvoid :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAvoid>
{
public:
    PlayerStateAvoid(std::weak_ptr<Actor>  player);
    ~PlayerStateAvoid();
    void Init()override;
    void Update() override;
private:
    //ˆÚ“®•ûŒü
    Vector3 m_avoidDir;
    //‘¬“x
    float m_speed;
};
