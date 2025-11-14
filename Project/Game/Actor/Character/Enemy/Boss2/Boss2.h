#pragma once
#include "../EnemyBase.h"
#include <memory>
class Boss2Arm;
class CharaStatusData;
class Boss2 :
	public EnemyBase
{
public:
	Boss2(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~Boss2();
	//初期化処理
	void Init()override;
	//更新処理
	void Update() override;
	//更新処理による結果の確定
	void Complete() override;
	//終了処理
	void End()override;
private:
	//アーム初期化
	void InitArm(CSVDataLoader& csvLoader, std::vector<std::wstring>& pathData);
private:
	//腕
	std::weak_ptr<Boss2Arm> m_leftArm;
	std::weak_ptr<Boss2Arm> m_rightArm;
	std::shared_ptr<CharaStatusData> m_charaStatusData;
};

