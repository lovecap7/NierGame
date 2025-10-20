#include "PlayerStateAvoid.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "PlayerStateMoving.h"
#include "PlayerStateLightAttack.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../Main/Application.h"
#include "../../../../General/ShaderPostProcess.h"
namespace
{
	constexpr float kSpeedDif = 10.0f;
	constexpr float kLerpSpeedRate = 0.1f;
	
	//アニメーション
	const std::wstring kForwardAvoid = L"AvoidForward";
	const std::wstring kBackAvoid = L"AvoidBack";
	const std::wstring kJustAvoid = L"JustAvoid";

	//ジャスト回避フレーム
	constexpr float kJustFrame = 15.0f;
	//スロー速度
	constexpr float kSlowSpeed = 0.5f;
	//無敵フレーム
	constexpr float kNoDamageFrame = 30.0f;
	//ジャスト回避終了前
	constexpr float kFisishJustAvoidFrame = 5.0f;
}

PlayerStateAvoid::PlayerStateAvoid(std::weak_ptr<Actor> player) :
	PlayerStateBase(player),
	m_avoidDir(),
	m_speed(0.0f),
	m_endSpeed(0.0f),
	m_isJustAvoid(false),
	m_finishJustAvoid(0.0f)
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->SetCollState(CollisionState::Move);

	//武器は持たない
	owner->PutAwaySword();

	auto& input = Input::GetInstance();
	//移動
	Vector3 vec = Vector3::Zero();
	vec.x = input.GetStickInfo().leftStickX;
	vec.z = -input.GetStickInfo().leftStickY;
	//下がる
	bool isBack = false;
	//移動方向があるなら
	if (vec.SqMagnitude() > 0.0f)
	{
		vec = vec.Normalize();
	}
	//ない場合は下がる
	else
	{
		isBack = true;
		vec = Vector3::Back();
	}
	//カメラの向きに合わせて移動方向を変える
	vec = owner->GetCameraRot() * vec;
	//回避方向設定
	m_avoidDir = vec;

	//前進か下がるかでアニメーション切り替え
	if (!isBack)
	{
		owner->GetModel()->SetAnim(owner->GetAnim(kForwardAvoid).c_str(), false);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(vec.x, vec.z));
	}
	else
	{
		owner->GetModel()->SetAnim(owner->GetAnim(kBackAvoid).c_str(), false);
		//モデルの向き
		owner->GetModel()->SetDir(Vector2(-vec.x, -vec.z));
	}

	auto status = owner->GetCharaStatus();
	//速度
	float baseSpeed = status->GetMS();
	m_endSpeed = baseSpeed + kSpeedDif;
	float startSpeed = MathSub::ClampFloat(baseSpeed - kSpeedDif,0, m_endSpeed);
	m_speed = startSpeed;

	//空中にいるなら重力の影響を受けない
	if (!owner->IsFloor())
	{
		auto rb = owner->GetRb();
		rb->SetIsGravity(false);
		rb->SetVecY(0.0f);
		//回避不可能(ジャンプをするか地面に付くと可能に)
		owner->SetIsAvoidable(false);
	}

	//無敵
	status->SetIsNoDamage(true);
}

PlayerStateAvoid::~PlayerStateAvoid()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	owner->GetRb()->SetIsGravity(true);

	//ジャスト回避に成功しているなら数フレーム間無敵
	if (m_isJustAvoid)
	{
		//無敵
		owner->SetNoDamageFrame(kNoDamageFrame);
	}
}

void PlayerStateAvoid::Init()
{
	//次の状態を自分の状態を入れる
	ChangeState(shared_from_this());
}

void PlayerStateAvoid::Update()
{
	if (m_pOwner.expired())return;
	auto owner = std::dynamic_pointer_cast<Player>(m_pOwner.lock());
	auto model = owner->GetModel();
	auto& app = Application::GetInstance();

	//入力
	auto& input = Input::GetInstance();

	//フレームカウント
	CountFrame();

	//ジャスト回避
	UpdateJustAvoid(owner, model, app);

	if (model->IsFinishAnim())
	{
		//ジャスト回避成功したとき
		if (m_isJustAvoid)
		{
			//攻撃
			if (input.IsBuffered("X"))
			{
				ChangeState(std::make_shared<PlayerStateLightAttack>(m_pOwner, false, true));
				return;
			}
		}

		//無敵解除
		owner->GetCharaStatus()->SetIsNoDamage(false);
		auto& input = Input::GetInstance();
		if (input.GetStickInfo().IsLeftStickInput())
		{
			//走る
			ChangeState(std::make_shared<PlayerStateMoving>(m_pOwner, true));
			return;
		}
		else
		{
			//待機
			ChangeState(std::make_shared<PlayerStateIdle>(m_pOwner));
			return;
		}
	}

	//回避移動
	MoveAvoid(app, owner);
}

void PlayerStateAvoid::InitJustAvoid(std::shared_ptr<Model> model, std::shared_ptr<Player> owner)
{
	//ジャスト回避成功
	m_isJustAvoid = true;

	//ジャスト回避
	model->SetAnim(owner->GetAnim(kJustAvoid).c_str(), false, 1.2f);

	//終了フレーム
	m_finishJustAvoid = model->GetTotalAnimFrame() - kFisishJustAvoidFrame;

	//スローモーション
	auto& app = Application::GetInstance();
	app.SetTimeScale(kSlowSpeed);
	//シェーダー
	app.GetPostProcess()->SetJustAvoidEffectTime(model->GetTotalAnimFrame());
	app.GetPostProcess()->AddPostEffectState(ShaderPostProcess::PostEffectState::JustAvoid);

	//フレームリセット
	m_frame = 0.0f;

	//無敵
	owner->GetCharaStatus()->SetIsNoDamage(true);
}

void PlayerStateAvoid::UpdateJustAvoid(std::shared_ptr<Player> owner, std::shared_ptr<Model> model, Application& app)
{
	if (!m_isJustAvoid)
	{
		//ジャスト回避フレーム内で攻撃を受けたら
		if (m_frame <= kJustFrame && owner->GetCharaStatus()->IsHit())
		{
			//ジャスト回避成功時に初期化
			InitJustAvoid(model, owner);
		}
	}
	else
	{
		//終了フレーム
		if (m_finishJustAvoid < m_frame)
		{
			//元に戻す
			app.SetTimeScale(1.0f);
			//無効
			owner->DisableIsMyScale();
		}
	}
}

void PlayerStateAvoid::MoveAvoid(Application& app, std::shared_ptr<Player> owner)
{
	float timeScale = app.GetTimeScale();
	m_speed = MathSub::Lerp(m_speed, m_endSpeed, kLerpSpeedRate);
	Vector3 vec = m_avoidDir * m_speed * timeScale;
	//移動
	owner->GetRb()->SetMoveVec(vec);
}