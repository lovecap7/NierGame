#pragma once
#include "SceneBase.h"
class Input;
class SceneController;

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
    
};

