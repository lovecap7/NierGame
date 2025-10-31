#pragma once
#include <map>
#include <string>
/// <summary>
/// リソース管理を行うクラス
/// </summary>
class AssetManager final
{
private:
	//シングルトンの準備
	AssetManager() = default;
	~AssetManager() = default;
	//コピー禁止
	AssetManager(const AssetManager&) = delete;
	AssetManager& operator = (const AssetManager&) = delete;
	//ムーブ禁止
	AssetManager(AssetManager&&) = delete;
	AssetManager& operator = (AssetManager&&) = delete;
public:
	//インスタンスを取得
	static AssetManager& GetInstance()
	{
		static AssetManager instance;
		return instance;
	}

	//モデルの取得
	int GetModelHandle(std::wstring path);
	//モデルのリソースの削除
	void DeleteModelHandle();

	//エフェクトの取得
	int GetEffectHandle(std::wstring path);
	//エフェクトのリソースの削除
	void DeleteEffectHandle();

	//画像ハンドル
	int GetImageHandle(std::wstring path);
	//画像ハンドル削除
	void DeleteImageHandle();
private:
	//モデルリソース
	std::map<std::wstring,int>m_modelHandles;
	//エフェクトリソース
	std::map<std::wstring, int>m_effectHandles;
	//画像ハンドル
	std::map<std::wstring, int>m_imageHandles;
};

