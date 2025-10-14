#pragma once
#include "../../General/Collision/Collidable.h"
#include "../../General/CharaStatus.h"
#include <list>
#include <memory>
#include "../Actor/Actor.h"
class Actor;
class AttackData;
class CharacterBase;
class AttackBase abstract :
    public Collidable
{
public:
    AttackBase(Shape shape, std::shared_ptr<AttackData> attackData, std::weak_ptr<CharacterBase> pOwner);
    ~AttackBase();
    //初期化処理
    virtual void Init()override;
    //更新処理
    virtual void Update();
    //描画
    virtual void Draw()const abstract;
    //衝突イベント
    virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
    //位置確定
    virtual void Complete()override {};
    //終了処理
    virtual void End()override;

    //削除
    bool IsDelete();
    void Delete();
protected:
    //威力
    int m_attackPower;
    //攻撃の重さ
    CharaStatus::AttackWeight m_attackWeight;
    //持続フレーム
    float m_keepFrame;
    //ノックバック垂直
    float m_knockBackV;
    //ノックバック大きさ
    float m_knockBackPower;
    //当てたことのあるActorのIDを覚えておく
    std::list<int> m_hitId;
    //持ち主
    std::weak_ptr<CharacterBase> m_pOwner;
};

