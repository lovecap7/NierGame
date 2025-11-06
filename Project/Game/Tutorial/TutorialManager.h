#pragma once
#include <memory>
#include <list>
#include <string>
class Player;
class TalkUI;
class TutorialData;
class TutorialManager
{
public:
	TutorialManager(std::weak_ptr<Player> pPlayer,std::wstring stageName);
	~TutorialManager();
	//更新
	void Update();

	//チュートリアルをクリアしたら
	bool IsClear()const { return m_isClear; };
private:
	//プレイヤー
	std::weak_ptr<Player> m_pPlayer;
	
	//クリアしたか
	bool m_isClear;
private:
	
};

