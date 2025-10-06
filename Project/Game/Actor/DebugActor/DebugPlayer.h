#pragma once
#include "../Actor.h"
#include <memory>
class Collidable;
class PlayerCamera;
class CameraBase;
class DebugWeapon;
class DebugPlayer :
    public Actor
{
public:
    DebugPlayer(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager);
    ~DebugPlayer();
    /// <summary>
    /// 初期化処理
    /// </summary>
    virtual void Init() override;
    /// <summary>
    /// 位置などの更新を行う(引数なし)
    /// </summary>
    virtual void Update() override;
    /// <summary>
    /// 対象のアクターを表示
    /// </summary>
    virtual void Draw() const override;
    /// <summary>
    /// 終了処理
    /// </summary>
	virtual void End() override;
    /// <summary>
    /// 衝突したときのイベント関数
    /// </summary>
    /// <param name="other"></param>
    virtual void OnCollide(const std::shared_ptr<Collidable> other)override;
    /// <summary>
    /// Updateの処理によって起こった衝突処理などの処理の確定
    /// </summary>
    virtual void Complete() override;
	//武器のセット
    void SetWeapon(const std::weak_ptr<DebugWeapon> weapon);
private:
    //敵の参照
	std::weak_ptr<Actor> m_pEnemy;
    //ジャスト回避
	void JustAvoid();
	//無敵時間
	float m_invincibleTime = 0.0f;
    //武器
	std::weak_ptr<DebugWeapon> m_weapon;
};

