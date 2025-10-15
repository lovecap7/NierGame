#pragma once
#include "PlayerStateBase.h"
#include <memory>
class Actor;
class AttackData;
class SwordAttack;
class PlayerStateHeavyAttack :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateHeavyAttack>
{
public:
    PlayerStateHeavyAttack(std::weak_ptr<Actor>  player);
    ~PlayerStateHeavyAttack();
    void Init()override;
    void Update() override;
private:
    //UŒ‚ƒf[ƒ^
    std::shared_ptr<AttackData> m_attackData;
    //UŒ‚”­¶
    bool m_isAppearedAttack;
    //UŒ‚‚ÌQÆ
    std::weak_ptr<SwordAttack> m_pSwordAttack;
private:
    //UŒ‚íœ
    void DeleteAttack();
};


