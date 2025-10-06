#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <array>
#include "CSVDatas.h"

class CSVDataLoader
{
public:
    CSVDataLoader();
	virtual ~CSVDataLoader();

    /// <summary>
    /// CSVファイルからオブジェクトのTranceform情報を取得
    /// </summary>
    /// <param name="fileName">csv</param>
    /// <returns></returns>
    std::vector<ActorData> LoadActorDataCSV(const char* fileName);

    /// <summary>
    /// 攻撃データの読み取り
    /// </summary>
    /// <param name="fileName"></param>
    /// <param name="name"></param>
    /// <returns></returns>
    std::vector<AttackData> LoadAttackDataCSV();

    /// <summary>
    /// アクター毎のスコアデータの読み取り
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    std::vector<ScoreData> LoadActorScoreDataCSV();

    /// <summary>
    /// ハイスコアデータの読み取り
    /// </summary>
    /// <param name="fileName"></param>
    /// <returns></returns>
    std::array<std::array<int,3>, static_cast<int>(Stage::StageIndex::StageNum)> LoadHighScoreDataCSV();

    /// <summary>
    /// リザルトスコアUI情報の読み取り
    /// </summary>
    /// <returns></returns>
    std::vector <ResultScoreUIData> LoadResultScoreUIDataCSV();

    /// <summary>
    /// UI情報の読み取り
    /// </summary>
    /// <returns></returns>
    std::vector <UIData> LoadUIDataCSV(const char* fileName);

    /// <summary>
    /// セーブデータの読み取り
    /// </summary>
    /// <returns></returns>
    SaveDatas LoadSaveDataCSV();
    /// <summary>
    /// 音量の読み取り
    /// </summary>
    /// <returns></returns>
    VolumeData LoadVolumeDataCSV();
private:
    const std::vector<std::vector<std::string>> GetStringList(const char* fileName,int elementNum);
    const std::vector<std::vector<std::wstring>> GetWStringList(const char* fileName,int elementNum);
};