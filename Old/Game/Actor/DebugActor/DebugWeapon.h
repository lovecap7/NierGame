#pragma once
#include "../Actor.h"
class DebugWeapon :
    public Actor
{
public:
	DebugWeapon(int weaponHandle);
	virtual ~DebugWeapon();
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
    void SetFrameIndex(int itemSlotIndex) { m_itemSlotIndex = itemSlotIndex; };
    //持ち主のハンドル
    void SetOwnerHandle(int ownerHandle) { m_ownerHandle = ownerHandle; };
    //大きさ
    void SetScale(float scale) { m_scale = scale; };
private:
    //持ち主のハンドル
    int m_ownerHandle;
    //フレーム番号
    int m_itemSlotIndex;
    //大きさ
    float m_scale;
};

