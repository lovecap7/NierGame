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
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
    //ステータス
    std::shared_ptr<CharaStatus> m_charaStatus;
};

