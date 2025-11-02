#pragma once
#include "../HPUIBase.h"
#include "../../../General/Math/MyMath.h"
#include <memory>
class CharaStatus;
class EnemyBase;
class NormalEnemyHPUI :
	public HPUIBase
{
public:
	NormalEnemyHPUI(std::shared_ptr<CharaStatus> charaStatus, std::weak_ptr<EnemyBase> pEnemy);
	~NormalEnemyHPUI();
	//XV
	void Update() override;
	//•`‰æ
	void Draw()const override;
private:
	//‚¿å
	std::weak_ptr<EnemyBase> m_pEnemy;
	//“G‚ÌÀ•W
	Vector3 m_enemyViewPos;
	//ƒJƒƒ‰‚É‰f‚Á‚Ä‚¢‚é‚©
	bool m_isInCameraView;
};

