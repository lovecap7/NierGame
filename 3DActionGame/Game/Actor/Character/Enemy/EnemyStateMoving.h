#pragma once
#include "EnemyStateBase.h"
class Actor;
class EnemyBase;
class EnemyStateMoving :
    public EnemyStateBase, public std::enable_shared_from_this<EnemyStateMoving>
{
public:
    EnemyStateMoving(std::weak_ptr<Actor>  enemy);
    ~EnemyStateMoving();
    void Init()override;
    void Update() override;
private:
    //‘¬“x
    float m_speed;
private:
    //ˆÚ“®
    void Move(std::shared_ptr<EnemyBase> owner);
};

