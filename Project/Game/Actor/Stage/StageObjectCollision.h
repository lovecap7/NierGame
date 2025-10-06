#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class GameCamera;
class Collidable;
/// <summary>
/// ステージのオブジェクトの当たり判定用のクラス
/// </summary>
class StageObjectCollision :
    public Actor
{
public:
    StageObjectCollision(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);//ポリゴンのコリジョン
    StageObjectCollision(VECTOR pos, float radius);//球のコリジョン
    StageObjectCollision(VECTOR pos1, VECTOR pos2, float radius);//カプセルのコリジョン
    ~StageObjectCollision();
    void Init()override;
    void Update() override;
    void OnCollide(const std::shared_ptr<Collidable> other)override;
    void Draw()const override;
    void Complete() override;
    void End()override;
    //すり抜けるか
    void SetIsThrough(bool isThrough);
private:
    int m_collisionHandle;
};


