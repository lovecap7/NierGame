#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include "../../../../General/CSV/PlayerAnimData.h"
#include "../../../../General/CSV/AttackData.h"
#include <memory>
#include <map>
#include <string>

class ActorManager;
class CharacterStateBase;
class CharaStatusData;
class Input;
class ActorData;
class PlayerCamera;
class Weapon;
class CSVDataLoader;
class AttackBase;
class Player :
	public CharacterBase
{
public:
	Player(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData, std::weak_ptr<ActorManager> pActorManager);
	virtual ~Player();
	//初期化処理
	void Init()override;
	//更新処理
	void Update() override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//終了処理
	void End()override;
	//ステートにアクセスさせる関数
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
	CollisionState GetCollState()const { return m_collState; };
	//カメラの回転量
	Quaternion GetCameraRot()const;

	//ジャンプ回数
	int GetJumpNum() const { return m_jumpNum; };
	void AddJumpNum();
	//ジャンプ可能
	bool IsJumpable()const;

	//回避可能
	bool IsAvoidable()const { return m_isAvoidable; };
	void SetIsAvoidable(bool isAvoidable){ m_isAvoidable = isAvoidable; };

	//落下しているか
	bool IsFall()const;

	//無敵時間
	void SetNoDamageFrame(float m_frame);
	//ジャスト回避後の無敵フレーム
	void UpdateJustAvoid();

	//持ってる武器情報
	PlayerAnimData::WeaponType GetHaveWeaponType()const { return m_haveWeaponType; };

	//武器の参照
	void SetSword(std::weak_ptr<Weapon> weapon,bool isLightSword);
	std::weak_ptr<Weapon> GetWeapon(PlayerAnimData::WeaponType type)const;

	//片手剣を持つ
	void HaveLightSword();
	//大剣を持つ
	void HaveBigSword();
	//武器を収める
	void PutAwaySword();

	//アニメーションデータ
	std::string GetAnim(std::wstring state)const;
	//攻撃データ
	std::shared_ptr<AttackData> GetAttackData(std::wstring attackName)const;

	//攻撃の登録
	void SetAttack(std::shared_ptr<AttackBase> attack);

private:
	//アニメーションデータをCSVから読み込む
	void InitAnimData(std::shared_ptr<CSVDataLoader> csvLoader);
	//攻撃データを読み込む
	void InitAttackData(std::shared_ptr<CSVDataLoader> csvLoader);
	//カメラ
	std::weak_ptr<PlayerCamera> GetPlayerCamera()const;
private:
	//ジャンプ回数
	int m_jumpNum;
	//回避可能
	bool m_isAvoidable;

	//無敵時間(ジャスト回避成功時数フレーム無敵)
	bool m_isJustAvoided;
	float m_noDamageFrame;

	//武器
	std::weak_ptr<Weapon> m_pLightSword;
	std::weak_ptr<Weapon> m_pBigSword;

	//武器の状態
	PlayerAnimData::WeaponType m_haveWeaponType;

	//武器を収めるまでのフレームをカウント
	float m_putAwayCountFrame;

	//アニメーションデータ
	std::vector<std::shared_ptr<PlayerAnimData>> m_animDatas;
	//攻撃データ
	std::vector<std::shared_ptr<AttackData>> m_attackDatas;
};

