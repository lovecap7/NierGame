#include "PlayerStateHit.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateFall.h"
#include "../../../../General/Model.h"
#include "../../../../General/Input.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../General/ShaderPostProcess.h"
#include "../../../../Main/Application.h"

namespace
{
	//アニメーション
	const std::wstring kHit = L"Hit";
	//モデルの旋回速度
	constexpr int kModelRotateSpeed = 5;

	//グリッジ(通常)
	constexpr float kGlitchFrame = 20.0f;
	constexpr float kGlitchScale = 10.0f;
	constexpr float kGlitchSpeed = 1.0f;
	constexpr float kGlitchStrengt = 1.0f;
	//ピンチ
	constexpr float kPinchGlitchFrame = 45.0f;
	constexpr float kPinchGlitchScale = 15.0f;
	constexpr float kPinchGlitchSpeed = 5.0f;
	constexpr float kPinchGlitchStrengt = 5.0f;
}

PlayerStateHit::PlayerStateHit(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_glitchFrame(0.0f),
	m_glitchScale(0.0f),
	m_glitchSpeed(0.0f),
	m_glitchkStrengt(0.0f)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->GetModel()->SetAnim(owner->GetAnim(kHit).c_str(), false);
	owner->SetCollState(CollisionState::Normal);

	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(true);

	//グリッジ
	auto& app = Application::GetInstance();
	auto& postEff = app.GetPostProcess();
	postEff->AddPostEffectState(ShaderPostProcess::PostEffectState::Glitch);

	//体力が3割を切ったら
	if (status->IsPinchHP())
	{
		m_glitchFrame = kPinchGlitchFrame;
		m_glitchScale = kPinchGlitchScale;
		m_glitchSpeed = kPinchGlitchSpeed;
		m_glitchkStrengt = kPinchGlitchStrengt;
	}
	else
	{
		m_glitchFrame = kGlitchFrame;
		m_glitchScale = kGlitchScale;
		m_glitchSpeed = kGlitchSpeed;
		m_glitchkStrengt = kGlitchStrengt;
	}
}

PlayerStateHit::~PlayerStateHit()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto status = owner->GetCharaStatus();
	//無敵
	status->SetIsNoDamage(false);

	auto& app = Application::GetInstance();
	auto& postEff = app.GetPostProcess();
	postEff->SetBlockScele(0.0f);
	postEff->SetNoiseSpeed(0.0f);
	postEff->SetShakeStrength(0.0f);
}
void PlayerStateHit::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}
void PlayerStateHit::Update()
{
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto& app = Application::GetInstance();
	auto& postEff = app.GetPostProcess();
	auto status = owner->GetCharaStatus();
	CountFrame();

	//グリッジフレーム間の処理
	if (m_frame >= kGlitchFrame && !status->IsPinchHP())
	{
		//グリッジ
		postEff->SubPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
	}
	else
	{
		//だんだん弱く
		float rate = m_frame / m_glitchFrame;
		postEff->SetBlockScele(MathSub::Lerp(m_glitchScale,0.0f, rate));
		postEff->SetNoiseSpeed(MathSub::Lerp(m_glitchSpeed, 0.0f, rate));
		postEff->SetShakeStrength(MathSub::Lerp(m_glitchkStrengt, 0.0f, rate));
	}

	//モデルのアニメーションが終わったら
	if (owner->GetModel()->IsFinishAnim())
	{
		//待機
		ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
		return;
	}
}
