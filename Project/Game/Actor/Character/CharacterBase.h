#pragma once
#include "../Actor.h"
#include <memory>
#include <map>
#include <string>
class CharacterStateBase;
class ActorManager;
class ActorData;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(std::shared_ptr<ActorData> actorData,Shape shape, std::weak_ptr<ActorManager> pActorManager);
	virtual ~CharacterBase() {};
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
};

