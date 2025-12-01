#include "TitleUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CharaStatus.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/MyDraw.h"
#include "../../../General/Game.h"
#include "../../../General/ShaderPostProcess.h"
#include <DxLib.h>

namespace
{
	//ハンドル
	const std::wstring kTitleHandlePath = L"Title/Title";
	const std::wstring kTitleBackHandlePath = L"Title/TitleBack";
}

TitleUI::TitleUI() :
	UIBase(),
	m_titleLogoHandle(-1),
	m_titleBackHandle(-1)
{
	//ハンドル
	m_titleLogoHandle = AssetManager::GetInstance().GetImageHandle(kTitleHandlePath);
	m_titleBackHandle = AssetManager::GetInstance().GetImageHandle(kTitleBackHandlePath);
	//前描画
	m_isFrontDraw = true;

	//シェーダ
	m_shader = std::make_shared<ShaderPostProcess>();
	m_shader->Init();
	m_shader->SetPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
}

TitleUI::~TitleUI()
{
}

void TitleUI::Update()
{
	m_shader->Update();
}

void TitleUI::Draw() const
{
	//タイトル背景
	DrawRotaGraphF(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_titleBackHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	//タイトルロゴ
	m_shader->Draw(Game::kScreenCenterX, Game::kScreenCenterY, m_titleLogoHandle);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
