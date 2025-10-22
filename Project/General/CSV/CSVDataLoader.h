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
    std::vector<std::shared_ptr<CSVData>> LoadCSV(const wchar_t* path);
private:
    std::vector<std::vector<std::wstring>> GetWStringList(const wchar_t* path);
};