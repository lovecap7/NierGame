#pragma once
#include "../../../../General/Collision/Collidable.h"
class AvoidColl :
    public Collidable
{
public:
	AvoidColl(float radius);
	~AvoidColl();
	void Update(Vector3 nextPos,bool isEnableColl);
	void Draw()const;
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	void Complete() override {};

	//“–‚½‚Á‚½‚©
	bool IsHit()const { return m_isHit; };
private:
	bool m_isHit;
};

