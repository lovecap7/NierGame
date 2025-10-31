#pragma once
#include <memory>
class UIBase abstract:
	public std::enable_shared_from_this<UIBase>
{
public:
	UIBase();
	~UIBase();
	//‰Šú‰»
	virtual void Init();
	//I—¹
	virtual void End();
	//XV
	virtual void Update() abstract;
	//•`‰æ
	virtual void Draw()const abstract;
};

