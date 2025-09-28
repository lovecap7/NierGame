#pragma once
#include "Actor.h"
#include <memory>
class Collidable;
class Camera;
class DebugPlayer :
    public Actor
{
public:
    DebugPlayer();
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
	//カメラのセット
	void SetCamera(const std::weak_ptr<Camera> camera) { m_pCamera = camera; }
private:
    //カメラの参照
	std::weak_ptr<Camera> m_pCamera;
};

