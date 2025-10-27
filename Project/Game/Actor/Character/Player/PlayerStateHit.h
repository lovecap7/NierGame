#include "PlayerStateBase.h"
class Actor;
class PlayerStateHit :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateHit>
{
public:
    PlayerStateHit(std::weak_ptr<Actor>  player);
    ~PlayerStateHit();
    void Init()override;
    void Update() override;
private:
    //グリッジフレーム
    float m_glitchFrame;
    float m_glitchScale;
    float m_glitchSpeed;
    float m_glitchkStrengt;
};

