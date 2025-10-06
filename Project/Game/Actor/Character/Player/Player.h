#pragma once
#include "../CharacterBase.h"
#include "../../../../General/Math/MyMath.h"
#include <memory>
#include <map>
#include <string>

class ActorManager;
class UltGage;
class GameCamera;
class CharacterStateBase;
class HitPoints;
class TrackActorEffect;
class Input;
class Player :
	public CharacterBase
{
public:
	Player(int modelHandle, Position3 firstPos);
	virtual ~Player();
	//初期化処理
	void Init()override;
	//更新処理
	void Update(const std::weak_ptr<GameCamera> camera, const std::weak_ptr<ActorManager> actorManager) override;
	//衝突イベント関数
	void OnCollide(const std::shared_ptr<Collidable> other)override;
	//描画
	void Draw()const override;
	//更新処理による結果の確定
	void Complete() override;
	//死亡処理
	void Dead(const std::weak_ptr<ActorManager> actorManager, const std::weak_ptr<Score> score) override;
	//終了処理
	void End()override;
	//復活
	void Revival();

	//ステートにアクセスさせる関数
	//リジッドボディ
	std::shared_ptr<Rigidbody> GetRb() const { return m_rb; }
	//コリジョン
	std::shared_ptr<ColliderBase> GetColl() const { return m_collisionData; }
	//コリジョンの状態を設定
	void SetCollState(CollisionState collState) { m_collState = collState; }
	CollisionState GetCollState()const { return m_collState; };

	//入力中の方向キー(プレイヤーのモデルの向きに対応したベクトル)
	Vector2 GetPlayerStickVec() { return m_stickVec; };
	//必殺技ゲージ
	std::weak_ptr<UltGage> GetUltGage() const{ return m_ultGage; };
	//ターゲットのデータ
	TargetData GetTargetData() const { return m_targetData; };
	//ターゲットの索敵
	virtual void TargetSearch(float searchDistance, float searchAngle, Vector3 targetPos) override;
	//ダッシュキープフラグ
	bool IsRunKeep() { return m_isRunKeep; };
	void SetIsRunKeep(bool isRunKeep) { m_isRunKeep = isRunKeep; };

	//死亡
	bool IsDead()const { return m_isDead; };
	void SetIsDead(bool isDead) { m_isDead = isDead; };
	//ゲームの進行に使う関数
	//ゲーム開始アニメーションをしている
	bool IsStartAnim();
	//ゲームクリアアニメーション
	bool IsClearAnim();
	//ゲームクリアアニメーション終了
	bool IsFinishClearAnim();
private:
	//スティックの向きを持つベクトル
	Vector2 m_stickVec;
	//必殺技ゲージ
	std::shared_ptr<UltGage> m_ultGage;
	//ダッシュ状態をキープするフラグ
	bool m_isRunKeep;
	//ダッシュ状態を解除するフレーム
	int m_cancelRunFrame;
	//必殺ゲージMAXエフェクト
	std::weak_ptr<TrackActorEffect> m_ultMaxEff;
	//落下したときに戻す座標(チェックポイントを実装したら使う)
	Vector3 m_initPos;
	//死んだフラグ
	bool m_isDead;
private:
	//プレイヤーの入力ベクトルを更新
	void UpdatePlayerStickVec(Input& input);
	//走りを継続するか
	void CheckRunKeep();
	//必殺ゲージが最大か確認
	void CheckUltMax();
	//サウンド初期化
	void InitSound() override;
};

