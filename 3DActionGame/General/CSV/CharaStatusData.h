#pragma once
#include "CSVData.h"
#include "../CharaStatus.h"
#include <memory>
class CharaStatusData :
    public CSVData
{
public:
	CharaStatusData();
	CharaStatusData(std::shared_ptr<CSVData> data);
	~CharaStatusData();
	//ステータス
	int					m_id;		//ID
	int					m_hp;		//最大体力
	int					m_at;		//攻撃力
	int					m_df;		//防御力
	float				m_ms;		//移動速度
	float				m_jp;		//ジャンプ力
	CharaStatus::Armor  m_ar;		//アーマー
	float m_searchRange;			//索敵半径
	float m_searchAngle;			//索敵アングル
	float m_meleeAttackRange;			//近接攻撃距離
private:
	//変換
	void Conversion() override;
};

