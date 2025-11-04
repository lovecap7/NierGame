#include "AttackManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "../../Main/Application.h"
#include "AttackBase.h"

namespace
{
	constexpr float kHitStopTimeScale = 0.05f;
}

AttackManager::AttackManager():
	m_hitStopFrame(0),
	m_isHitStop(false)
{

}

AttackManager::~AttackManager()
{
}

//攻撃を追加
void AttackManager::Entry(std::shared_ptr<AttackBase> attack)
{
	//すでに登録されているならしない
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it != m_attacks.end())return;
	//攻撃の初期化
	attack->Init();
	//攻撃を追加
	m_attacks.emplace_back(attack);
}

void AttackManager::Exit(std::shared_ptr<AttackBase> attack)
{
	//登録されていないならしない
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it == m_attacks.end())return;
	attack->End();
	m_attacks.erase(it);
}


void AttackManager::Init()
{
}

void AttackManager::Update()
{
	//攻撃の更新とヒットストップを行うか
	for (auto& attack : m_attacks)
	{
		//ヒットストップをするか
		if (attack->IsRequestHitStop())
		{
			HitStop(attack);
		}

		//更新
		attack->Update();
	}

	//削除チェック
	CheckDelete();

	//ヒットストップ
	if (m_hitStopFrame >= 0 && m_isHitStop)
	{
		auto& app = Application::GetInstance();
		--m_hitStopFrame;
		if (m_hitStopFrame <= 0)
		{
			//終了
			m_isHitStop = false;
			//もとに戻す
			app.SetTimeScale(1.0f);
		}
	}
}

void AttackManager::Draw() const
{
	//描画
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
}

void AttackManager::End()
{
}

void AttackManager::HitStop(std::shared_ptr<AttackBase> attack)
{
	auto& app = Application::GetInstance();
	app.SetTimeScale(kHitStopTimeScale);
	//ヒットストップ
	m_isHitStop = true;
	//フレーム数は大きいほうを優先
	m_hitStopFrame = MathSub::Max(m_hitStopFrame, attack->GetHitStopFrame());
	////カメラの揺れも大きいほうを優先
	//float cameraShakePower = MathSub::Max(m_hitStopFrame, attack->GetHitStopShakePower());
}

//消滅フラグをチェックして削除
void AttackManager::CheckDelete()
{
	//消滅フラグが立っているものを削除
	m_attacks.remove_if([](const std::shared_ptr<AttackBase>& attack) {
		if (attack->IsDelete()) {
			attack->End();
			return true;
		}
		return false;
		});
}