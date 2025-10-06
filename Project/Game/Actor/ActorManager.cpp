#include "ActorManager.h"
#include "Actor.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "DebugActor/DebugPlayer.h"
#include "Stage/StageObject.h"


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
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update();
	}
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

void ActorManager::CreateActorCSV(const char* path)
{
	//CSVを読み込む
	std::shared_ptr<CSVDataLoader> csvLoader = std::make_shared<CSVDataLoader>();
	auto csvDatas = csvLoader->LoadCSV(path);
	//アクターの作成
	for (auto& data : csvDatas)
	{
		//データを変換
		auto actorData = std::make_shared<ActorData>(data);
		//アクターを作成
		std::shared_ptr<Actor> actor;
		if (actorData->m_actorType == ActorData::ActorType::Character)
		{
			//キャラクター
			actor = std::make_shared<DebugPlayer>(actorData, shared_from_this());
		}
		else if (actorData->m_actorType == ActorData::ActorType::Stage)
		{
			//ステージ
			actor = std::make_shared<StageObject>(actorData, shared_from_this());
		}
		else if (actorData->m_actorType == ActorData::ActorType::Attack)
		{
			//攻撃
			actor = std::make_shared<DebugPlayer>(actorData, shared_from_this());
		}
		Entry(actor);
	}
}
