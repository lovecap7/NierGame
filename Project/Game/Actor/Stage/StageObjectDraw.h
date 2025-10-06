#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class GameCamera;
class AttackManager;
class Model;
/// <summary>
/// ステージのオブジェクトを描画する用のクラス
/// </summary>
class StageObjectDraw :
    public Actor
{
public:
    StageObjectDraw(int modelHandle, VECTOR pos, VECTOR scale, VECTOR angle);
    ~StageObjectDraw();
    void Init()override {};
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override ;
    void Update()override;
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override;
    void Complete() override {};
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    void End()override;
private:
};

