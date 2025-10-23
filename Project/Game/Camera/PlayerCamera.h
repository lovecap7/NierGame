#pragma once
#include "CameraBase.h"
#include <memory>
#include <list>
class Actor;
class ActorManager;
class EnemyBase;
class PlayerCamera :
    public CameraBase
{
public:
	PlayerCamera();
	~PlayerCamera();
	virtual void Init() override;
	virtual void Update() override;
	//ロックオン開始
	void StartLockOn(std::weak_ptr<Actor> lockOnTarget);
	//ロックオン終了
	void EndLockOn();
	//ロックオン中か
	bool IsLockOn() const { return m_isLockOn; }
	//プレイヤーの座標
	void SetPlayerPos(const Vector3& pos) { m_playerPos = pos; }
	//プレイヤーのベクトル
	void SetPlayerVec(const Vector3& vec) { m_playerVec = vec; }
	//ターゲット探索
	void SearchTarget(std::shared_ptr<ActorManager> actorM, const std::list<std::shared_ptr<EnemyBase>>& enemys);
private:
	//ロックオン対象
	std::weak_ptr<Actor> m_lockOnTarget;
	//ロックオン中か
	bool m_isLockOn;
	//プレイヤーの座標
	Vector3 m_playerPos;
	//プレイヤーのベクトル
	Vector3 m_playerVec;
	//Lerp率
	float m_lerpRate;
private:
	//カメラの向きを更新
	void UpdateCameraDirection(const Vector3& targetDir, float followSpeed);
};

