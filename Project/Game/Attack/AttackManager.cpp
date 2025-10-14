#include "AttackManager.h"
#include <DxLib.h>
#include <cassert>
#include "../../General/CSV/CSVDataLoader.h"
#include "../../General/CSV/ActorData.h"
#include "AttackBase.h"

AttackManager::AttackManager()
{

}

AttackManager::~AttackManager()
{
}

//アクターを追加
void AttackManager::Entry(std::shared_ptr<AttackBase> attack)
{
	//すでに登録されているならしない
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it != m_attacks.end())return;
	//攻撃の初期化
	attack->Init();
	//攻撃を追加
	m_attacks.emplace_back(attack);
}

void AttackManager::Exit(std::shared_ptr<AttackBase> attack)
{
	//登録されていないならしない
	auto it = std::find(m_attacks.begin(), m_attacks.end(), attack);
	if (it == m_attacks.end())return;
	attack->End();
	m_attacks.erase(it);
}


void AttackManager::Init()
{
}

void AttackManager::Update()
{
	//攻撃の更新
	for (auto& attack : m_attacks)
	{
		attack->Update();
	}
	//削除チェック
	CheckDelete();
}

void AttackManager::Draw() const
{
	//描画
	for (auto& attack : m_attacks)
	{
		attack->Draw();
	}
}

void AttackManager::End()
{
}

//消滅フラグをチェックして削除
void AttackManager::CheckDelete()
{
	std::list<std::shared_ptr<AttackBase>> deleteAttack;
	for (int i = 0;i < 3;++i)
	{
		bool isOneMore = false;
		auto thisPointer = shared_from_this();
		for (auto& attack : m_attacks)
		{
			bool isDead = attack->IsDelete();//死亡したかをチェック
			if (isDead)
			{
				isOneMore = true;
				//削除候補
				deleteAttack.emplace_back(attack);
			}
		}
		//削除
		for (auto& actor : deleteAttack)
		{
			thisPointer->Exit(actor);
		}
		deleteAttack.clear();
		if (!isOneMore)break;
	}
}