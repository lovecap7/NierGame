#include "Pod.h"
#include "PodStateIdle.h"
#include "../../CharacterStateBase.h"
#include "../../../ActorManager.h"
#include "../../../../Camera/PlayerCamera.h"
#include "../../../../../General/Model.h"
#include "../../../../../General/Input.h"
#include "../../../../../General/CSV/CharaStatusData.h"
#include "../../../../../General/CSV/ActorData.h"
#include "../../../../../General/CSV/PodData.h"
#include "../../../../../General/CSV/CSVDataLoader.h"
#include "../../../../Attack/BulletAttack.h"
#include <cassert>

namespace

{
	const std::wstring kAnimPath = L"Player/PodAnimData";
	const std::wstring kAttackPath = L"Player/PodAttackData";
}

Pod::Pod(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager, std::weak_ptr<Player> pPlayer) :
	CharacterBase(actorData, charaStatusData, Shape::Sphere, pActorManager),
	m_pPlayer(pPlayer),
	m_isGliding(false)
{
	m_tag = GameTag::Player;
}

Pod::~Pod()
{
}

void Pod::Init()
{
	//Physicsに登録
	CharacterBase::Init(kAnimPath, kAttackPath);
	//待機状態にする(最初はプレイヤー内で状態を初期化するがそのあとは各状態で遷移する
	auto thisPointer = std::dynamic_pointer_cast<Pod>(shared_from_this());
	m_state = std::make_shared<PodStateIdle>(thisPointer);
	//状態を変化する
	m_state->ChangeState(m_state);

	//弾の作成
	//攻撃データ
	auto attackData =GetAttackData(L"NormalShot");
	for (int i = 0; i < attackData->m_param1; i++)
	{
		auto bullet = std::make_shared<BulletAttack>(attackData, thisPointer);
		bullet->SetActive(false);		//更新と描画を止める
		m_bullets.emplace_back(bullet);
	}
}

void Pod::Update()
{
	//入力の取得
	auto& input = Input::GetInstance();

	//滑空状態かチェック
	if (!m_pPlayer.expired())
	{
		m_isGliding = m_pPlayer.lock()->IsGliding();
	}

	//共通処理
	CharacterBase::Update();
}

void Pod::OnCollide(const std::shared_ptr<Collidable> other)
{
}

void Pod::Draw() const
{
	m_model->Draw();
}

void Pod::Complete()
{
	m_rb->SetPos(m_rb->GetNextPos());
	//モデルの座標更新
	m_model->SetPos(m_rb->m_pos.ToDxLibVector());
	m_model->ApplyMat();
}

void Pod::End()
{
	//モデル削除
	m_model->End();
	//登録解除
	Collidable::End();
}
Vector3 Pod::GetPlayerPos() const
{
	if (m_pPlayer.expired())return Vector3::Zero();
	//座標
	return m_pPlayer.lock()->GetNextPos();
}

Vector3 Pod::GetPlayerDir() const
{
	if (m_pPlayer.expired())return Vector3::Forward();
	//座標
	return m_pPlayer.lock()->GetModel()->GetDir();
}

TargetInfo Pod::GetPlayerTargetInfo() const
{
	TargetInfo info;
	if (m_pPlayer.expired())return info;
	info = m_pPlayer.lock()->GetTargetInfo();
	//ターゲット
	return info;
}

Vector3 Pod::GetCameraDir() const
{
	if (m_pActorManager.expired())return Vector3::Zero();
	auto actorManager = m_pActorManager.lock();
	//カメラ
	if(actorManager->GetPlayerCamera().expired())return Vector3::Zero();
	//向き
	return actorManager->GetPlayerCamera().lock()->GetLook();
}

std::string Pod::GetAnim(std::wstring state) const
{
	std::string path = "RobotArmature|";
	//探す
	return CharacterBase::GetAnim(state, path);
}
