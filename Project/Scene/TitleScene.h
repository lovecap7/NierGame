#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
class Input;
class SceneController;
class CameraController;
class TitleUI;
class TitleScene :
    public SceneBase
{
public:
    TitleScene(SceneController& controller);
    ~TitleScene();
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
    //カメラ
    std::shared_ptr<CameraController> m_cameraController;
    //フレームカウント
    int m_hardShakingCountFrame;
};

