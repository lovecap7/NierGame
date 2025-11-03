#include "ResultUI.h"
#include "../../../General/AssetManager.h"
#include "../../../General/Game.h"
#include "../../../General/Math/MyMath.h"
#include "../../../General/Timer.h"
#include "../../../General/CSV/CSVDataLoader.h"
#include "../../../General/CSV/ResultRankData.h"
#include <DxLib.h>
#include <cmath>
#include <sstream>
#include <iomanip>
namespace
{
    //テキストカラー
    constexpr int kTextColor = 0xe1e1df;

    //Alpha速度
    constexpr int kAlphaSpeed = 5;

    //リザルトの文字
    constexpr int kResultTextPosX = Game::kScreenCenterX;
    constexpr int kResultTextPosY = 150;
    
    //リザルトの情報
    constexpr int kResultInfoPosX = Game::kScreenCenterX;
    constexpr int kResultInfoPosY = Game::kScreenCenterY;
    //タイム
    constexpr int kTimePosX = Game::kScreenCenterX + 90;
    constexpr int kTimePosY = Game::kScreenCenterY - 55;
    //桁数
    constexpr int kDigitsNum = 2;

    //ランク
    constexpr int kRankPosX = Game::kScreenCenterX + 150;
    constexpr int kRankPosY = Game::kScreenCenterY + 20;

    //PressAnyBotton
    constexpr int kPressAnyBottonPosX = Game::kScreenCenterX;
    constexpr int kPressAnyBottonPosY = Game::kScreenHeight - 150.0f;

    //30分を最大とする
    constexpr int kTimeMaxHalfAnHour = 30;

    //リザルトの情報を表示するフレーム
    constexpr int kDisplayResultInfoFrame = 60;

    //透明度の最高
    constexpr int kAlphaMax = 255;

    //リザルトランクのパス
    const std::wstring kResultRankPath = L"/ResultRankData";
}

ResultUI::ResultUI(std::wstring stageName, std::shared_ptr<Timer> timer):
    m_resultHandle(-1),
    m_backHandle(-1),
    m_resultInfoHandle(-1),
    m_fontHandle(-1),
    m_countFrame(0),
    m_timer(timer),
    m_rank(L"E")
{
    auto& assetManager = AssetManager::GetInstance();
    m_resultHandle.handle = assetManager.GetImageHandle(L"Result/Result");
    m_resultInfoHandle.handle = assetManager.GetImageHandle(L"Result/ResultInfo");
    m_pressAnyBottonHandle.handle = assetManager.GetImageHandle(L"Result/PressAnyBotton");
    m_backHandle = assetManager.GetImageHandle(L"Back/BlackBack2");
    m_fontHandle = assetManager.GetFontHandle(AssetManager::Font(AssetManager::FontType::Roboto, AssetManager::FontSize::Size36));

    //ランクを計算
    int min = m_timer->GetMinutes();
    int sec = m_timer->GetSeconds();
    //クリアタイム
    float clearTime = min * 60 + sec;
   
    auto& csvLoader = CSVDataLoader::GetInstance();
    std::wstring path = stageName + kResultRankPath;
    auto datas = csvLoader.LoadCSV(path.c_str());
    for (auto data : datas)
    {
        if (!data)continue;
        auto resultRank = std::make_shared<ResultRankData>(data);

        //ランク条件タイム
        float rankTime = resultRank->GetMin() * 60 + resultRank->GetSec();

        //タイマーが条件を満たすタイムなら
        if (clearTime <= rankTime)
        {
            m_rank = resultRank->GetRank();
            break;
        }
    }
}

ResultUI::~ResultUI()
{
}

void ResultUI::Update()
{
    ++m_countFrame;
    m_resultHandle.alpha += kAlphaSpeed;
    if (m_countFrame >= kDisplayResultInfoFrame)
    {
        m_resultInfoHandle.alpha += kAlphaSpeed;
    }
    if (m_resultInfoHandle.alpha >= kAlphaMax)
    {
        m_pressAnyBottonHandle.alpha = (std::cos(m_countFrame * kAlphaSpeed * MyMath::DEG_2_RAD) * kAlphaMax);
    }
}

void ResultUI::Draw() const
{
    //背景
    DrawRotaGraph(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_backHandle, true);

    //リザルトの文字
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_resultHandle.alpha);
    DrawRotaGraph(kResultTextPosX, kResultTextPosY, 1.0, 0.0, m_resultHandle.handle, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //リザルトの情報
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_resultInfoHandle.alpha);
    DrawRotaGraph(kResultInfoPosX, kResultInfoPosY, 1.0, 0.0, m_resultInfoHandle.handle, true);

    //タイム
    int mins    = m_timer->GetMillisecond();
    int sec     = m_timer->GetSeconds();
    int min     = m_timer->GetMinutes();
    //30分を超えたら
    if (min >= kTimeMaxHalfAnHour)
    {
        //30分を上限とする
        min     = kTimeMaxHalfAnHour;
        sec     = 0;
        mins    = 0;
    }

    std::wstringstream ss;
    //0埋め
    ss << std::setfill(L'0')
        << std::setw(kDigitsNum) << min << L":"
        << std::setw(kDigitsNum) << sec << L":"
        << std::setw(kDigitsNum) << mins;
    std::wstring time = ss.str();
    DrawStringToHandle(kTimePosX, kTimePosY, time.c_str(), kTextColor, m_fontHandle);

    //ランク
    DrawStringToHandle(kRankPosX, kRankPosY, m_rank.c_str(), kTextColor, m_fontHandle);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    //PressAnyBottonHandle
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pressAnyBottonHandle.alpha);
    DrawRotaGraph(kPressAnyBottonPosX, kPressAnyBottonPosY, 1.0, 0.0, m_pressAnyBottonHandle.handle, true);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
