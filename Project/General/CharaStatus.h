#pragma once
//ステータスを管理します
class CharaStatus
{
public:
	CharaStatus();
	~CharaStatus();
	//体力
	int GetMaxHP()const { return m_maxHp; };
	void SetMaxHP(int maxHP) { m_maxHp = maxHP; };
	int GetNowHP()const { return m_nowHp; };
	void SetNowHP(int nowHP) { m_nowHp = nowHP; };
	//攻撃力
	int GetAT()const { return m_at; };
	void SetAT(int at) { m_at = at; };
	//防御力
	int GetDF()const { return m_df; };
	void SetDF(float df) { m_df = df; };
	//移動速度
	float GetMS()const { return m_ms; };
	void SetMS(float ms) { m_ms = ms; };
	//ダメージ
	void OnDamage(int power,int at);
private:
	//ステータス
	int m_maxHp;	//最大体力
	int m_nowHp;	//現在体力
	int m_at;		//攻撃力
	int m_df;		//防御力
	float m_ms;		//移動速度
};

