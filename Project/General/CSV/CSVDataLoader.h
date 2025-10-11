#pragma once
#include <DxLib.h>
#include <string>
#include <vector>
#include <memory>
class CSVData;
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
    std::vector<std::shared_ptr<CSVData>> LoadCSV(const char* path);
private:
    std::vector<std::vector<std::wstring>> GetWStringList(const char* path);
};