#pragma once
#include "../UIBase.h"
#include <memory>
class CharaStatus;
class PlayerHPUI :
    public UIBase
{
public:
	PlayerHPUI(std::shared_ptr<CharaStatus> charaStatus);
	~PlayerHPUI();
	//更新
	void Update() override;
	//描画
	void Draw()const override;
private:
	//プレイヤーのステータス
	std::shared_ptr<CharaStatus> m_playerStatus;

	//前の現在体力
	float m_beforeNowHP;

	//ダメージフレーム
	int m_damageFrame;
	//ダメージバーの割合
	float m_damageValueRate;

	//回復フレーム
	int m_healFrame;
	//回復バーの割合
	float m_healValueRate;
private:
	//体力ハンドル
	int m_nowValueHandle;
	//後ろのバー
	int m_backValueHandle;
	//ダメージ
	int m_damageValueHandle;
	//回復
	int m_healValueHandle;
};

