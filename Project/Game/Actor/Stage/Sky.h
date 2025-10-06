#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include <DxLib.h>
class ActorManager;
class GameCamera;
class AttackManager;
class Model;
class Sky :
    public Actor
{
public:
	Sky(int handle);
	~Sky();
    void Init()override;
    void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
    void Update() override;
    void OnCollide(const std::shared_ptr<Collidable> other)override {};
    void Draw()const override;
    void Complete() override {};
    void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override {};
    void End()override;
private:
	//ƒ‚ƒfƒ‹
	std::shared_ptr<Model> m_model;
};

