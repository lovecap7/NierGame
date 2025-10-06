#include <fstream>   // ファイル読み込み用
#include <sstream>   // 文字列分解用（stringstream）stringをファイルのように扱える
#include "CSVDataLoader.h"
#include "StringUtil.h"
namespace
{
	//アクターのデータ数
	constexpr int kActorDataElementNum = 19;
	//攻撃のデータ数
	constexpr int kAttackDataElementNum = 16;
	//スコアデータの数
	constexpr int kScoreDataElementNum = 2;
	//スコアデータの数
	constexpr int kResultScoreUIDataElementNum = 4;
	//UIデータ数
	constexpr int kUIDataElementNum = 3;
	//セーブデータの要素数
	constexpr int kSaveDataElementNum = 5;
	//音量の要素数
	constexpr int kVolumeDataElementNum = 4;
	//Unityの座標に掛けることでDXライブラリでもUnityと同じ大きさになる
	constexpr float kUnityToDXPosition = 100.0f;
}

CSVDataLoader::CSVDataLoader()
{
}

CSVDataLoader::~CSVDataLoader()
{
}

std::vector<ActorData> CSVDataLoader::LoadActorDataCSV(const char* fileName)
{
	//データを格納する配列
	std::vector<ActorData> objects;

	//データをすべて読み込む
	auto valuesDatas = GetStringList(fileName, kActorDataElementNum);

	for(auto values: valuesDatas)
	{
		//構造体にデータを入れていく
		ActorData objData;
		
		//名前
		objData.name = values[0];
		//座標
		objData.pos.x	= std::stof(values[1]) * kUnityToDXPosition;	//std::stofは文字列をfloatに変換する
		objData.pos.y	= std::stof(values[2]) * kUnityToDXPosition;
		objData.pos.z	= std::stof(values[3]) * kUnityToDXPosition;
		//回転
		objData.rot.x	= std::stof(values[4]);	
		objData.rot.y	= std::stof(values[5]);
		objData.rot.z	= -1 * std::stof(values[6]);
		//大きさ
		objData.scale.x = std::stof(values[7]);	
		objData.scale.y = std::stof(values[8]);
		objData.scale.z = std::stof(values[9]);
		//グループ
		objData.gropeTag = values[10];
		//体力
		objData.hp = std::stoi(values[11]);
		//アーマー
		objData.armor = static_cast<Battle::Armor>(std::stoi(values[12]));
		//攻撃力
		objData.attackPower = std::stoi(values[13]);
		//速度
		objData.speed = std::stof(values[14]);
		//探索範囲
		objData.searchPlaceRang = std::stof(values[15]);
		//視野角
		objData.viewingAngle = std::stof(values[16]) * MyMath::DEG_2_RAD;
		//索敵距離
		objData.searchDistance = std::stof(values[17]);
		//モデルの旋回速度
		objData.modelRotateSpeed = std::stof(values[18]);

		//配列に追加
		objects.emplace_back(objData);
	}

	return objects;
}

std::vector<AttackData> CSVDataLoader::LoadAttackDataCSV()
{
	//データを格納する配列
	std::vector<AttackData> attackDatas;
	//データをすべて読み込む
	auto valuesDatas = GetStringList(kCharacterAttackDataPath.c_str(), kAttackDataElementNum);
	for (auto values : valuesDatas)
	{
		//構造体にデータを入れていく
		AttackData attackData;
		//持ち主の名前
		attackData.ownerName = values[0];
		//攻撃の名前
		attackData.attackName = values[1];
		//ダメージ
		attackData.damege = std::stoi(values[2]);
		//持続
		attackData.keepFrame = std::stoi(values[3]);
		//発生
		attackData.startFrame = std::stoi(values[4]);
		//ノックバックの大きさ
		attackData.knockBackPower = std::stof(values[5]);
		//アニメーション
		attackData.anim =  values[6];
		//アニメーション速度
		attackData.animSpeed = std::stof(values[7]);
		//攻撃の半径
		attackData.radius = std::stof(values[8]);
		//必殺ゲージの上昇量
		attackData.addUltGage = std::stoi(values[9]);
		//前進フレーム
		attackData.moveFrame = std::stoi(values[10]);
		//前進速度
		attackData.moveSpeed = std::stof(values[11]);
		//攻撃の重さ
		auto aw = values[12];
		if		(aw == "Light")		attackData.attackWeight	= Battle::AttackWeight::Light;
		else if (aw == "Middle")	attackData.attackWeight	= Battle::AttackWeight::Middle;
		else if (aw == "Heavy")		attackData.attackWeight	= Battle::AttackWeight::Heavy;
		else if (aw == "Heaviest")	attackData.attackWeight = Battle::AttackWeight::Heaviest;
		//ヒットストップの長さ
		attackData.hitStopFrame = std::stoi(values[13]);
		//カメラの揺れ
		auto sp = values[14];
		if		(sp == "None")		attackData.shakePower = ShakePower::None;
		else if (sp == "Low")		attackData.shakePower = ShakePower::Low;
		else if (sp == "Middle")	attackData.shakePower = ShakePower::Middle;
		else if (sp == "High")		attackData.shakePower = ShakePower::High;
		else if (sp == "Highest")	attackData.shakePower = ShakePower::Highest;
		//攻撃回数
		attackData.attackNum = std::stoi(values[15]);
		//配列に追加
		attackDatas.emplace_back(attackData);
	}
	return attackDatas;
}

std::vector<ScoreData> CSVDataLoader::LoadActorScoreDataCSV()
{
	//データを格納する配列
	std::vector<ScoreData> scoreDatas;
	//データをすべて読み込む
	auto valuesDatas = GetStringList(kActorScoreDataPath.c_str(), kScoreDataElementNum);
	for (auto values : valuesDatas)
	{
		//構造体にデータを入れていく
		ScoreData scoreData;
		//持ち主の名前
		scoreData.dataName = values[0];
		//攻撃の名前
		scoreData.score = std::stoi(values[1]);
		//配列に追加
		scoreDatas.emplace_back(scoreData);
	}
	return scoreDatas;
}

std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> CSVDataLoader::LoadHighScoreDataCSV()
{
	//データを格納する配列
	std::array<std::array<int, 3>, static_cast<int>(Stage::StageIndex::StageNum)> highScoreDatas;
	//データをすべて読み込む
	auto valuesDatas = GetStringList(kHighScoreDataPath.c_str(), 3);
	for (int i = 0;i < highScoreDatas.size(); ++i)
	{
		//配列に追加(各ステージの1位から3位までのハイスコアを取得)
		highScoreDatas[i][0] = std::stoi(valuesDatas[i][0]);
		highScoreDatas[i][1] = std::stoi(valuesDatas[i][1]);
		highScoreDatas[i][2] = std::stoi(valuesDatas[i][2]);
	}
	return highScoreDatas;
}

std::vector<ResultScoreUIData> CSVDataLoader::LoadResultScoreUIDataCSV()
{
	//データを格納する配列
	std::vector<ResultScoreUIData> resultScoreUIDatas;
	//データをすべて読み込む
	auto valuesDatas = GetWStringList(kResutlScoreDataPath.c_str(), kResultScoreUIDataElementNum);
	for (auto values : valuesDatas)
	{
		//構造体にデータを入れていく
		ResultScoreUIData resultScoreUIData;
		//名前
		resultScoreUIData.name = StringUtil::WstringToString(values[0]);
		//座標
		resultScoreUIData.pos.x = std::stof(values[1]);
		resultScoreUIData.pos.y = std::stof(values[2]);
		//テキスト
		resultScoreUIData.text = values[3];
		//配列に追加
		resultScoreUIDatas.emplace_back(resultScoreUIData);
	}
	return resultScoreUIDatas;
}

std::vector<UIData> CSVDataLoader::LoadUIDataCSV(const char* fileName)
{
	//データを格納する配列
	std::vector<UIData> uis;

	//データをすべて読み込む
	auto uiDatas = GetWStringList(fileName, kUIDataElementNum);

	for (auto data : uiDatas)
	{
		//構造体にデータを入れていく
		UIData uiData;
		//テキスト
		uiData.text = data[0];
		//座標
		uiData.pos.x = std::stof(data[1]);
		uiData.pos.y = std::stof(data[2]);
		//配列に追加
		uis.emplace_back(uiData);
	}

	return uis;
}

SaveDatas CSVDataLoader::LoadSaveDataCSV()
{
	//データを保存する変数
	SaveDatas saveDatas;
	//データをすべて読み込む
	auto valuesDatas = GetWStringList(kSaveDataPath.c_str(), kSaveDataElementNum);
	//データを格納してく
	for (auto data : valuesDatas)
	{		
		//クリアしたステージのロード
		saveDatas.stage1Clear = (data[0] == L"1");
		saveDatas.stage2Clear = (data[1] == L"1");
		saveDatas.stage3Clear = (data[2] == L"1");
		//総プレイ時間
		saveDatas.totalPlayTime = std::stoi(data[3]);
		//日付と時間
		saveDatas.nowTime = data[4];
	}
	return saveDatas;
}

VolumeData CSVDataLoader::LoadVolumeDataCSV()
{
	//データを保存する変数
	VolumeData volumeDatas;
	//データをすべて読み込む
	auto valuesDatas = GetStringList(kVolumeDataPath.c_str(), kVolumeDataElementNum);
	//データを格納してく
	for (auto data : valuesDatas)
	{
		//音量セット
		volumeDatas.masterVolume	= stoi(data[0]);
		volumeDatas.bgmVolume		= stoi(data[1]);
		volumeDatas.seVolume		= stoi(data[2]);
		volumeDatas.voiceVolume		= stoi(data[3]);
	}
	return volumeDatas;
}

//データをすべて読み込む
const std::vector<std::vector<std::string>> CSVDataLoader::GetStringList(const char* fileName, int elementNum)
{
	//返す値
	std::vector<std::vector<std::string>> valuesDatas;
	//ファイルを開く
	std::ifstream file(fileName);
	//もしもファイルを開けなかったら
	if (!file.is_open())return valuesDatas;//空のリストを返す
	//1行ずつ読み取る用の変数
	std::string line;
	//最初のヘッダーはスキップしたい
	bool isHeader = true;
	//CSVの終わりまで読み取る
	// getlineで読み取っていく(読み取り位置（内部の「ポインタ」）は、ループのたびに前に進みます)
	//1行ずつ読み取っていき読み取る行がなくなったらfalseになる
	while (std::getline(file, line))//1行ずつ読み取る
	{
		//最初の行はスキップする(ヘッダー)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		//行をカンマ区切りで1つずつ読み込むための準備
		std::stringstream ss(line);			//文字列をストリーム(getlineで読み取るため)に変換
		std::string part;					//分解して取り出した1要素
		std::vector<std::string> values;	//要素をまとめた配列
		//カンマ区切りで取り出していく
		//ssから,区切りで取り出していきpartに入れていく
		while (std::getline(ss, part, ',')) {
			values.emplace_back(part);           //分割された項目をリストに追加
		}
		//要素数チェック
		if (values.size() < elementNum)continue;//ない場合は不正な行なので飛ばす
		//データを配列に追加
		valuesDatas.emplace_back(values);
	}
	//暗黙ムーブが走るのでおそらく大丈夫
	return valuesDatas;
}

const std::vector<std::vector<std::wstring>> CSVDataLoader::GetWStringList(const char* fileName, int elementNum)
{
	//返す値
	std::vector<std::vector<std::wstring>> valuesDatas;
	//ファイルを開く
	std::wifstream file(fileName);

	//ロケールを設定（環境依存、必要に応じてUTF-8などに変更）
	//ロケールは文字解釈のルールのこと
	//もしCSVファイルが UTF-8 で保存されている場合、
	//Windowsの std::wifstream はデフォルトでUTF-8を正しく解釈できないので設定した
	file.imbue(std::locale("")); 

	//もしもファイルを開けなかったら
	if (!file.is_open())return valuesDatas;//空のリストを返す
	//1行ずつ読み取る用の変数
	std::wstring line;
	//最初のヘッダーはスキップしたい
	bool isHeader = true;
	//CSVの終わりまで読み取る
	// getlineで読み取っていく(読み取り位置（内部の「ポインタ」）は、ループのたびに前に進みます)
	//1行ずつ読み取っていき読み取る行がなくなったらfalseになる
	while (std::getline(file, line))//1行ずつ読み取る
	{
		//最初の行はスキップする(ヘッダー)
		if (isHeader)
		{
			isHeader = false;
			continue;
		}
		//行をカンマ区切りで1つずつ読み込むための準備
		std::wstringstream ss(line);			//文字列をストリーム(getlineで読み取るため)に変換
		std::wstring part;					//分解して取り出した1要素
		std::vector<std::wstring> values;	//要素をまとめた配列
		//カンマ区切りで取り出していく
		//ssから,区切りで取り出していきpartに入れていく
		while (std::getline(ss, part, L',')) {
			values.emplace_back(part);           //分割された項目をリストに追加
		}
		//要素数チェック
		if (values.size() < elementNum)continue;//ない場合は不正な行なので飛ばす
		//データを配列に追加
		valuesDatas.emplace_back(values);
	}
	//暗黙ムーブが走るのでおそらく大丈夫
	return valuesDatas;
}
