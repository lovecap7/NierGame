#pragma once
#include "../Actor.h"
#include <memory>
#include <map>
#include <string>
#include "../../../General/CSV/AnimData.h"
#include "../../../General/CharaStatus.h"
class CharacterStateBase;
class ActorManager;
class ActorData;
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

    //初期化処理
    void Init(std::wstring animPath, std::wstring attackPath);
	//更新処理
	virtual void Update() override;

    //ステートにアクセスさせる関数
    //リジッドボディ
    std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
    //コリジョン
    std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
    //コリジョンの状態を設定
    void SetCollState(CollisionState collState) { m_collState = collState; }
    CollisionState GetCollState()const { return m_collState; };

    //ステータス
    std::shared_ptr<CharaStatus> GetCharaStatus()const;
    //攻撃の登録
    void SetAttack(std::shared_ptr<AttackBase> attack);
    //攻撃データ
    std::shared_ptr<AttackData> GetAttackData(std::wstring attackName)const;
    //アニメーションの検索
    virtual std::string GetAnim(std::wstring state, std::string path = "", AnimData::WeaponType type = AnimData::WeaponType::None) const;

    //アーマーを変える
    void ChangeArmor(CharaStatus::Armor armor);
    //アーマーを元に戻す
    void InitArmor();
protected:
    //攻撃データを読み込む
    void InitAttackData(CSVDataLoader& csvLoader, std::wstring path);
    //アニメーションの読み込み
    void InitAnimData(CSVDataLoader& csvLoader, std::wstring path);
protected:
    //キャラクターの状態
    std::shared_ptr<CharacterStateBase> m_state;
    //ステータス
    std::shared_ptr<CharaStatus> m_charaStatus;
    //攻撃データ
    std::vector<std::shared_ptr<AttackData>> m_attackDatas;
    //アニメーションデータ
    std::vector<std::shared_ptr<AnimData>> m_animDatas;

};

