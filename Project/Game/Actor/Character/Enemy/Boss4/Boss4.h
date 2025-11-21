#pragma once
#include "../EnemyBase.h"
#include <memory>
class Boss4 :
	public EnemyBase
{
public:
	Boss4(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~Boss4();
	//初期化処理
	void Init()override;
	//更新処理による結果の確定
	void Complete() override;
	//終了処理
	void End()override;
private:
};

