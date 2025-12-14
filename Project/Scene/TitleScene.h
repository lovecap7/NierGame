#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
class Input;
class SceneController;
class CameraController;
class TitleUI;
class ActorManager;
class SE;
class EffekseerManager;
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
    //最初の激しいグリッジを行うフレームをカウント
    int m_hardShakingCountFrame;
    //各項目の値
    float m_blockScele;
    float m_noiseSpeed;
    float m_shakeStrength;

    //アクターマネージャー
    std::shared_ptr<ActorManager> m_actorManager;

    //エフェクト
    EffekseerManager& m_effectManager;

    //SE
    std::weak_ptr<SE> m_noiseSE;
private:
    //グリッジの更新
    void UpdateGlitch();
};

