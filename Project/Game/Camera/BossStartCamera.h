#pragma once
#include "CameraBase.h"
#include <memory>
class ActorManager;
class BossStartCamera :
    public CameraBase
{
public:
    BossStartCamera(Vector3 bossPos,Vector3 bossDir,float distance, std::weak_ptr<ActorManager> pActorManager);
    ~BossStartCamera();
    void Init() override;
    void Update() override;
    void Draw() const override;

private:
    //ボスの座標
    Vector3 m_bossPos;
    Vector3 m_bossDir;
    //アクターマネージャーの参照
    std::weak_ptr<ActorManager> m_pActorManager;

    //フレームカウント
    int m_countFrame;
};

