#pragma once
#include "CSVData.h"
#include <memory>
#include <string>
#include "../Math/MyMath.h"
class EffectData :
	public CSVData
{
public:
	EffectData();
	EffectData(std::shared_ptr<CSVData> data);
	~EffectData();
	
	//–¼‘O
	std::wstring m_name;

	//ƒpƒX
	std::wstring m_path;

private:
	//•ÏŠ·
	void Conversion() override;
};

