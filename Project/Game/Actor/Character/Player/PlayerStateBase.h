#pragma once
#include "../CharacterStateBase.h"
#include <memory>
#include "../../../../General/Math/MyMath.h"
#include <string>
class Actor;
class Slash;
class GameCamera;
class ActorManager;
class PlayerStateBase abstract:
	public CharacterStateBase
{
public:
	PlayerStateBase(std::weak_ptr<Actor> player);
	virtual ~PlayerStateBase();
	//ランダムで攻撃ボイスを出す
	void RandAttackVC();
protected:
	//進行方向を返す関数
	virtual Vector3 GetForwardVec(const std::weak_ptr<GameCamera> camera);
};

//攻撃のデータを検索するときに使う
namespace
{
	std::string kPlayerName = "Player";
	std::string kNA1Name	= "NA1";
	std::string kNA2Name	= "NA2";
	std::string kNA3Name	= "NA3";
	std::string kNA4Name	= "NA4";
	std::string kCA1Name	= "CA1";
	std::string kCA2Name	= "CA2";
	std::string kCA3Name	= "CA3";
	std::string kULTName	= "ULT";
}

