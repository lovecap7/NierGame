#pragma once
#include "../Actor.h"
#include "../../../General/CSVDatas.h"
#include <memory>
#include <map>
#include <string>
class CharacterStateBase;
class SE;
class Voice;
class CharacterBase abstract:
    public Actor
{
public:
	CharacterBase(Shape shape);
	virtual ~CharacterBase() {};
	//サウンド
	std::weak_ptr<SE>  CharacterOnceSE(std::string name);
	std::weak_ptr<SE>  CharacterLoopSE(std::string name);
	std::weak_ptr<Voice>  CharacterVC(std::string name);
	//キャラクターのデータ
	void SetCharaData(ActorData data);
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
	//サウンド
	std::map<std::string, int> m_soundHandles;
	//サウンド初期化
	virtual void InitSound() abstract;
	//サウンド終了処理
	virtual void EndSound();
	//視野角
	float m_viewingAngle;
	//索敵距離
	float m_searchDistance;
};

