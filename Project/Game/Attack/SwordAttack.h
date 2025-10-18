#pragma once
#include "CapsuleAttackBase.h"
#include <memory>
class AttackData;
class Actor;
class CharacterBase;
class SwordAttack :
    public CapsuleAttackBase
{
public:
    SwordAttack(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner);
    ~SwordAttack();
    //•`‰æ
    void Draw()const override;
    //“–‚½‚Á‚½Žž
    void OnCollide(const std::shared_ptr<Collidable> other) override;
};

