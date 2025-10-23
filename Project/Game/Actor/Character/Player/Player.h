#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include "../../../../General/CSV/AnimData.h"
#include "../../../../General/CSV/AttackData.h"
#include <memory>
#include <map>
#include <string>
#include <list>

class ActorManager;
class CharacterStateBase;
class CharaStatusData;
class Input;
class ActorData;
class PlayerCamera;
class Weapon;
class CSVDataLoader;
class AttackBase;
class EnemyBase;
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

	//空中攻撃可能か
	bool IsAirAttacked()const { return m_isAirAttacked; };
	void SetIsAirAttacked(bool isAttack){ m_isAirAttacked = isAttack; };

	//落下しているか
	bool IsFall()const;

	//無敵時間
	void SetNoDamageFrame(float m_frame);
	//ジャスト回避後の無敵フレーム
	void UpdateJustAvoid();

	//持ってる武器情報
	AnimData::WeaponType GetHaveWeaponType()const { return m_haveWeaponType; };

	//武器の参照
	void SetSword(std::weak_ptr<Weapon> weapon,bool isLightSword);
	std::weak_ptr<Weapon> GetWeapon(AnimData::WeaponType type)const;

	//片手剣を持つ
	void HaveLightSword();
	//大剣を持つ
	void HaveBigSword();
	//武器を収める
	void PutAwaySword();

	//アニメーションの検索
	std::string GetAnim(std::wstring state) const;

	//滑空状態か
	bool IsGliding()const;

	//描画
	void SetIsDraw(bool isDraw) { m_isDraw = isDraw; };

	//ターゲット情報
	struct TargetInfo
	{
		TargetInfo():
			m_isFound(false),
			m_pTarget()
		{
		}
		//発見したかどうか
		bool m_isFound;
		//参照
		std::weak_ptr<EnemyBase> m_pTarget;
	};
	TargetInfo GetTargetInfo()const { return m_targetInfo; };
private:
	//カメラ
	std::weak_ptr<PlayerCamera> GetPlayerCamera()const;
	//ターゲットを探す
	void SearchTarget(Input& input, std::shared_ptr<PlayerCamera> camera, const std::list<std::shared_ptr<EnemyBase>>& enemys);
private:
	//ジャンプ回数
	int m_jumpNum;
	//回避可能
	bool m_isAvoidable;
	//空中攻撃を行ったか
	bool m_isAirAttacked;

	//無敵時間(ジャスト回避成功時数フレーム無敵)
	bool m_isJustAvoided;
	float m_noDamageFrame;

	//武器
	std::weak_ptr<Weapon> m_pLightSword;
	std::weak_ptr<Weapon> m_pBigSword;

	//武器の状態
	AnimData::WeaponType m_haveWeaponType;

	//武器を収めるまでのフレームをカウント
	float m_putAwayCountFrame;
	//描画しない
	bool m_isDraw;
	
	//現在のターゲット情報
	TargetInfo m_targetInfo;
};

