#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateStart.h"
#include "PlayerStateRun.h"
#include "PlayerStateWin.h"
#include "../../Attack/AttackBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/HitPoints.h"
#include "../../../../General/AttackPoints.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/PolygonCollider.h"
#include "../../../../General/Collision/SphereCollider.h"
#include "../../../../General/Collision/ColliderBase.h"
#include "../../../../General/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../../../General/Effect/EffekseerManager.h"
#include "../../../../General/Effect/TrackActorEffect.h"
#include "../../../../General/Sound/SoundManager.h"
#include "../../ActorManager.h"
#include "../../../../Game/Camera/GameCamera/GameCamera.h"
#include "UltGage.h"
#include "../../../UI/UIManager.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//当たり判定
	const Vector3 kCapsuleHeight = { 0.0f,150.0f,0.0f };	//カプセルの上端
	constexpr float kCapsuleRadius = 20.0f;					//カプセルの半径
	//必殺技ゲージの最大値
	constexpr int kMaxUltGage = 100;
	//索敵の基準方向を決める際のモデルの向きと入力の向きの割合
	constexpr float kTargetSearchDirRate = 0.2f;
	//ダッシュ持続状態解除
	constexpr int kCancelRunFrame = 5;
	//落下したと判定するY座標
	constexpr float kStageFallY = -500.0f;
}

Player::Player(int modelHandle, Position3 firstPos) :
	CharacterBase(Shape::Capsule),
	m_stickVec(0.0f,0.0f),
	m_isRunKeep(false),
	m_cancelRunFrame(0),
	m_initPos{},
	m_isDead(false)
{
	//座標
	m_rb->m_pos = firstPos;
	//カプセルの終点の設定
	Vector3 endPos = firstPos;
	endPos += kCapsuleHeight; //カプセルの上端
	auto cap = std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData);
	cap->SetEndPos(endPos);
	cap->SetRadius(kCapsuleRadius);
	//モデル
	m_model = std::make_shared<Model>(modelHandle, firstPos.ToDxLibVector());
	//モデルの高さ調整(半径分だけモデルが浮いてるので)
	m_model->SetModelHeightAdjust(-kCapsuleRadius);
	//必殺技ゲージ
	m_ultGage = std::make_shared<UltGage>(kMaxUltGage);
	//体力ステータス
	m_hitPoints = std::make_shared<HitPoints>();
	//攻撃ステータス
	m_attackPoints = std::make_shared<AttackPoints>();
}

Player::~Player()
{
}

void Player::Init()
{
	//コンストラクタで全部初期化したかったけど
	//shared_from_this()を使う場合コンストラクタ時点では
	//メモリが確定していないので使うことができない
	//対策としてInitを使う

	//コライダブルの初期化
	AllSetting(CollisionState::Normal, Priority::Middle, GameTag::Player, false, false,true);
	//Physicsに登録
	Collidable::Init();
	//音初期化
	InitSound();
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateStart>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//初期化座標
	m_initPos = m_rb->m_pos;
	//プレイヤー関連のUIの準備
	UIManager::GetInstance().CreatePlayerUI(thisPointer);
}


void Player::Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager)
{
	//入力の取得
	auto& input = Input::GetInstance();

	//プレイヤーの移動やモデルの向きを更新するための
	//入力ベクトルを更新
	UpdatePlayerStickVec(input);
	//ターゲットを発見できたかをチェック
	auto target = actorManager.lock()->GetNearestEnemy();
	if (!target.expired())
	{
		TargetSearch(m_searchDistance, m_viewingAngle, target.lock()->GetPos());
	}
	//走りを継続するか
	CheckRunKeep();
	//必殺ゲージが最大の時エフェクトをつける
	CheckUltMax();
	//状態に合わせた更新
	m_state->Update(camera,actorManager);
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();
	//体力ステータスの更新
	m_hitPoints->Update();
	//攻撃ステータスの更新
	m_attackPoints->Update();

#if _DEBUG
	//ゲージマックス
	if (input.IsTrigger("Max"))
	{
		m_ultGage->AddUltGage(10000);
		m_hitPoints->Heal(10000);
	}
	//ボスの頭上に飛ぶ
	if (input.IsPress("BossArea"))
	{
		auto bossPos = actorManager.lock()->GetBoss().lock()->GetPos();
		m_rb->m_pos.y = bossPos.y + 100.0f;
		m_rb->m_pos.x = bossPos.x;
		m_rb->m_vec = Vector3::Zero();
	}
	//体力を0に
	if (input.IsPress("PlayerDead"))
	{
		m_hitPoints->Damage(999999);
	}
#endif
}

void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Player::Draw() const
{
#if _DEBUG
	//衝突判定
	DrawCapsule3D(
		m_rb->m_pos.ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos().ToDxLibVector(),
		std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius(),
		16,
		0xff0000,
		0xff0000,
		false//地面にいると塗りつぶされる
	);
	////探索範囲
	//DrawSphere3D(m_rb->m_pos.ToDxLibVector(), kSearchDistance, 4, 0x0000ff, 0x0000ff, false);
	////見てる方向
	//auto forward = m_model->GetDir();
	//forward = forward * kSearchDistance;
	////視野角
	//auto viewDir1 = Quaternion::AngleAxis(kSearchAngle / 2.0f, Vector3::Up()) * forward;
	//auto viewDir2 = Quaternion::AngleAxis(-kSearchAngle / 2.0f, Vector3::Up()) * forward;
	////描画
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + forward).ToDxLibVector(), 0xff0000);
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir1).ToDxLibVector(), 0xff00ff);
	//DrawLine3D(m_rb->m_pos.ToDxLibVector(), (m_rb->m_pos + viewDir2).ToDxLibVector(), 0xff00ff);
	////ターゲットとの距離
	//if (m_targetData.isHitTarget)
	//{
	//	printf("ターゲットとの距離 : %f\n", m_targetData.targetDis);
	//	DrawLine3D(m_rb->m_pos.ToDxLibVector(), m_targetData.targetPos.ToDxLibVector(), 0x0000ff);
	//}
#endif
	m_model->Draw();
}

void Player::Complete()
{
	//落下した際の処理
	if (m_rb->m_pos.y < kStageFallY)
	{
		m_rb->m_pos = m_initPos;
		m_rb->m_vec = Vector3::Zero();
	}

	m_rb->m_pos += m_rb->m_vec;//次の座標へ
	Vector3 endPos = m_rb->m_pos;
	endPos += kCapsuleHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
}

void Player::Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score)
{
	//死亡エフェクト
	EffekseerManager::GetInstance().CreateEffect("DeathEff", m_rb->m_pos);
}

void Player::End()
{
	m_model->End();
	EndSound();
	Collidable::End();
}
void Player::Revival()
{
	m_hitPoints->Heal(999999);
	m_isDead = false;
}

bool Player::IsStartAnim()
{
	//スタート状態以外はfalse
	if (std::dynamic_pointer_cast<PlayerStateStart>(m_state) == nullptr)
	{
		return false;
	}
	return true;
}

bool Player::IsClearAnim()
{
	//クリア状態以外はfalse
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	return true;
}

bool Player::IsFinishClearAnim()
{
	//勝利状態以外はfalse
	if (std::dynamic_pointer_cast<PlayerStateWin>(m_state) == nullptr)
	{
		return false;
	}
	//勝利状態の時にアニメーションが終了したらtrue
	return m_model->IsFinishAnim();
}

void Player::TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos)
{
	//リセット
	m_targetData.isHitTarget = false;
	//距離を確認
	auto toTarget = targetPos.XZ() - m_rb->GetPos().XZ();
	//向き
	auto dir = toTarget;
	//入力があるなら
	if (m_stickVec.Magnitude())
	{
		//入力との間のベクトルから索敵
		dir = Vector2::Lerp(m_model->GetDir().XZ(), m_stickVec, kTargetSearchDirRate);
	}
	if (toTarget.Magnitude() <= searchDistance)
	{
		//視野角内にターゲットがいるか
		auto angle = abs(Vector2::GetRad(m_model->GetDir().XZ(), dir));
		if (angle <= (searchAngle))
		{
			m_targetData.isHitTarget = true;
			m_targetData.targetPos = targetPos;
			m_targetData.targetDirXZ = dir.XZ().Normalize();
			m_targetData.targetDis = toTarget.Magnitude();
		}
	}
}

void Player::UpdatePlayerStickVec(Input& input)
{
	//スティックの向きを入れる(モデルのZ方向が反対なので)
	m_stickVec.x = static_cast<float>(input.GetStickInfo().leftStickX);
	m_stickVec.y = -static_cast<float>(input.GetStickInfo().leftStickY);
}

void Player::CheckRunKeep()
{
	//ダッシュ状態じゃないとき
	if (std::dynamic_pointer_cast<PlayerStateRun>(m_state) == nullptr)
	{
		++m_cancelRunFrame;
		//解除
		if (m_cancelRunFrame > kCancelRunFrame)
		{
			m_isRunKeep = false;
			m_cancelRunFrame = 0;
		}
	}
	else
	{
		m_cancelRunFrame = 0;
	}
}

void Player::CheckUltMax()
{
	//エフェクトをだしてなくて必殺ゲージ最大ならエフェクトをつける
	if (m_ultMaxEff.expired() && m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff = EffekseerManager::GetInstance().CreateTrackActorEffect("UltGageMaxEff", std::dynamic_pointer_cast<Actor>(shared_from_this()));
	}
	else if (!m_ultMaxEff.expired() && !m_ultGage->IsMaxUlt())
	{
		m_ultMaxEff.lock()->Delete();
	}
}

void Player::InitSound()
{
	//プレイヤーSE
	m_soundHandles["NA"] = LoadSoundMem(L"Data/Sound/SE/Player/NA.mp3");
	m_soundHandles["CA"] = LoadSoundMem(L"Data/Sound/SE/Player/CA.mp3");
	m_soundHandles["UltCharge"] = LoadSoundMem(L"Data/Sound/SE/Player/UltCharge.mp3");
	m_soundHandles["UltLaser"] = LoadSoundMem(L"Data/Sound/SE/Player/UltLaser.mp3");
	m_soundHandles["UltShot"] = LoadSoundMem(L"Data/Sound/SE/Player/UltShot.mp3");
	m_soundHandles["CARankUp"] = LoadSoundMem(L"Data/Sound/SE/Player/CARankUp.mp3");
	m_soundHandles["CARankMax"] = LoadSoundMem(L"Data/Sound/SE/Player/CARankMax.mp3");
	m_soundHandles["CACharge"] = LoadSoundMem(L"Data/Sound/SE/Player/CACharge.mp3");
	//VC
	m_soundHandles["Attack1"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack1.mp3");
	m_soundHandles["Attack2"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack2.mp3");
	m_soundHandles["Attack3"] = LoadSoundMem(L"Data/Sound/VC/Player/Attack3.mp3");
	m_soundHandles["Damage1"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage1.mp3");
	m_soundHandles["Damage2"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage2.mp3");
	m_soundHandles["Damage3"] = LoadSoundMem(L"Data/Sound/VC/Player/Damage3.wav");
	m_soundHandles["Ult"] = LoadSoundMem(L"Data/Sound/VC/Player/Ult.mp3");
	m_soundHandles["Dead"] = LoadSoundMem(L"Data/Sound/VC/Player/Dead.wav");
	m_soundHandles["Result"] = LoadSoundMem(L"Data/Sound/VC/Player/Result.mp3");
	m_soundHandles["Rolling1"] = LoadSoundMem(L"Data/Sound/VC/Player/Rolling1.wav");
	m_soundHandles["Rolling2"] = LoadSoundMem(L"Data/Sound/VC/Player/Rolling2.wav");
	m_soundHandles["Start"] = LoadSoundMem(L"Data/Sound/VC/Player/Start.mp3");
	m_soundHandles["Jump"] = LoadSoundMem(L"Data/Sound/VC/Player/Jump.mp3");
	//ロードに成功したかチェック
	for (const auto& [key, value] : m_soundHandles) {
		assert(value >= 0);
	}
}