#pragma once
//ステータスを管理します
class CharaStatus
{
public:
	CharaStatus();
	~CharaStatus();
	//体力
	int GetMaxHP()const { return m_maxHp; };
	int GetNowHP()const { return m_nowHp; };
	//攻撃力
	int GetAT()const { return m_at; };
	//防御力
	int GetDF()const { return m_df; };
	//移動速度
	float GetMS()const { return m_ms; };
private:
	//ステータス
	int m_maxHp;	//最大体力
	int m_nowHp;	//現在体力
	int m_at;		//攻撃力
	int m_df;		//防御力
	float m_ms;		//移動速度
};

