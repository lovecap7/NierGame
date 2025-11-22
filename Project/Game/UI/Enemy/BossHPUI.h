#pragma once
#include "../HPUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class CharaStatus;
class EnemyBase;
class BossHPUI :
	public HPUIBase
{
public:
	BossHPUI(std::shared_ptr<CharaStatus> charaStatus, std::weak_ptr<EnemyBase> pEnemy);
	~BossHPUI();
	//XV
	void Update() override;
	//•`‰æ
	void Draw()const override;
private:
	//‚¿å
	std::weak_ptr<EnemyBase> m_pEnemy;
};

