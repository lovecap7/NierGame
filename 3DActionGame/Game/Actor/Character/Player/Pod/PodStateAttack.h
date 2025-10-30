#pragma once
#include "PodStateBase.h"
class Actor;
class AttackData;
class PodStateAttack :
    public PodStateBase, public std::enable_shared_from_this<PodStateAttack>
{
public:
    PodStateAttack(std::weak_ptr<Actor>  pod);
    ~PodStateAttack();
    void Init()override;
    void Update() override;
private:
    //UŒ‚ƒf[ƒ^
    std::shared_ptr<AttackData> m_attackData;
};

