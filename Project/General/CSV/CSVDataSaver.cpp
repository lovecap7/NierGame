#include "CSVDataSaver.h"
#include "../StringUtil.h"
#include "../Sound/SoundManager.h"
#include "../../Main/Application.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cassert>

namespace
{
    //パス
    const std::string kOptionPath = "Data/CSV/Save/OptionSaveData.csv";

    //オプションヘッダー
    const std::string kOptionSaveHeader = "BGM,SE,Voice,ScreenMode\n";
}

void CSVDataSaver::OptionSave()
{
    //ファイルを開く(ない場合は作成される)
    std::ofstream file(kOptionPath);
    //ファイルが読み込めなかったら
    if (!file)
    {
        assert("ファイルの読み込みに失敗");
    }

    //ヘッダーを書き込む
    file << kOptionSaveHeader;

    //サウンド
    auto& soundManager = SoundManager::GetInstance();

    //各サウンドレベル
    int bgmLv = soundManager.GetLevelToVolume(soundManager.GetBGMVolume());
    int seLv = soundManager.GetLevelToVolume(soundManager.GetSEVolume());
    int voiceLv = soundManager.GetLevelToVolume(soundManager.GetVoiceVolume());

    //アプリケーション
    auto& app = Application::GetInstance();

    //スクリーンモード
    int isWindow = static_cast<int>(app.IsWindowMode());

    //データ書き込み
    file << bgmLv << ","
        << seLv << ","
        << voiceLv << ","
        << isWindow <<
        std::endl;
  
    //閉じる
    file.close();
}
