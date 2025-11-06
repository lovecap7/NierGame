#pragma once
#include <memory>
#include <list>
class Player;
class TalkUI;
class TutorialManager
{
public:
	TutorialManager(std::weak_ptr<Player> pPlayer);
	~TutorialManager();
	//更新
	void Update();
private:
	//プレイヤー
	std::weak_ptr<Player> m_pPlayer;
	//チュートリアルのUI
	std::list<std::weak_ptr<TalkUI>>m_talkUIs;
private:
	//状態
	using UpdateFunc_t = void(TutorialManager::*)(std::shared_ptr<Player> player);
	UpdateFunc_t m_update;
private:
	//移動
	void UpdateMoveStep(std::shared_ptr<Player> player);
	//ジャンプ
	void UpdateJumpStep(std::shared_ptr<Player> player);
	//攻撃
	void UpdateAttackStep(std::shared_ptr<Player> player);
	//ジャスト回避
	void UpdateJustStep(std::shared_ptr<Player> player);
	//実践
	void UpdateBattleStep(std::shared_ptr<Player> player);
};

