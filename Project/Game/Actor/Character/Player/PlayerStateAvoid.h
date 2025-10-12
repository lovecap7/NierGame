#pragma once
#include "PlayerStateBase.h"
#include <memory>
class Actor;
class Player;
class Model;
class Application;
class PlayerStateAvoid :
    public PlayerStateBase, public std::enable_shared_from_this<PlayerStateAvoid>
{
public:
    PlayerStateAvoid(std::weak_ptr<Actor>  player);
    ~PlayerStateAvoid();
    void Init()override;
    void Update() override;
private:
    //移動方向
    Vector3 m_avoidDir;
    //速度
    float m_speed;
    //エンド
    float m_endSpeed;
    //ジャスト回避成功
    bool m_isJustAvoid;
    //ジャスト回避が終了フレーム
    float m_finishJustAvoid;
private:
    //ジャスト回避初期化
    void InitJustAvoid(std::shared_ptr<Model> model, std::shared_ptr<Player> owner);
    //ジャスト回避更新
    void UpdateJustAvoid(std::shared_ptr<Player> owner, std::shared_ptr<Model> model, Application& app);
    //回避移動
    void MoveAvoid(Application& app, std::shared_ptr<Player> owner);
};
