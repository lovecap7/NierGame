#pragma once
#include "CSVDatas.h"
#include <string>
#include <memory>
class Score;
/// <summary>
/// 保存したいデータをCSVに保存する
/// </summary>
class CSVDataSaver
{
public:
	CSVDataSaver();
	~CSVDataSaver();
	/// <summary>
	/// スコアを保存する
	/// </summary>
	/// <param name="score"></param>
	void SaveScoreDataToCSV(std::shared_ptr<Score> score);

	/// <summary>
	/// ゲームの進捗度などを記録
	/// </summary>
	void SaveDataToCSV(SaveDatas saveDatas);
	/// <summary>
	/// 音量を保存
	/// </summary>
	void SaveVolumeDataToCSV(VolumeData volumeDatas);
};

