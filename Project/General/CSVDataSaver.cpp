#include "CSVDataSaver.h"
#include "StringUtil.h"
#include <iostream>
#include <fstream>
#include <list>
#include <cassert>
#include "../Game/GameRule/Score.h"

CSVDataSaver::CSVDataSaver()
{
}

CSVDataSaver::~CSVDataSaver()
{
}

void CSVDataSaver::SaveScoreDataToCSV(std::shared_ptr<Score> score)
{
    //ファイルを開く(ない場合は作成される)
    std::ofstream file(kHighScoreDataPath);
    //ファイルが読み込めなかったら
    if (!file)
    {
        assert("ファイルの読み込みに失敗");
    }
    
    //ヘッダーを書き込む
    file << "1位,2位,3位,Stage\n";
    //各ステージのランキングを記録
    for (int i = static_cast<int>(Stage::StageIndex::Stage1);i < static_cast<int>(Stage::StageIndex::StageNum);++i)
    {
        auto highScore = score->GetHighScore(static_cast<Stage::StageIndex>(i));
        //データ書き込み
        file << highScore[0] << ","
             << highScore[1] << ","
             << highScore[2] << ","
             << "Stage" << i + 1 << std::endl;
    }
    //閉じる
    file.close();
}

void CSVDataSaver::SaveDataToCSV(SaveDatas saveDatas)
{
    //ファイルを開く(ない場合は作成される)
    std::ofstream file(kSaveDataPath);
    //ファイルが読み込めなかったら
    if (!file)
    {
        assert("ファイルの読み込みに失敗");
    }

    //ヘッダーを書き込む
    file << "ステージ1のクリア,ステージ2のクリア,ステージ3のクリア,総プレイ時間,日付と時間\n";

    //データ書き込み
    file << static_cast<int>(saveDatas.stage1Clear) << ","//ステージ1のクリア
         << static_cast<int>(saveDatas.stage2Clear) << ","//ステージ2のクリア
         << static_cast<int>(saveDatas.stage3Clear) << ","//ステージ3のクリア
         << saveDatas.totalPlayTime                 << ","//総プレイ時間  
         << StringUtil::WstringToString(saveDatas.nowTime)//日付と時間
         << std::endl;
    //閉じる
    file.close();
}

void CSVDataSaver::SaveVolumeDataToCSV(VolumeData volumeDatas)
{
    //ファイルを開く(ない場合は作成される)
    std::ofstream file(kVolumeDataPath);
    //ファイルが読み込めなかったら
    if (!file)
    {
        assert("ファイルの読み込みに失敗");
    }

    //ヘッダーを書き込む
    file << "マスター,BGM,SE,Voice\n";

    //データ書き込み
    file << static_cast<int>(volumeDatas.masterVolume) << ","       //マスターボリューム
        << static_cast<int>(volumeDatas.bgmVolume) << ","           //BGMボリューム
        << static_cast<int>(volumeDatas.seVolume) << ","            //SEボリューム
        << static_cast<int>(volumeDatas.voiceVolume) << std::endl;  //Voiceボリューム
    //閉じる
    file.close();
}
