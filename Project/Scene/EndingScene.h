#pragma once
#include "SceneBase.h"
#include "../General/StageIndex.h"
#include <memory>
class ActorManager;
class EffekseerManager;
class EndingTextUI;
class EndingScene :
    public SceneBase
{
public:
    EndingScene(SceneController& controller);
    ~EndingScene();
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

    //エフェクト
    EffekseerManager& m_effectManager;

	//エンディングテキストUI
	std::weak_ptr<EndingTextUI> m_endingTextUI;

    //スキップフレーム
	int m_skipFrame;
};

