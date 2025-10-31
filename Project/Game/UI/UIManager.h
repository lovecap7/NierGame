#pragma once
#include <memory>
#include <list>
class UIBase;
class UIManager final
{
private:
	//シングルトンの準備
	UIManager() = default;
	~UIManager() = default;
	//コピー禁止
	UIManager(const UIManager&) = delete;
	UIManager& operator = (const UIManager&) = delete;
	//ムーブ禁止
	UIManager(UIManager&&) = delete;
	UIManager& operator = (UIManager&&) = delete;
public:
	//インスタンスを取得
	static UIManager& GetInstance()
	{
		static UIManager instance;
		return instance;
	}
	//登録
	void Entry(std::shared_ptr<UIBase> ui);
	//解除
	void Exit(std::shared_ptr<UIBase> ui);

	//更新
	void Update();
	//描画
	void Draw()const;
private:
	std::list<std::shared_ptr<UIBase>> m_uis;
private:
	
};

