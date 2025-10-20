#pragma once
#include "PlayerStateAttackBase.h"
#include <memory>
class Actor;
class AttackData;
class SwordAttack;
class PlayerStateLightAttack :
    public PlayerStateAttackBase, public std::enable_shared_from_this<PlayerStateLightAttack>
{
public:
    PlayerStateLightAttack(std::weak_ptr<Actor> player, bool isJump = false, bool isJust = false);
    ~PlayerStateLightAttack();
    void Init()override;
    void Update() override;
private:
    //ˆÚ“®‚©‘Ò‹@‚©
    void ChangeToMoveOrIdle(std::shared_ptr<Player> owner, Input& input);
private:
  
};


