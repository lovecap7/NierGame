#pragma once
#include "CSVData.h"
#include <memory>
#include <string>
#include "../Math/MyMath.h"
class PodData :
	public CSVData
{
public:
	PodData();
	PodData(std::shared_ptr<CSVData> data);
	~PodData();
	//‘å‚«‚³
	Vector3 m_scale;
	//ƒ‚ƒfƒ‹‚ÌƒpƒX
	std::wstring m_modelPath;
	//UŒ‚—Í
	int m_at;
private:
	//•ÏŠ·
	void Conversion() override;
};

