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
    //移動か待機か
    void ChangeToMoveOrIdle(std::shared_ptr<Player> owner, Input& input);
private:
    //ジャスト回避
    bool m_isJust;
};


