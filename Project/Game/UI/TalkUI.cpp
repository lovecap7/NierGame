#include "TalkUI.h"
#include "../../General/Game.h"
#include <DxLib.h>
#include "../../General/AssetManager.h"
#include "../../General/StringUtil.h"
#include "../../General/CSV/TextData.h"
namespace
{
	//背景座標
	constexpr int kBackPosX = Game::kScreenCenterX;
	constexpr int kBackPosY = Game::kScreenHeight - 110;

	//テキスト座標
	constexpr int kTextPosX = Game::kScreenCenterX - 370;
	constexpr int kTextPosY = Game::kScreenHeight - 120;

	//１行に入る文字列の数
	constexpr int kOneLineMaxNum = 60;

	//ハンドル
	const std::wstring kBack = L"Text/TalkUI";

	//デフォルトの速度
	constexpr int kDefTalkSpeed = 10;
}

TalkUI::TalkUI(std::list<std::shared_ptr<TextData>> textDatas):
	m_textDatas(textDatas),
	m_backHandle(-1),
	m_fontHandle(-1),
	m_countFrame(0),
	m_talkSpeed(kDefTalkSpeed),
	m_displayTextNum(0),
	m_text(),
	m_speaker()
{
	auto& assetManager = AssetManager::GetInstance();
	m_backHandle = assetManager.GetImageHandle(kBack);
	m_fontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::NotoSansJP, AssetManager::FontSize::Size24));

	//30文字ごとに区切る
	m_text = StringUtil::InsertNewLines(m_textDatas.front()->GetText(), kOneLineMaxNum);
	//話してる人
	m_speaker = m_textDatas.front()->GetSpeaker();
}

TalkUI::~TalkUI()
{
}

void TalkUI::Update()
{

	++m_countFrame;
	if ((m_countFrame % m_talkSpeed == 0) && (m_countFrame > 0))
	{
		//最後まで行ったら
		if (m_text.size() <= m_displayTextNum)
		{
			//前のデータを削除
			m_textDatas.pop_front();
			//次のデータをチェック
			if (!m_textDatas.empty())
			{
				m_text = StringUtil::InsertNewLines(m_textDatas.front()->GetText(), kOneLineMaxNum);
				m_speaker = m_textDatas.front()->GetSpeaker();
				m_displayTextNum = 0;
				m_countFrame = 0;
			}
			else
			{
				m_isDelete = true;
			}
		}

		++m_displayTextNum;
	}
}

void TalkUI::Draw() const
{
	//背景
	DrawRotaGraph(kBackPosX, kBackPosY, 1.0, 0.0, m_backHandle, true);
	//テキスト
	DrawStringToHandle(kTextPosX, kTextPosY, m_text.substr(0, m_displayTextNum).c_str(), 0xffffff, m_fontHandle);
	//話してる人
	DrawStringToHandle(300 - GetDrawFormatStringWidthToHandle(m_fontHandle, m_speaker.c_str()), kTextPosY - 80, m_speaker.c_str(), 0xffffff, m_fontHandle);
}
