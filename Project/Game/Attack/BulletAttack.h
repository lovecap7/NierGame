#pragma once
#include "SphereAttackBase.h"
#include <memory>
class AttackData;
class Collidable;
class CharacterBase;
class BulletAttack :
    public SphereAttackBase
{
public:
    BulletAttack(std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner);
    ~BulletAttack();
    //XV
    void Update() override;
    //•`‰æ
    void Draw()const override;
    //“–‚½‚Á‚½
    void OnCollide(const std::shared_ptr<Collidable> other) override;

    //ˆÚ“®•ûŒü
    void SetMoveVec(Vector3 vec) { m_moveVec = vec; };
private:
    Vector3 m_moveVec;
};

