#include "CameraController.h"

CameraController::CameraController():
	m_isUpdate(true),
	m_cameras()
{
}

CameraController::~CameraController()
{
}

void CameraController::Init()
{
	m_isUpdate = true;
	m_cameras.clear();
}

void CameraController::Update()
{
	//更新をしないならreturn
	if (!m_isUpdate)return;
	if (m_cameras.empty())return;
	//最後にプッシュ(入れた)カメラのみ更新処理を行う
	m_cameras.back()->Update();
}

void CameraController::ChangeCamera(std::shared_ptr<CameraBase> camera)
{
	//空(最初の処理)
	if (m_cameras.empty())
	{
		m_cameras.emplace_back(camera);
	}
	else
	{
		//先頭のカメラが切り替わる
		m_cameras.back() = camera;
	}
	//初期化処理
	m_cameras.back()->Init();
}

void CameraController::ChangeBaseCamera(std::shared_ptr<CameraBase> camera)
{
	//空の場合エラー
	if (m_cameras.empty())
	{
		m_cameras.emplace_back(camera);
	}
	else
	{
		//先頭のカメラが切り替わる
		m_cameras.front() = camera;
	}
	//初期化処理
	m_cameras.front()->Init();
}

void CameraController::PushCamera(std::shared_ptr<CameraBase> camera)
{
	m_cameras.emplace_back(camera);
	//初期化処理
	m_cameras.back()->Init();
}

void CameraController::PopCamera()
{
	//1つしかないなら抜ける
	if (m_cameras.size() <= 1)return;
	//最後のカメラを取り出す
	m_cameras.pop_back();
}
