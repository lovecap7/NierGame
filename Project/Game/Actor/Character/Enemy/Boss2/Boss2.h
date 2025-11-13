#pragma once
#include "../EnemyBase.h"
#include <memory>
class Boss2 :
	public EnemyBase
{
public:
	Boss2(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~Boss2();
	//初期化処理
	void Init()override;
	//更新処理による結果の確定
	void Complete() override;
	//終了処理
	void End()override;
private:
	//ランダムに攻撃を取得
	std::shared_ptr<AttackData> GetRandomAttack(std::vector<std::wstring> keys) const override;
};

