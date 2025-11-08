#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateFall.h"
#include "Weapon/Weapon.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/EnemyBase.h"
#include "../../../../General/game.h"
#include "../../../../General/Collision/CapsuleCollider.h"
#include "../../../../General/Collision/Rigidbody.h"
#include "../../../../General/Collision/Collidable.h"
#include "../../../../General/CharaStatus.h"
#include "../../../../General/Input.h"
#include "../../../../General/Model.h"
#include "../../../../General/Animator.h"
#include "../../ActorManager.h"
#include "../../../Attack/AttackManager.h"
#include "../../../Camera/PlayerCamera.h"
#include "../../../../General/CSV/CSVDataLoader.h"
#include "../../../../General/CSV/CSVData.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>
#include "../../../../General/ShaderPostProcess.h"
#include "../../../../Main/Application.h"
#include "../../../../General/AssetManager.h"
#include "../../../../Game/UI/Player/PlayerHPUI.h"
#include "../../../../Game/UI/UIManager.h"

namespace
{
	//プレイヤーのパスデータ数
	constexpr int kPathNum = 3;

	//ジャンプの最大数
	constexpr int kMaxJumpNum = 2;

	//武器の持ち手
	constexpr int kHandIndex = 295;
	//背中
	constexpr int kBackLightIndex = 302;
	constexpr int kBackBigIndex = 304;

	//武器を収めるまでのフレーム
	constexpr float kPutAwayFrame = 60.0f * 5.0f;

	//ヒットグリッジ(通常)
	constexpr float kGlitchFrame = 20.0f;
	constexpr float kGlitchScale = 100.0f;
	constexpr float kGlitchSpeed = 10.0f;
	constexpr float kGlitchStrengt = 100.0f;
	//ヒットグリッジ(ピンチ)
	constexpr float kPinchGlitchScale = 100.0f;
	constexpr float kPinchGlitchSpeed = 10.0f;
	constexpr float kPinchGlitchStrengt = 300.0f;
	constexpr float kGlitchPinchFrame = 10.0f;
	constexpr float kGlitchStartFrame = 230.0f;
	constexpr float kGlitchEndFrame = kGlitchStartFrame + kGlitchFrame;

	//落下した際のリスポーン地点の位置調整
	constexpr float kReturnPushBackDistance = 500.0f;
}

Player::Player(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	CharacterBase(actorData,charaStatusData,Shape::Capsule,pActorManager),
	m_jumpNum(0),
	m_isAvoidable(true),
	m_isJustAvoided(false),
	m_noDamageFrame(0.0f),
	m_putAwayCountFrame(0.0f),
	m_haveWeaponType(AnimData::WeaponType::None),
	m_isAirAttacked(false),
	m_isDraw(true),
	m_glitchFrame(0.0f),
	m_glitchCountFrame(0.0f),
	m_glitchScale(0.0f),
	m_glitchSpeed(0.0f),
	m_glitchkStrengt(0.0f),
	m_totalJustAvoidNum(0),
	m_isHitGlitch(false),
	m_isGoal(false),
	m_respawnPos()
{
}

Player::~Player()
{
}

void Player::Init()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_actorData->GetCSVPathData().c_str()).front()->GetData();

	assert(pathData.size() >= kPathNum);
	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str(),pathData[2].c_str());

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->GetCollRadius());

	//体力
	auto playerHPUI = std::make_shared<PlayerHPUI>(m_charaStatus);
	playerHPUI->Init();

	//初期リスポーン地点
	SetRespawnPos(GetPos());
}


void Player::Update()
{
	//入力の取得
	auto& input = Input::GetInstance();

	//ターゲットの検索
	if (!m_pActorManager.expired() && !GetPlayerCamera().expired())
	{
		auto actorM = m_pActorManager.lock();
		auto camera = GetPlayerCamera().lock();
		SearchTarget(input, camera,actorM->GetEnemyManager()->GetEnemys());
	}
	


#if _DEBUG
	if (input.IsTrigger("FullRecovery"))
	{
		//復活
		m_charaStatus->FullRecovery();
		//数フレーム無敵
		SetNoDamageFrame(60.0f);
	}
	
	if (input.IsTrigger("Pinch"))
	{
		//ピンチに
		m_charaStatus->SetNowHP(m_charaStatus->GetMaxHP() / 3);
	}

	////タイムスケール
	//if (input.IsTrigger("Y"))
	//{
	//	m_rb->SetIsMyTimeScale(!m_rb->IsMyTimeScale());
	//	if (m_rb->IsMyTimeScale())
	//	{
	//		SetTimeScale(0.5f);
	//	}
	//	else
	//	{
	//		SetTimeScale(1.0f);
	//	}
	//}

	//メイン攻撃
	//if (input.IsTrigger("X"))
	//{
	//	HaveLightSword();
	//}
	////サブ攻撃
	//if (input.IsTrigger("Y"))
	//{
	//	HaveBigSword();
	//}


#endif
	//体力に応じてポストエフェクトをかける
	UpdateHit();

	//武器を収める
	UpdatePutAwayWeapon();


	//共通処理
	CharacterBase::Update();

	//ジャスト回避無敵時間
	UpdateJustAvoid();

	//武器を描画するか
	if (!m_pBigSword.expired() && !m_pLightSword.expired())
	{
		m_pBigSword.lock()->SetIsDraw(m_isDraw);
		m_pLightSword.lock()->SetIsDraw(m_isDraw);
	}
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
		0x00ff00,
		0x00ff00,
		false
	);
	//衝突判定
	DrawSphere3D(
		m_respawnPos.ToDxLibVector(),
		m_actorData->GetCollRadius(),
		16,
		0x00ff00,
		0x00ff00,
		false
	);
#endif
	if (m_isDraw)
	{
		m_model->Draw();
	}
}

void Player::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->GetCollHeight();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
	//ジャンプ回数リセット
	if (m_isFloor)
	{
		m_jumpNum = 0;
	}
	//カメラに情報を渡す
	if (!GetPlayerCamera().expired())
	{
		auto camera = GetPlayerCamera().lock();
		camera->SetPlayerPos(GetCenterPos());
		camera->SetPlayerVec(m_rb->GetMoveVec());
		camera->SetPlayerDir(m_model->GetDir());
	}

	bool isHit = m_charaStatus->IsHit();
	if (isHit)
	{
#if _DEBUG
		printf("プレイヤーの残りの体力 : %d\n", m_charaStatus->GetNowHP());
#endif
	}
}


void Player::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
}

Vector3 Player::GetCenterPos() const
{
	Vector3 centerPos = (m_rb->GetPos() + std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetEndPos()) * 0.5f;
	return centerPos;
}

Quaternion Player::GetCameraRot() const
{
	Quaternion q = Quaternion::IdentityQ();
	if (!m_pActorManager.expired())
	{
		auto actorM = m_pActorManager.lock();
		if (!actorM->GetPlayerCamera().expired())
		{
			q = actorM->GetPlayerCamera().lock()->GetRotH();
		}
	}
	return q;
}

void Player::AddJumpNum()
{
	++m_jumpNum;
	m_jumpNum = MathSub::ClampInt(m_jumpNum, 0, kMaxJumpNum);
}

bool Player::IsJumpable() const
{
	return m_jumpNum < kMaxJumpNum;
}

bool Player::IsFall() const
{
	return m_rb->m_vec.y < 0.0f;
}

void Player::SetNoDamageFrame(float m_frame)
{
	m_noDamageFrame = m_frame;
	m_isJustAvoided = true;
}

void Player::UpdateJustAvoid()
{
	//ジャスト回避後
	if (m_isJustAvoided)
	{
		m_noDamageFrame -= GetTimeScale();
		m_charaStatus->SetIsNoDamage(true);
		//無敵時間が切れて今回避状態ではないなら無敵解除
		if (m_noDamageFrame <= 0.0f && std::dynamic_pointer_cast<PlayerStateAvoid>(m_state) == nullptr)
		{
			m_isJustAvoided = false;
			m_charaStatus->SetIsNoDamage(false);
		}
	}
#if _DEBUG
	/*if (m_charaStatus->IsNoDamage())
	{
		printf("今無敵です\n");
	}
	else
	{
		printf("今無敵ではない\n");
	}*/
#endif
}

void Player::SetSword(std::weak_ptr<Weapon> weapon, bool isLightSword)
{
	if (weapon.expired())return;
	auto sword = weapon.lock();
	sword->SetOwnerHandle(m_model->GetModelHandle());
	if (isLightSword)
	{
		m_pLightSword = weapon;
		sword->SetFrameIndex(kHandIndex, kBackLightIndex);
	}
	else
	{
		m_pBigSword = weapon;
		sword->SetFrameIndex(kHandIndex, kBackBigIndex);
	}
	
}

std::weak_ptr<Weapon> Player::GetWeapon(AnimData::WeaponType type) const
{
	if (type == AnimData::WeaponType::LightSword)
	{
		return m_pLightSword;
	}
	else
	{
		return m_pBigSword;
	}
}

void Player::HaveLightSword()
{
	//片手剣を持つ
	if (m_pLightSword.expired() || m_pBigSword.expired())return;

	auto lightSword = m_pLightSword.lock();
	auto bigSword = m_pBigSword.lock();

	lightSword->SetIsBattle(true);
	bigSword->SetIsBattle(false);

	//武器を収めるまでのフレーム
	m_putAwayCountFrame = kPutAwayFrame;

	//片手剣を持っている
	m_haveWeaponType = AnimData::WeaponType::LightSword;
}

void Player::HaveBigSword()
{
	//大剣を持つ
	if (m_pLightSword.expired() || m_pBigSword.expired())return;

	auto lightSword = m_pLightSword.lock();
	auto bigSword = m_pBigSword.lock();

	lightSword->SetIsBattle(false);
	bigSword->SetIsBattle(true);

	//武器を収めるまでのフレーム
	m_putAwayCountFrame = kPutAwayFrame;

	//大剣を持っている
	m_haveWeaponType = AnimData::WeaponType::BigSword;
}

void Player::PutAwaySword()
{
	//武器を収める
	if (m_pLightSword.expired() || m_pBigSword.expired())return;

	auto lightSword = m_pLightSword.lock();
	auto bigSword = m_pBigSword.lock();

	lightSword->SetIsBattle(false);
	bigSword->SetIsBattle(false);

	//武器を持っていない
	m_haveWeaponType = AnimData::WeaponType::None;
}

std::string Player::GetAnim(std::wstring state)const
{
	std::string path = "Player|";

	//探す
	return CharacterBase::GetAnim(state, path, m_haveWeaponType);
}

bool Player::IsGliding() const
{
	auto state = std::dynamic_pointer_cast<PlayerStateFall>(m_state);
	if (state != nullptr)
	{
		return state->IsGliding();
	}
	return false;
}


float Player::GetRadius() const
{
	return std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->GetRadius();
}

void Player::SetRespawnPos(Vector3 pos)
{
	Vector3 respownPos = pos;
	respownPos.y += m_actorData->GetCollRadius();
	m_respawnPos = respownPos;
}

void Player::Respawn()
{
	m_rb->SetPos(m_respawnPos);
	Vector3 endPos = m_rb->m_pos;
	endPos.y += m_actorData->GetCollHeight();
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);
	m_rb->SetVec(Vector3::Zero());
}

std::weak_ptr<PlayerCamera> Player::GetPlayerCamera() const
{
	std::weak_ptr<PlayerCamera> camera;
	if (!m_pActorManager.expired())
	{
		auto actorM = m_pActorManager.lock();
		if (!actorM->GetPlayerCamera().expired())
		{
			camera = actorM->GetPlayerCamera();
		}
	}
	return camera;
}

void Player::SearchTarget(Input& input, std::shared_ptr<PlayerCamera> camera, const std::list<std::shared_ptr<EnemyBase>>& enemys)
{
	//ターゲットがいなくなったら発見フラグをfalseにする
	if(m_targetInfo.m_pTarget.expired())
	{
		ResetTarget(camera);
	}
	else
	{
		auto target = std::dynamic_pointer_cast<EnemyBase>(m_targetInfo.m_pTarget.lock());

		//アクティブじゃないなら解除
		if(!target->IsActive())
		{
			ResetTarget(camera);
		}
		else
		{
			//ターゲットがいるなら距離を確認
			Vector3 toTarget = target->GetNextPos() - GetPos();
			float distance = toTarget.Magnitude();
			//範囲外になったラ解除
			if (distance > m_charaStatus->GetSearchRange())
			{
				ResetTarget(camera);
			}
		}
	}

	//LBでロックオン開始
	if (input.IsTrigger("LB"))
	{
		//ロックオン中なら解除
		if (camera->IsLockOn())
		{
			ResetTarget(camera);
		}
		//ロックオンしていないなら開始
		else
		{
			//プレイヤーの座標
			Vector3 playerPos = GetPos();

			//最も近い敵を探す
			std::shared_ptr<EnemyBase> nearestEnemy = nullptr;
			float minDis = m_charaStatus->GetSearchRange(); //索敵範囲
			bool isFind = false;

			for (auto enemy : enemys)
			{
				//活動していないなら無視
				if (!enemy->IsActive())continue;

				Vector3 enemyPos = enemy->GetNextPos();
				Vector3 toEnemy = enemyPos - playerPos;
				float distance = toEnemy.Magnitude();
				if (distance < minDis)
				{
					//距離の更新
					minDis = distance;
					//敵を保持
					nearestEnemy = enemy;
					//発見
					isFind = true;
				}
			}
			//ロックオン開始
			if (isFind)
			{
				camera->StartLockOn(nearestEnemy);
				m_targetInfo.m_pTarget = nearestEnemy;
			}
			m_targetInfo.m_isFound = isFind;
		}
	}
}

void Player::ResetTarget(std::shared_ptr<PlayerCamera> camera)
{
	m_targetInfo.m_isFound = false;
	if (camera->IsLockOn())
	{
		//解除
		camera->EndLockOn();
	}
}

void Player::UpdateHit()
{
	//グリッジ
	auto& app = Application::GetInstance();
	auto& postEff = app.GetPostProcess();

	//ダメージを受けたら
	if (m_charaStatus->IsHit() && !m_charaStatus->IsNoDamage())
	{
		//グリッジ
		postEff->AddPostEffectState(ShaderPostProcess::PostEffectState::Glitch);

		//初期化
		m_glitchFrame = kGlitchFrame;
		m_glitchScale = kGlitchScale;
		m_glitchSpeed = kGlitchSpeed;
		m_glitchkStrengt = kGlitchStrengt;

		//攻撃を喰らった
		m_isHitGlitch = true;

		//カウントリセット
		m_glitchCountFrame = 0.0f;
	}

	//カウント
	m_glitchCountFrame += GetTimeScale();

	//攻撃を喰らった際の処理
	if (m_isHitGlitch)
	{
		//終了フレーム
		if (m_glitchCountFrame >= m_glitchFrame || m_charaStatus->IsNoDamage())
		{
			//グリッジを削除
			postEff->SubPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
			//リセット
			m_isHitGlitch = false;
		}
		else
		{
			//グリッジフレーム間の処理
			//だんだん弱く
			float rate = m_glitchCountFrame / m_glitchFrame;
			postEff->SetBlockScele(MathSub::Lerp(m_glitchScale, 0.0f, rate));
			postEff->SetNoiseSpeed(MathSub::Lerp(m_glitchSpeed, 0.0f, rate));
			postEff->SetShakeStrength(MathSub::Lerp(m_glitchkStrengt, 0.0f, rate));
		}
	}
	else
	{
		//ピンチ
		if (m_charaStatus->IsPinchHP())
		{
			//グレーに
			postEff->AddPostEffectState(ShaderPostProcess::PostEffectState::Gray);
			//グリッジ
			postEff->AddPostEffectState(ShaderPostProcess::PostEffectState::Glitch);
			if (m_glitchCountFrame >= kGlitchEndFrame)
			{
				m_glitchCountFrame = 0.0f;
			}
			else if (m_glitchCountFrame >= kGlitchStartFrame)
			{
				postEff->SetShakeStrength(kPinchGlitchStrengt);
				postEff->SetBlockScele(kPinchGlitchScale);
				postEff->SetNoiseSpeed(kPinchGlitchSpeed);
			}
			else
			{
				postEff->SetShakeStrength(0.0f);
				postEff->SetBlockScele(0.0f);
				postEff->SetNoiseSpeed(0.0f);
			}
		}
		//通常
		else
		{
			m_glitchCountFrame = 0.0f;
		}
	}
}

void Player::UpdatePutAwayWeapon()
{
	//武器を収める
	if (m_putAwayCountFrame <= 0.0f)
	{
		m_putAwayCountFrame = 0.0f;
		PutAwaySword();
	}
	else
	{
		//カウント
		m_putAwayCountFrame -= GetTimeScale();
	}
}

bool Player::IsWait()const
{
	return m_state->IsWait();
}