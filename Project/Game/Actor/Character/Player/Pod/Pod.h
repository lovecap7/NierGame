#pragma once
#include "../../CharacterBase.h"
#include <memory>
#include <vector>
#include <string>
class ActorManager;
class Collidable;
class Player;
class Rigidbody;
class ActorData;
class CharaStatusData;
class AttackData;
class BulletAttack;
class CSVDataLoader;
class AttackBase;

class Pod :
    public CharacterBase
{
public:
    Pod(std::shared_ptr<ActorData> actorData, std::shared_ptr<CharaStatusData> charaStatusData,
		std::weak_ptr<ActorManager> pActorManager,std::weak_ptr<Player> pPlayer);
    ~Pod();
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
	//Player座標
	Vector3 GetPlayerPos()const;
	//カメラの向き
	Vector3 GetCameraDir()const;

	//攻撃データ
	std::shared_ptr<AttackData> GetAttackData(std::wstring attackName)const;
	//弾
	std::vector<std::shared_ptr<BulletAttack>> GetBullets()const { return m_bullets; };
private:
	//プレイヤーの参照
	std::weak_ptr<Player> m_pPlayer;

	//攻撃データ
	std::vector<std::shared_ptr<AttackData>> m_attackDatas;

	//弾を打つ際にその都度newすると負荷がかかり重くなるのであらかじめ作っておく
	std::vector<std::shared_ptr<BulletAttack>> m_bullets;
private:
	//攻撃データを読み込む
	void InitAttackData(std::shared_ptr<CSVDataLoader> csvLoader);
};

