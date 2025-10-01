#pragma once
#include "SceneBase.h"
#include <memory>
class Input;
class CameraController;
class SceneController;
class ActorManager;

class GameScene :
    public SceneBase
{
public:
    GameScene(SceneController& controller);
    ~GameScene();
    /// <summary>
    /// 派生クラスで実装を実装
    /// virtual はこれを継承するかもしれないから
    /// overrideは明示的にオーバーライドをエディタに示すため
    /// </summary>
    virtual void Init()override;
    virtual void Update() override;
    virtual void Draw() override;
    virtual void End() override;
    virtual void Restart() override {};
    //デバッグ用
    virtual void DebugDraw() const override;
private:
	//アクターマネージャー
	std::shared_ptr<ActorManager> m_actorManager;
	//カメラ
	std::shared_ptr<CameraController> m_cameraController;
};

