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
	m_haveWeaponType(AnimData::WeaponType::None),
	m_isAirAttacked(false),
	m_isDraw(true)
{
}

Player::~Player()
{
}

void Player::Init()
{
	//必要なパスを取得
	auto& csvLoader = CSVDataLoader::GetInstance();
	auto pathData = csvLoader.LoadCSV(m_actorData->m_csvPathData.c_str()).front()->GetData();

	assert(pathData.size() > 0);

	//共通初期化
	CharacterBase::Init(pathData[0].c_str(), pathData[1].c_str());
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
	//カメラに情報を渡す
	if (!GetPlayerCamera().expired())
	{
		auto camera = GetPlayerCamera().lock();
		camera->SetPlayerPos(m_rb->m_pos);
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
		//ターゲットがいるなら距離を確認
		auto target = m_targetInfo.m_pTarget.lock();
		Vector3 toTarget = target->GetNextPos() - GetPos();
		float distance = toTarget.Magnitude();
		//範囲外になったラ解除
		if (distance > m_charaStatus->GetSearchRange())
		{
			ResetTarget(camera);
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
