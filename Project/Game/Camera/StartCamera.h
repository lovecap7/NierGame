#pragma once
#include "CameraBase.h"
#include <memory>
class Player;
class StartCamera :
	public CameraBase
{
public:
	StartCamera(std::weak_ptr<Player> pPlayer);
	~StartCamera();
	virtual void Init() override;
	virtual void Update() override;

private:
	//プレイヤーの参照
	std::weak_ptr<Player> m_pPlayer;
	//カメラの回転角度
	float m_rotaAngle;
	//カメラの高さ調整用変数
	float m_heightAdjust;
	//フレームカウント
	int m_countFrame;
};

