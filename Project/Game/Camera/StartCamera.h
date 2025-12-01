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
	void Init() override;
	void Update() override;
	void Draw() const override;

private:
	//プレイヤーの参照
	std::weak_ptr<Player> m_pPlayer;
	//カメラの回転角度
	float m_rotaAngle;
	//カメラの高さ調整用変数
	float m_heightAdjust;
	//フレームカウント
	int m_countFrame;
	//シネマスコープ
	int m_cinemaScopeHandle;
	float m_cinemaScopeTopPosY;
	float m_cinemaScopeUnderPosY;
};

