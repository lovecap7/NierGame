#pragma once
#include<memory>
#include<list>
#include "CameraBase.h"
#include "../../General/Math/MyMath.h"
class CameraController
{
private:
	//シングルトンの準備
	CameraController() = default;
	~CameraController() = default;
	//コピー禁止
	CameraController(const CameraController&) = delete;
	CameraController& operator = (const CameraController&) = delete;
	//ムーブ禁止
	CameraController(CameraController&&) = delete;
	CameraController& operator = (CameraController&&) = delete;
public:
	//インスタンスを取得
	static CameraController& GetInstance()
	{
		static CameraController instance;
		return instance;
	}
	void Init();
	void Update();
	/// <summary>
	/// カメラを変更する
	/// </summary>
	/// <param name="">次の状態のポインタ</param>
	void ChangeCamera(std::shared_ptr<CameraBase> camera);
	/// <summary>
	/// 最初のカメラを切り替え
	/// <param name=""></param>
	void ChangeBaseCamera(std::shared_ptr<CameraBase> camera);
	/// <summary>
	/// カメラをプッシュ(入れる)する
	/// </summary>
	/// <param name=""></param>
	void PushCamera(std::shared_ptr<CameraBase> camera);
	/// <summary>
	/// カメラをポップ(取り出す)する
	/// </summary>
	/// <param name=""></param>
	void PopCamera();
	/// <summary>
	/// 更新する
	/// </summary>
	void StartUpdate() { m_isUpdate = true; };
	/// <summary>
	/// 更新を止める
	/// </summary>
	void StopUpdate() { m_isUpdate = false; };
	//現在のカメラを取得
	Vector3 GetCameraPos() const 
	{
		if (m_cameras.empty())return Vector3::Zero();
		return m_cameras.back()->GetPos();
	};
	Vector3 GetCameraLook() const
	{
		if (m_cameras.empty())return Vector3::Zero();
		return m_cameras.back()->GetLook();
	};
	Quaternion GetCameraRot() const
	{
		if (m_cameras.empty())return Quaternion::IdentityQ();
		return m_cameras.back()->GetRotH();
	};
private:
	std::list<std::shared_ptr<CameraBase>> m_cameras;
	//シーンの更新
	bool m_isUpdate;
};

