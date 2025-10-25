#include <fstream>   // ファイル読み込み用
#include <sstream>   // 文字列分解用（stringstream）stringをファイルのように扱える
#include "CSVDataLoader.h"
#include "CSVData.h"
#include "ActorData.h"
#include "../StringUtil.h"
#include <codecvt>
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
namespace
{
	const std::wstring kPath = L"Data/CSV/";
	const std::wstring kCSV = L".csv";
}

CSVDataLoader& CSVDataLoader::GetInstance()
{
	//この時点でメモリが確保されてアプリ終了まで残る
	static CSVDataLoader csvDataLoader;
	return csvDataLoader;
}

std::vector<std::shared_ptr<CSVData>> CSVDataLoader::LoadCSV(const wchar_t* path)
{
	//データを格納する配列
	std::vector<std::shared_ptr<CSVData>> datas;

	//パス
	std::wstring pPath = kPath + path + kCSV;
	//データをすべて読み込む
	auto valuesDatas = GetWStringList(pPath.c_str());

	for (auto values : valuesDatas)
	{
		//データ
		std::shared_ptr<CSVData> data = std::make_shared<CSVData>();
		//データを入れる
		data->SetData(values);
		//配列に追加
		datas.emplace_back(data);
	}

	return datas;
}

std::vector<std::vector<std::wstring>> CSVDataLoader::GetWStringList(const wchar_t* path)
{
	//返す値
	std::vector<std::vector<std::wstring>> valuesDatas;
	//ファイルを開く
	std::wifstream file(path);

	//ロケールを設定（環境依存、必要に応じてUTF-8などに変更）
	//ロケールは文字解釈のルールのこと
	//もしCSVファイルが UTF-8 で保存されている場合、
	//Windowsの std::wifstream はデフォルトでUTF-8を正しく解釈できないので設定した
	file.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));

	//もしもファイルを開けなかったら
	if (!file.is_open())
	{
		return valuesDatas;//空のリストを返す
	}
	//1行ずつ読み取る用の変数
	std::wstring line;
	//最初のヘッダーから要素数をカウント
	bool isHeader = false;
	std::vector<std::wstring> header;	//要素をまとめた配列
	//CSVの終わりまで読み取る
	// getlineで読み取っていく(読み取り位置（内部の「ポインタ」）は、ループのたびに前に進みます)
	//1行ずつ読み取っていき読み取る行がなくなったらfalseになる
	while (std::getline(file, line))//1行ずつ読み取る
	{
		//行をカンマ区切りで1つずつ読み込むための準備
		std::wstringstream ss(line);			//文字列をストリーム(getlineで読み取るため)に変換
		std::wstring part;					//分解して取り出した1要素
		std::vector<std::wstring> values;	//要素をまとめた配列
		//カンマ区切りで取り出していく
		//ssから,区切りで取り出していきpartに入れていく
		while (std::getline(ss, part, L',')) {
			values.emplace_back(part);           //分割された項目をリストに追加
		}
		//最初の行
		if (!isHeader)
		{
			header = values;
			isHeader = true;
			continue;
		}
		//要素数チェック
		if (values.size() < header.size())continue;//ない場合は不正な行なので飛ばす
		//データを配列に追加
		valuesDatas.emplace_back(values);
	}
	//暗黙ムーブが走るのでおそらく大丈夫
	return valuesDatas;
}
