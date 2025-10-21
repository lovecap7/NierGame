#include "Player.h"
#include "PlayerStateBase.h"
#include "PlayerStateIdle.h"
#include "PlayerStateAvoid.h"
#include "PlayerStateFall.h"
#include "Weapon/Weapon.h"
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
#include "../../../../General/CSV/AttackData.h"
#include <DxLib.h>
#include <cmath>
#include <cassert>

namespace
{
	//ジャンプの最大数
	constexpr int kMaxJumpNum = 2;

	//武器の持ち手
	constexpr int kHandIndex = 295;
	//背中
	constexpr int kBackLightIndex = 302;
	constexpr int kBackBigIndex = 304;

	//武器を収めるまでのフレーム
	constexpr float kPutAwayFrame = 60.0f * 5.0f;
}

Player::Player(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager) :
	CharacterBase(actorData,charaStatusData,Shape::Capsule,pActorManager),
	m_jumpNum(0),
	m_isAvoidable(true),
	m_isJustAvoided(false),
	m_noDamageFrame(0.0f),
	m_putAwayCountFrame(0.0f),
	m_haveWeaponType(PlayerAnimData::WeaponType::None),
	m_isAirAttacked(false),
	m_isDraw(true)
{
}

Player::~Player()
{
}

void Player::Init()
{
	//Physicsに登録
	Collidable::Init();

	//CSVを読み込む
	auto csvLoader = std::make_shared<CSVDataLoader>();
	//アニメーションデータ
	InitAnimData(csvLoader);
	//攻撃データ
	InitAttackData(csvLoader, "Player/PlayerAttackData");

	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Player>(shared_from_this());
	m_state = std::make_shared<PlayerStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);
	//モデルの高さ調整
	m_model->SetModelHeightAdjust(-m_actorData->m_collRadius);
}


void Player::Update()
{
	//入力の取得
	auto& input = Input::GetInstance();

#if _DEBUG
	if (input.IsTrigger("FullRecovery"))
	{
		//復活
		m_charaStatus->FullRecovery();
		//数フレーム無敵
		SetNoDamageFrame(60.0f);
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

	//状態に合わせた更新
	m_state->Update();
	//状態が変わったかをチェック
	if (m_state != m_state->GetNextState())
	{
		//状態を変化する
		m_state = m_state->GetNextState();
		m_state->Init();
	}
	//アニメーションの更新
	m_model->Update();

	//ジャスト回避無敵時間
	UpdateJustAvoid();

	//状態のリセット
	m_charaStatus->InitHitState();

	//武器を描画するか
	if (!m_pBigSword.expired() && !m_pLightSword.expired())
	{
		m_pBigSword.lock()->SetIsDraw(m_isDraw);
		m_pLightSword.lock()->SetIsDraw(m_isDraw);
	}
}


void Player::OnCollide(const std::shared_ptr<Collidable> other)
{
	//デバッグ
	if (other->GetGameTag() == GameTag::Attack)
	{
		if (m_charaStatus->IsHitReaction())
		{
#if _DEBUG
			printf("残りの体力 : %d\n", m_charaStatus->GetNowHP());
#endif
		}
	}
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
	endPos.y += m_actorData->m_collHeight;
	std::dynamic_pointer_cast<CapsuleCollider>(m_collisionData)->SetEndPos(endPos);//カプセルの移動
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
	//ジャンプ回数リセット
	if (m_isFloor)
	{
		m_jumpNum = 0;
	}
	//カメラに位置を渡す
	if (!GetPlayerCamera().expired())
	{
		auto camera = GetPlayerCamera().lock();
		camera->SetPlayerPos(m_rb->m_pos);
		camera->SetPlayerVec(m_rb->GetMoveVec());
	}
}


void Player::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
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

std::weak_ptr<Weapon> Player::GetWeapon(PlayerAnimData::WeaponType type) const
{
	if (type == PlayerAnimData::WeaponType::LightSword)
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
	m_haveWeaponType = PlayerAnimData::WeaponType::LightSword;
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
	m_haveWeaponType = PlayerAnimData::WeaponType::BigSword;
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
	m_haveWeaponType = PlayerAnimData::WeaponType::None;
}

std::string Player::GetAnim(std::wstring state)const
{
	std::string path = "Player|";

	//探す
	for (auto& data : m_animDatas)
	{
		//条件に合うものがあったら
		if (data->m_stateName == state && data->m_weaponType == m_haveWeaponType)
		{
			path += data->m_animName;
			break;
		}
	}

	return path;
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
void Player::InitAnimData(std::shared_ptr<CSVDataLoader> csvLoader)
{
	auto datas = csvLoader->LoadCSV("Player/PlayerAnimData");
	//登録
	for (auto& data : datas)
	{
		std::shared_ptr<PlayerAnimData> animData = std::make_shared<PlayerAnimData>(data);
		m_animDatas.emplace_back(animData);
	}
}