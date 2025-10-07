#pragma once
#include "../Actor.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/Collision/Rigidbody.h"
class DebugEnemy :
    public Actor
{
public:
	DebugEnemy();
	~DebugEnemy();
	virtual void Init() override;
	virtual void Update() override;
	virtual void Draw() const override;
	virtual void End() override;
	void OnCollide(const std::shared_ptr<Collidable> other) override;
	void Complete();
	void SetPos(const Vector3& pos) { m_rb->m_pos = pos; }
private:
};

