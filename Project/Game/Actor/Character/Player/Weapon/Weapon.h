#pragma once
#include "../../../Actor.h"
class ActorData;
class ActorManager;
class Weapon :
    public Actor
{
public:
    Weapon(std::shared_ptr<ActorData> actorData, std::weak_ptr<ActorManager> pActorManager);
    virtual ~Weapon();
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
    //武器の行列
    void SetWeaponMat(Matrix4x4 mat);
    //フレーム番号
    void SetFrameIndex(int battleSlotIndex, int idleSlotIndex)
    {
        m_battleSlotIndex = battleSlotIndex;
        m_idleSlotIndex = idleSlotIndex;
    };
    //持ち主のハンドル
    void SetOwnerHandle(int ownerHandle) { m_ownerHandle = ownerHandle; };
    
    //戦闘中か
    bool IsBattle()const { return m_isBattle; };
    void SetIsBattle(bool isBattle) { m_isBattle = isBattle; };

    //剣先
    Vector3 GetUP()const { return m_up; };
private:
    //持ち主のハンドル
    int m_ownerHandle;

    //戦闘中のフレーム番号
    int m_battleSlotIndex;

    //非戦闘のフレーム番号
    int m_idleSlotIndex;

    //戦闘中かどうか
    bool m_isBattle;

    //剣先
    Vector3 m_up;
};

