#pragma once
#include <memory>
#include <list>
#include <string>
class Player;
class TalkUI;
class TutorialClearData;
class ActorManager;
class CameraController;
class TutorialManager
{
public:
	TutorialManager(std::weak_ptr<Player> pPlayer,std::shared_ptr<ActorManager> pActorManager,
		std::wstring stageName, std::shared_ptr<CameraController> pCameraController);
	~TutorialManager();
	//更新
	void Update();

	//チュートリアルをクリアしたら
	bool IsClear()const { return m_isClear; };
private:
	//プレイヤー
	std::weak_ptr<Player> m_pPlayer;
	//テキストUI
	std::weak_ptr<TalkUI> m_pTextUI;
	//アクターマネージャー
	std::shared_ptr<ActorManager> m_pActorManager;
	//クリア情報
	std::shared_ptr<TutorialClearData> m_clearData;
	//クリアしたか
	bool m_isClear;
	//カメラ
	std::shared_ptr<CameraController> m_pCameraController;
private:
	
};

