#include "CharaStatus.h"
#include "Math/MathSub.h"

namespace
{
	constexpr int kMaxStatus = 9999;
	constexpr int kMinStatus = 1;
}

CharaStatus::CharaStatus():
	m_maxHp(0),
	m_nowHp(0),
	m_at(0),
	m_df(0),
	m_ms(0.0f)
{
}

CharaStatus::~CharaStatus()
{
}

void CharaStatus::OnDamage(int power, int at)
{
	int atk = MathSub::ClampInt(at, kMinStatus, kMaxStatus);
	int df = MathSub::ClampInt(m_df, kMinStatus, kMaxStatus);
	int damage = power * (atk * atk) / (atk + df);
}
