#pragma once
#include <memory>
#include <list>
class UIBase;
class UIManager
{
public:
	UIManager();
	~UIManager();
	//“o˜^
	void Entry(std::shared_ptr<UIBase> attack);
	//‰ğœ
	void Exit(std::shared_ptr<UIBase> attack);
	//‰Šú‰»
	void Init();
	//XV
	void Update();
	//•`‰æ
	void Draw()const;
	//I—¹ˆ—
	void End();
private:
	std::list<std::shared_ptr<UIBase>> m_uis;
private:
	
};

