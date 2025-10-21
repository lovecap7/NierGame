#pragma once
#include "../Actor.h"
#include <memory>
#include <map>
#include <string>
class CharacterStateBase;
class ActorManager;
class ActorData;
class CharaStatus;
class CharaStatusData;
class AttackBase;
class CSVDataLoader;
class AttackData;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData,Shape shape, std::weak_ptr<ActorManager> pActorManager);
	virtual ~CharacterBase() {};
    //ステータス
    std::shared_ptr<CharaStatus> GetCharaStatus()const;

    //攻撃の登録
    void SetAttack(std::shared_ptr<AttackBase> attack);
    //攻撃データ
    std::shared_ptr<AttackData> GetAttackData(std::wstring attackName)const;
protected:
    //攻撃データを読み込む
    void InitAttackData(std::shared_ptr<CSVDataLoader> csvLoader, std::string path);

protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
    //ステータス
    std::shared_ptr<CharaStatus> m_charaStatus;
    //攻撃データ
    std::vector<std::shared_ptr<AttackData>> m_attackDatas;
};

