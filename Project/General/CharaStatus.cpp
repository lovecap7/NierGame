#include "CharaStatus.h"
#include "Math/MyMath.h"
#include "CSV/CharaStatusData.h"
#include "ShaderPostProcess.h"
#include "../Main/Application.h"
#include <cmath>

namespace
{
	constexpr int kMaxStatus = 9999;
	constexpr int kMinStatus = 1;
	constexpr float randomMin = 0.9f;
	constexpr float randomMax = 1.1f;
}

CharaStatus::CharaStatus(std::shared_ptr<CharaStatusData> charaStatusData) :
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
	m_charaStatusData(charaStatusData),
	m_ownerPos(),
	m_searchAngle(0.0f),
	m_searchRange(0.0f),
	m_meleeAttackRange(0.0f)
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
		m_searchAngle = m_charaStatusData->m_searchAngle;
		m_searchRange = m_charaStatusData->m_searchRange;
		m_meleeAttackRange = m_charaStatusData->m_meleeAttackRange;
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
	int damage = GetDamage(power, at);
	m_nowHp -= damage;
	m_nowHp = MathSub::ClampInt(m_nowHp, 0, m_maxHp);
#if _DEBUG
	printf("ダメージを受けた！！ %d\n", damage);
#endif
	//ひるむか
	if (CheckHitReaction(aw, m_ar))
	{
		m_isHitReaction = true;
	}
}

Vector3 CharaStatus::GetKnockBack(const Vector3& charPos, const Vector3& attackPos, float power, float up)
{
	//向きを計算(攻撃からキャラへのベクトル)
	Vector3 vec = charPos - attackPos;
	vec.y = 0.0f; //Y成分は無視
	if (vec.SqMagnitude() > 0.0f)
	{
		vec = vec.Normalize();
	}
	vec *= power;
	vec.y = up;
	return vec;
}

void CharaStatus::Heal(int value)
{
	m_nowHp += abs(value);
	m_nowHp = MathSub::ClampInt(m_nowHp, 0, m_maxHp);
	if (!IsPinchHP())
	{
		//ポストエフェクト解除
		auto& app = Application::GetInstance();
		auto& postEff = app.GetPostProcess();
		postEff->SubPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
		postEff->SubPostEffectState(ShaderPostProcess::PostEffectState::Gray);
	}
}

void CharaStatus::FullRecovery()
{
	Heal(m_maxHp);
}

bool CharaStatus::IsPinchHP() const
{
	return m_nowHp <= (m_maxHp / 3);
}

int CharaStatus::GetDamage(int power, int at)
{
	//攻撃力
	float atk = static_cast<float>(MathSub::ClampInt(at, kMinStatus, kMaxStatus));
	//防御力
	float df = static_cast<float>(MathSub::ClampInt(m_df, kMinStatus, kMaxStatus));
	//乱数
	float random = MyMath::GetRandF(randomMin, randomMax);

	//攻撃と防御の比率で減衰（防御が高いほど小さくなる）
	float rate = atk / (atk + df);

	//ダメージ = 威力 × 攻防比 × 乱数
	int damage = power * rate * random;

	//1は最低値
	if (damage <= 0)
	{
		damage = 1;
	}

	return damage;
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