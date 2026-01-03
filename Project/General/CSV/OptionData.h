#pragma once
#include "CSVData.h"
#include <memory>
class OptionData :
    public CSVData
{
public:
	OptionData();
	OptionData(std::shared_ptr<CSVData> data);
	~OptionData();
	
private:
	//変換
	void Conversion() override;

private:
	//音量
	int m_bgmLv;
	int m_seLv;
	int m_voiceLv;
	//スクリーンモード
	bool m_isWindow;
};

