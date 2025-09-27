#pragma once
#include <DxLib.h>
#include <memory>
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

class Animator;
class Model
{
public:
	Model(int modelHandle, VECTOR pos);
	Model(int modelHandle, VECTOR pos, Vector3 dir);
	virtual ~Model();
	int GetModelHandle() { return m_modelHandle; };
	void Update();
	void Draw()const;
	void End();
	//セッター
	void SetPos(VECTOR pos);
	void SetScale(VECTOR pos);
	void SetRot(VECTOR rot);
	void SetDir(Vector2 vec);
	void SetColor(float r, float g, float b, float a);
	void SetColor(COLOR_F color);
	void ResetColor();
	//モデル変更
	void SetModel(int modelHandle);
	//ゲッター
	Vector3 GetPos() const { return m_pos; };
	Vector3 GetScale() const { return m_scale; };
	//向き
	Vector3 GetDir();
	//回転
	Quaternion GetRot()const { return m_rotation; };
	//回転速度
	void SetRotSpeed(int speed) { m_rotaSpeed = speed; };
	//高さ調整
	void SetModelHeightAdjust(float height) { m_modelHeightAdjust = height; };
	//攻撃を喰らった時のモデルのリアクション(攻撃を喰らった時に呼ぶ)
	void ModelHit();
	//アニメーション関係
	void SetAnim(const char* animName,bool isLoop);//アニメーション変更
	void SetAnim(const char* animName,bool isLoop,const float& animSpeed);//アニメーション変更(再生速度も)
	void SetAnimSpeed(const float& animSpeed);//アニメーションの速度変更
	bool IsFinishAnim();//アニメーションが終わったかどうか
	float GetTotalAnimFrame();//全体フレーム
	void SetTotalAnimFrame(float totalFrame);//全体フレームセット
	float GetNowAnimFrame();//現在のアニメーションフレーム
	void ReplayAnim();//アニメーションを最初から再生
	void DeleteAnim();//再生中のアニメーションを削除
	void SetFixedLoopFrame(float loopFrame);//指定ループ再生
	bool IsFinishFixedLoop();//指定ループ再生終了
private:
	int m_modelHandle;
	//アニメーション
	std::unique_ptr<Animator> m_animator;
	//回転の向き
	Vector3 m_angleAxis;
	//モデルの回転情報
	Quaternion m_rotation;
	//モデルの正面
	Vector3 m_forward;
	//次のモデルの正面
	Vector3 m_nextForward;
	//回転クォータニオン
	Quaternion m_rotaQ;
	//回転フレーム
	int m_rotaFrame;
	//回転速度
	int m_rotaSpeed;
	//自分の座標
	Vector3 m_pos;
	//大きさ
	Vector3 m_scale;
	//1フレーム前の指定された向き
	Vector2 m_beforeSetDir;
	//モデルの高さ調整
	float m_modelHeightAdjust;

	//攻撃を喰らった時にもとの状態に戻るのを数えるフレーム
	int m_hitCountFrame;
	//攻撃を喰らった時に赤色から元に戻す処理に使う
	COLOR_F m_color;
	//攻撃を食らった時に少し大きくするので差を覚えておく
	Vector3 m_beforeScaleDif;
	Vector3 m_beforeScale;
	//モデルに行列を適用させる
	void ApplyMat();
};

