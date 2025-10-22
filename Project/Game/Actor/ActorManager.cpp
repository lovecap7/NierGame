#include "ActorManager.h"
#include "Actor.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "../../General/CSV/CharaStatusData.h"
#include "../../General/CSV/PodData.h"
#include "Stage/StageObject.h"
#include "Character/CharacterBase.h"
#include "Character/Player/Player.h"
#include "Character/Enemy/NormalEnemy.h"
#include "Character/Player/Weapon/Weapon.h"
#include "Character/Player/Pod/Pod.h"
#include "Character/Enemy/EnemyManager.h"


namespace
{
	const std::wstring kCharaStatusDataName = L"CharaStatusData";
	const std::wstring kWeaponDataName = L"Player/WeaponData";
	const std::wstring kPodDataName = L"Player/PodData";
}

ActorManager::ActorManager()
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
	//アクターを追加
	m_actors.emplace_back(actor);

	//敵なら
	if (actor->GetGameTag() == GameTag::Enemy)
	{
		m_pEnemyManager->Entry(std::dynamic_pointer_cast<EnemyBase>(actor));
	}
}

void ActorManager::Exit(std::shared_ptr<Actor> actor)
{
	//登録されていないならしない
	auto it = std::find(m_actors.begin(), m_actors.end(), actor);
	if (it == m_actors.end())return;
	actor->End();
	m_actors.erase(it);

	//敵なら
	if (actor->GetGameTag() == GameTag::Enemy)
	{
		m_pEnemyManager->Exit(std::dynamic_pointer_cast<EnemyBase>(actor));
	}
}


void ActorManager::Init()
{
	//敵マネージャーの作成
	m_pEnemyManager = std::make_shared<EnemyManager>(shared_from_this());
}

void ActorManager::Update()
{
	//アクターの更新
	for (auto& actor : m_actors)
	{
		actor->Update();
	}
	//敵マネージャーの更新
	m_pEnemyManager->Update();
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
	//アクターの終了処理
	std::list<std::shared_ptr<Actor>> deleteActer;
	for (auto& actor : m_actors)
	{
		deleteActer.emplace_back(actor);
	}
	for (auto& actor : deleteActer)
	{
		Exit(actor);
	}
	deleteActer.clear();
}

void ActorManager::CreateActorCSV(const wchar_t* folderName, const wchar_t* fileName)
{
	//CSVを読み込む
	std::shared_ptr<CSVDataLoader> csvLoader = std::make_shared<CSVDataLoader>();
	std::wstring path = folderName + std::wstring(L"/") + fileName;
	auto csvDatas = csvLoader->LoadCSV(path.c_str());

	//キャラクターのステータスを読み込む
	std::wstring charaStatusPath = folderName + std::wstring(L"/") + kCharaStatusDataName;
	auto csvStatusDatas = csvLoader->LoadCSV(charaStatusPath.c_str());

	//アクターの作成
	for (auto& data : csvDatas)
	{
		if (!data)continue;
		//データを変換
		auto actorData = std::make_shared<ActorData>(data);
		//アクターを作成
		std::shared_ptr<Actor> actor;
		if (actorData->m_actorType == ActorData::ActorType::Character)
		{
			for (auto& statusData : csvStatusDatas)
			{
				auto tStatusData = std::make_shared<CharaStatusData>(statusData);
				//IDが一致するなら
				if (tStatusData->m_id == actorData->m_actorID)
				{
					//キャラクター作成
					actor = CreateChara(actorData->m_gameTag, actorData, tStatusData);
				}
			}
		}
		else if (actorData->m_actorType == ActorData::ActorType::Stage)
		{
			//ステージ
			actor = std::make_shared<StageObject>(actorData, shared_from_this());
		}
		Entry(actor);

		//プレイヤーなら
		if (actorData->m_gameTag == GameTag::Player)
		{
			//武器を持たせる
			SetUpPlayer(std::dynamic_pointer_cast<Player>(actor));
		}
		

	}
}

std::shared_ptr<CharacterBase> ActorManager::CreateChara(GameTag tag, std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData)
{
	std::shared_ptr<CharacterBase> chara;
	//プレイヤー
	if (tag == GameTag::Player)
	{
		chara = std::make_shared<Player>(actorData, charaStatusData, shared_from_this());
	}
	else if (tag == GameTag::Enemy)
	{
		//敵キャラクター
		//今回はノーマルエネミーのみ
		chara = std::make_shared<NormalEnemy>(actorData, charaStatusData, shared_from_this());
	}
	//nullチェック
	assert(chara);
	return chara;
}

void ActorManager::SetUpPlayer(std::shared_ptr<Player> player)
{
	//プレイヤーの参照
	m_pPlayer = player;

	//CSVを読み込む
	std::shared_ptr<CSVDataLoader> csvLoader = std::make_shared<CSVDataLoader>();
	std::wstring path = kWeaponDataName;
	auto csvDatas = csvLoader->LoadCSV(path.c_str());
	//武器作成
	for (auto& data : csvDatas)
	{
		if (!data)continue;
		//データを変換
		auto weaponData = std::make_shared<ActorData>(data);
		//武器を作成
		std::shared_ptr<Weapon> weapon = std::make_shared<Weapon>(weaponData,shared_from_this());

		//マネージャーにエントリー
		Entry(weapon);

		//武器をセット
		if (weaponData->m_name == L"LightSword")
		{
			//片手剣
			player->SetSword(weapon,true);
		}
		else
		{
			//大剣
			player->SetSword(weapon,false);
		}
	}
	//ポッドのデータ読み込み
	path = kPodDataName;
	csvDatas = csvLoader->LoadCSV(path.c_str());
	//データを変換
	auto podData = std::make_shared<PodData>(csvDatas.back());

	//必要な情報のみをセットしていく
	//アクターデータ
	auto podActorData = std::make_shared<ActorData>();
	podActorData->m_scale = podData->m_scale;
	podActorData->m_modelPath = podData->m_modelPath;
	auto podStatusData = std::make_shared<CharaStatusData>();
	podStatusData->m_at = podData->m_at;
	//ポッドを作成
	std::shared_ptr<Pod> pod = std::make_shared<Pod>(podActorData, podStatusData, shared_from_this(), player);

	//マネージャーにエントリー
	Entry(pod);
}
