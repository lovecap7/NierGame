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
    //攻撃データ
    std::shared_ptr<AttackData> m_attackData;
    //攻撃発生
    bool m_isAppearedAttack;
    //攻撃の参照
    std::weak_ptr<SwordAttack> m_pSwordAttack;
    //チャージフレーム
    float m_chargeCountFrame;
private:
    //攻撃削除
    void DeleteAttack();
};


