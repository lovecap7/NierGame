#pragma once
#include <memory>
class Player;
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
};

