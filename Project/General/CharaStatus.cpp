#include "CharaStatus.h"
#include "Math/MyMath.h"
#include "CSV/CharaStatusData.h"
#include <cmath>

namespace
{
	constexpr int kMaxStatus = 9999;
	constexpr int kMinStatus = 1;
}

CharaStatus::CharaStatus(std::shared_ptr<CharaStatusData> charaStatusData):
	m_maxHp(0),
	m_nowHp(0),
	m_at(0),
	m_df(0),
	m_ms(0.0f),
	m_jp(0.0f),
	m_ar(CharaStatus::Armor::Light),
	m_isNoDamage(false),
	m_isHit(false),
	m_isHitReaction(false),
	m_charaStatusData(charaStatusData)
{
	//初期化
	Init();
}

CharaStatus::~CharaStatus()
{
}

void CharaStatus::Init()
{
	//値の初期化
	if (m_charaStatusData)
	{
		m_maxHp = m_charaStatusData->m_hp;
		m_nowHp = m_charaStatusData->m_hp;
		m_at = m_charaStatusData->m_at;
		m_df = m_charaStatusData->m_df;
		m_ms = m_charaStatusData->m_ms;
		m_jp = m_charaStatusData->m_jp;
		m_ar = m_charaStatusData->m_ar;
		m_isNoDamage = false;
		InitHitState();
	}
}

void CharaStatus::InitHitState()
{
	m_isHit			= false;
	m_isHitReaction = false;
}

void CharaStatus::OnDamage(int power, int at, CharaStatus::AttackWeight aw)
{
	//攻撃は当たっているので
	m_isHit = true;

	//無敵中はダメージを食らわない
	if (m_isNoDamage)return;

	//ダメージ計算
	int atk = MathSub::ClampInt(at, kMinStatus, kMaxStatus);
	int df	= MathSub::ClampInt(m_df, kMinStatus, kMaxStatus);
	int damage = static_cast<float>(power) * static_cast<float>((atk * atk) / (atk + df));
	m_nowHp -= damage;
	m_nowHp = MathSub::ClampInt(m_nowHp, 0, m_maxHp);

	//ひるむか
	if (CheckHitReaction(aw, m_ar))
	{
		m_isHitReaction = true;
	}
}

void CharaStatus::Heal(int value)
{
	m_nowHp += abs(value);
	m_nowHp = MathSub::ClampInt(m_nowHp, 0, m_maxHp);
}

void CharaStatus::FullRecovery()
{
	m_nowHp = m_maxHp;
}

bool CharaStatus::CheckHitReaction(AttackWeight aw, Armor am)
{
	//攻撃の大きさがアーマーより大きいならtrue
	return (aw > am);
}


CharaStatus::AttackWeight operator+(const CharaStatus::AttackWeight& left, const CharaStatus::AttackWeight& right)
{
	int result = static_cast<int>(left) + static_cast<int>(right);
	result = MathSub::ClampInt(result, static_cast<int>(CharaStatus::AttackWeight::Light), static_cast<int>(CharaStatus::AttackWeight::Heaviest));
	return  static_cast<CharaStatus::AttackWeight>(result);
}

CharaStatus::AttackWeight operator+(const CharaStatus::AttackWeight& left, const int right)
{
	int result = static_cast<int>(left) + right;
	result = MathSub::ClampInt(result, static_cast<int>(CharaStatus::AttackWeight::Light), static_cast<int>(CharaStatus::AttackWeight::Heaviest));
	return  static_cast<CharaStatus::AttackWeight>(result);
}