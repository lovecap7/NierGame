#include "SelectUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/CSV/CSVDataLoader.h"
#include "../../../General/CSV/MissionBoardUIData.h"
#include "../../../General/Fader.h"
#include "MissionBoardUI.h"
#include <DxLib.h>

namespace
{
	//メインメニューの位置
	constexpr float kMainMenuStartX = 300.0f;
	constexpr float kMainMenuOffsetX = -600.0f;
	constexpr float kMainMenuStartY = 110.0f;
	constexpr float kMainMenuOffsetY = 110.0f;
	constexpr float kMainMenuLerpRate = 0.3f;
	constexpr float kMainMenuSecondX = -130.0f;

	//チュートリアルメニューの位置
	constexpr float kTutorialMenuStartX = 320.0f;
	constexpr float kTutorialMenuStartY = 110.0f;
	constexpr float kTutorialMenuOffsetY = 110.0f;
	constexpr float kTutorialMenuLerpRate = 0.3f;

	//ステージメニューの位置
	constexpr float kStageMenuStartX = 320.0f;
	constexpr float kStageMenuStartY = 110.0f;
	constexpr float kStageMenuOffsetY = 110.0f;
	constexpr float kStageMenuLerpRate = 0.3f;

	//影
	const Vector2 kShadowOffsetPos = { 10.0f,10.0f };

	//選択中にずらす量
	constexpr float kSelectOffsetX = 50.0f;

	//サイズ
	constexpr int kMainSize = 4;
	constexpr int kTutorialSize = 3;
	constexpr int kStageSelectSize = 3;

	//パス
	const std::wstring kTutorialPath = L"Select/Tutorial";
	const std::wstring kTutorial1Path = L"Select/Tutorial1";
	const std::wstring kTutorial2Path = L"Select/Tutorial2";
	const std::wstring kTutorial3Path = L"Select/Tutorial3";
	const std::wstring kSelectMissionPath = L"Select/SelectMission";
	const std::wstring kSelectMission1Path = L"Select/SelectMission1";
	const std::wstring kSelectMission2Path = L"Select/SelectMission2";
	const std::wstring kSelectMission3Path = L"Select/SelectMission3";
	const std::wstring kStageMissionBoardUIDataPath = L"Select/StageMissionBoardUIData";
	const std::wstring kOptionPath = L"Select/Option";
	const std::wstring kBackTitlePath = L"Select/BackTitle";
	const std::wstring kCursorPath = L"Mark/Cursor";

	//カーソル
	constexpr float kCursorOffsetPosX = -220.0f;
	constexpr float kCursorRange = 10.0f;
	constexpr float kCursorAngleSpeed = 5.0f;
	constexpr float kCursorLerpRate = 0.3f;
}

SelectUI::SelectUI() :
	UIBase(),
	m_selectMainMenuIndex(0),
	m_selectTutorialMenuIndex(0),
	m_selectStageMenuIndex(0),
	m_update(&SelectUI::UpdateMainMenu),
	m_cursor(),
	m_cursorAngle(0.0f)
{
	auto& assetManager = AssetManager::GetInstance();

	//メインメニュー
	m_mainMenus.resize(kMainSize);
	for (int i = 0; i < kMainSize; ++i)
	{
		m_mainMenus[i].pos = Vector2(kMainMenuStartX + kMainMenuOffsetX * (i + 1), kMainMenuStartY + i * kMainMenuOffsetY);
	}
	m_mainMenus[0].handle = assetManager.GetImageHandle(kTutorialPath);
	m_mainMenus[1].handle = assetManager.GetImageHandle(kSelectMissionPath);
	m_mainMenus[2].handle = assetManager.GetImageHandle(kOptionPath);
	m_mainMenus[3].handle = assetManager.GetImageHandle(kBackTitlePath);

	//チュートリアルメニュー
	m_tutorialMenus.resize(kTutorialSize);
	for (int i = 0; i < kTutorialSize; ++i)
	{
		m_tutorialMenus[i].pos = Vector2(kMainMenuStartX, kMainMenuStartY);
	}
	m_tutorialMenus[0].handle = assetManager.GetImageHandle(kTutorial1Path);
	m_tutorialMenus[1].handle = assetManager.GetImageHandle(kTutorial2Path);
	m_tutorialMenus[2].handle = assetManager.GetImageHandle(kTutorial3Path);

	//ステージメニュー
	m_stageMenus.resize(kStageSelectSize);
	for (int i = 0; i < kStageSelectSize; ++i)
	{
		m_stageMenus[i].pos = Vector2(kMainMenuStartX, kMainMenuStartY + kMainMenuOffsetY);
	}
	m_stageMenus[0].handle = assetManager.GetImageHandle(kSelectMission1Path);
	m_stageMenus[1].handle = assetManager.GetImageHandle(kSelectMission2Path);
	m_stageMenus[2].handle = assetManager.GetImageHandle(kSelectMission3Path);

	//ミッションボード
	auto& csvLoader = CSVDataLoader::GetInstance();
	std::vector<std::shared_ptr<MissionBoardUIData>> missionBoardUIData;
	for(auto& data : csvLoader.LoadCSV(kStageMissionBoardUIDataPath.c_str()))
	{
		auto mbData = std::make_shared<MissionBoardUIData>(data);
		missionBoardUIData.push_back(mbData);
	}
	auto mb = std::make_shared<MissionBoardUI>(kStageSelectSize, missionBoardUIData);
	mb->Init();
	m_missionBoard = mb;

	//カーソル
	m_cursor.handle = assetManager.GetImageHandle(kCursorPath);
	m_cursor.pos = m_mainMenus[0].pos;
	m_cursor.pos.x += kCursorOffsetPosX;
}

SelectUI::~SelectUI()
{
}

void SelectUI::Update()
{
	m_cursorAngle += kCursorAngleSpeed;
	m_cursorAngle = static_cast<int>(m_cursorAngle) % 360;
	(this->*m_update)();
}

void SelectUI::Draw() const
{
	auto& fader = Fader::GetInstance();

	if (m_update == &SelectUI::UpdateTutorialMenu)
	{
		//チュートリアルメニュー描画
		DrawMenu(fader, m_tutorialMenus, m_selectTutorialMenuIndex, m_update == &SelectUI::UpdateTutorialMenu);
	}

	if (m_update == &SelectUI::UpdateStageMenu)
	{
		//ステージメニュー描画
		DrawMenu(fader, m_stageMenus, m_selectStageMenuIndex, m_update == &SelectUI::UpdateStageMenu);
	}

	//メインメニュー描画
	DrawMenu(fader, m_mainMenus, m_selectMainMenuIndex, m_update == &SelectUI::UpdateMainMenu);

	//カーソル
	float cursorMoveX = kCursorRange * cosf(m_cursorAngle * MyMath::DEG_2_RAD);
	SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
	DrawRotaGraphF(m_cursor.pos.x + kShadowOffsetPos.x + cursorMoveX, m_cursor.pos.y + kShadowOffsetPos.y, 1.0, 0.0, m_cursor.handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	DrawRotaGraphF(m_cursor.pos.x + cursorMoveX, m_cursor.pos.y, 1.0, 0.0, m_cursor.handle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void SelectUI::SetSelectMainMenuIndex(int index)
{
	m_selectMainMenuIndex = MathSub::ClampInt(index, 0, kMainSize);
	m_update = &SelectUI::UpdateMainMenu;
}

void SelectUI::SetSelectTutorialMenuIndex(int index)
{
	m_selectTutorialMenuIndex = index;
	m_update = &SelectUI::UpdateTutorialMenu;
}

void SelectUI::SetSelectStageMenuIndex(int index)
{
	if(!m_missionBoard.expired())
	{
		m_missionBoard.lock()->SetSelectMissionBoardIndex(index);
	}
	m_selectStageMenuIndex = index;
	m_update = &SelectUI::UpdateStageMenu;
}

void SelectUI::UpdateMainMenu()
{
	auto& fader = Fader::GetInstance();
	//メインメニュー
	for (int i = 0; i < m_mainMenus.size(); ++i)
	{
		float targetX = kMainMenuStartX;
		//選んでいるメニューなら右に動かす
		if (m_selectMainMenuIndex == i && (!fader.IsFadeNow() || (fader.IsFadeOutNow())))
		{
			targetX += kSelectOffsetX;

			//カーソルを動かす
			Vector2 cursorNextPos = m_mainMenus[i].pos;
			cursorNextPos.x += kCursorOffsetPosX;
			m_cursor.pos = Vector2::Lerp(m_cursor.pos, cursorNextPos, kCursorLerpRate);
		}
		m_mainMenus[i].pos.x = MathSub::Lerp(m_mainMenus[i].pos.x, targetX, kMainMenuLerpRate);
	}
	//チュートリアル
	for (int i = 0; i < m_tutorialMenus.size(); ++i)
	{
		if (m_mainMenus.empty())
		{
			break;
		}
		m_tutorialMenus[i].pos = m_mainMenus[0].pos;
	}
	//ステージ
	for (int i = 0; i < m_tutorialMenus.size(); ++i)
	{
		if (m_mainMenus.empty())
		{
			break;
		}
		m_stageMenus[i].pos = m_mainMenus[1].pos;
	}

	//非表示
	if (m_missionBoard.expired())return;
	m_missionBoard.lock()->DisableDraw();
}

void SelectUI::UpdateTutorialMenu()
{
	for (int i = 0; i < m_tutorialMenus.size(); ++i)
	{
		float targetX = kTutorialMenuStartX;
		float targetY = kTutorialMenuStartY + kTutorialMenuOffsetY * i;
		//選んでいるメニューなら右に動かす
		if (m_selectTutorialMenuIndex == i)
		{
			targetX += kSelectOffsetX;

			//カーソルを動かす
			Vector2 cursorNextPos = m_tutorialMenus[i].pos;
			cursorNextPos.x += kCursorOffsetPosX;
			m_cursor.pos = Vector2::Lerp(m_cursor.pos, cursorNextPos, kCursorLerpRate);
		}
		m_tutorialMenus[i].pos.x = MathSub::Lerp(m_tutorialMenus[i].pos.x, targetX, kTutorialMenuLerpRate);
		m_tutorialMenus[i].pos.y = MathSub::Lerp(m_tutorialMenus[i].pos.y, targetY, kTutorialMenuLerpRate);
	}
	//メインメニュー
	for (int i = 0; i < m_mainMenus.size(); ++i)
	{
		float targetX = kMainMenuSecondX;
		//選んでいるメニューなら右に動かす
		if (m_selectMainMenuIndex == i)
		{
			targetX += kSelectOffsetX;
		}
		m_mainMenus[i].pos.x = MathSub::Lerp(m_mainMenus[i].pos.x, targetX, kMainMenuLerpRate);
	}

	//非表示
	if (m_missionBoard.expired())return;
	m_missionBoard.lock()->DisableDraw();
}

void SelectUI::UpdateStageMenu()
{
	for (int i = 0; i < m_stageMenus.size(); ++i)
	{
		float targetX = kStageMenuStartX;
		float targetY = kStageMenuStartY + kStageMenuOffsetY * i;
		//選んでいるメニューなら右に動かす
		if (m_selectStageMenuIndex == i)
		{
			targetX += kSelectOffsetX;

			//カーソルを動かす
			Vector2 cursorNextPos = m_stageMenus[i].pos;
			cursorNextPos.x += kCursorOffsetPosX;
			m_cursor.pos = Vector2::Lerp(m_cursor.pos, cursorNextPos, kCursorLerpRate);
		}
		m_stageMenus[i].pos.x = MathSub::Lerp(m_stageMenus[i].pos.x, targetX, kStageMenuLerpRate);
		m_stageMenus[i].pos.y = MathSub::Lerp(m_stageMenus[i].pos.y, targetY, kStageMenuLerpRate);
	}
	//メインメニュー
	for (int i = 0; i < m_mainMenus.size(); ++i)
	{
		float targetX = kMainMenuSecondX;
		//選んでいるメニューなら右に動かす
		if (m_selectMainMenuIndex == i)
		{
			targetX += kSelectOffsetX;
		}
		m_mainMenus[i].pos.x = MathSub::Lerp(m_mainMenus[i].pos.x, targetX, kMainMenuLerpRate);
	}

	//表示
	if (m_missionBoard.expired())return;
	m_missionBoard.lock()->EnableDraw();
}

void SelectUI::DrawMenu(Fader& fader, const std::vector<MenuUI>& menus, int selectMenuIndex, bool isMenuMode) const
{
	for (int i = 0; i < menus.size(); ++i)
	{
		SetDrawBlendMode(DX_BLENDMODE_SUB, 255);
		DrawRotaGraph(menus[i].pos.x + kShadowOffsetPos.x, menus[i].pos.y + kShadowOffsetPos.y, 1.0, 0.0, menus[i].handle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

		//選んでいるメニューなら反転
		if (selectMenuIndex == i &&
			(!fader.IsFadeNow() || (fader.IsFadeOutNow())) &&
			isMenuMode)
		{
			SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
		}
		DrawRotaGraph(menus[i].pos.x, menus[i].pos.y, 1.0, 0.0, menus[i].handle, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

