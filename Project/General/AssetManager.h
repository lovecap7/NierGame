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

private:
	//モデルリソース
	std::map<std::wstring,int>m_modelHandles;
	//エフェクトリソース
	std::map<std::wstring, int>m_effectHandle;

};

