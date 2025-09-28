#include "ActorManager.h"
#include "Actor.h"
#include <DxLib.h>
#include <cassert>


ActorManager::ActorManager():
	m_actorId(0)
{
	
}

ActorManager::~ActorManager()
{
}

//アクターを追加
void ActorManager::Entry(std::shared_ptr<Actor> actor)
{
	//すでに登録されているならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it != m_actors.end())return;
	//アクターの初期化
	actor->Init();
	//アクターのIDを設定
	actor->SetID(m_actorId);
	m_actorId++;
	//アクターを追加
	m_actors.emplace_back(actor);

	//プレイヤーなら
	if(actor->GetGameTag() == GameTag::Player)
	{
		//カメラにプレイヤーをセット
		m_pPlayer = actor;
	}
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//登録されていないならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it == m_actors.end())return;
	actor->End();
	m_actors.erase(it);
}


void ActorManager::Init()
{
}

void ActorManager::Update()
{
	//新規アクターの追加
	CheckNextAddActors();
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update();
	}
	//消滅フラグチェック
	CheckDeleteActors();
}

void ActorManager::Draw() const
{
	//アクターの描画
	for (auto& actor : m_actors)
	{
		actor->Draw();
	}
}

void ActorManager::End()
{
}

//新規アクターの追加予定を受け取る(public)
void ActorManager::AddNextActor(std::shared_ptr<Actor> actor)
{
	//追加予定のアクターを追加
	m_nextAddActors.emplace_back(actor);
}

//追加予定のアクターを実装
void ActorManager::CheckNextAddActors()
{
	for (auto& actor : m_nextAddActors)
	{
		Entry(actor);
	}
	m_nextAddActors.clear();//追加予定のアクターは消す
}

void ActorManager::CheckDeleteActors()
{
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (int i = 0; i < 3; ++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		for (auto& actor : m_actors)
		{
			bool isDead = actor->IsDelete();//死亡したかをチェック
			if (isDead)
			{
				isOneMore = true;
				//削除候補
				deleteActer.emplace_back(actor);
			}
		}
		//削除
		for (auto& actor : deleteActer)
		{
			thisPointer->Exit(actor);
		}
		deleteActer.clear();
		if (!isOneMore)break;
	}
}
