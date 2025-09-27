#include "Model.h"
#include "Math/MyMath.h"
#include "Animator.h"
#include "StringUtil.h"
#include <array>
#include <cassert>
#include <string>

namespace
{
	constexpr float kAnimSpeed = 0.5f;//再生速度
	//回転速度
	constexpr int kModelRotateSpeed = 10;
	//ヒット効果フレーム
	constexpr int kHitFrame = 30.0f;
	//ヒット効果でモデルが大きくなる倍率
	constexpr float kHiScaleRate = 1.1f;
}

Model::Model(int modelHandle, VECTOR pos) :
	m_modelHandle(modelHandle),
	m_forward{ 0.0f,0.0f,1.0f },
	m_nextForward{ 0.0f,0.0f,1.0f },
	m_rotation(Quaternion::AngleAxis(180 * MyMath::DEG_2_RAD, Vector3::Up())),
	m_rotaQ(Quaternion::IdentityQ()),
	m_rotaFrame(0),
	m_rotaSpeed(kModelRotateSpeed),
	m_pos(pos),
	m_scale{ 1.0f,1.0f,1.0f },
	m_hitCountFrame(0),
	m_color{ 1.0f,1.0f ,1.0f ,1.0f },
	m_beforeScale(m_scale),
	m_beforeScaleDif{},
	m_modelHeightAdjust(0.0f)
{
	//座標
	DxLib::MV1SetPosition(m_modelHandle, pos);
	//アニメーション
	m_animator = std::make_unique<Animator>();
}

Model::Model(int modelHandle, VECTOR pos, Vector3 forward) :
	m_modelHandle(modelHandle),
	m_forward(forward),
	m_nextForward(forward),
	m_rotation(Quaternion::IdentityQ()),
	m_rotaQ(Quaternion::IdentityQ()),
	m_rotaFrame(0),
	m_pos(),
	m_scale{ 1.0f,1.0f,1.0f },
	m_hitCountFrame(0),
	m_color{ 1.0f,1.0f ,1.0f ,1.0f },
	m_beforeScale(m_scale),
	m_beforeScaleDif{},
	m_beforeSetDir{ forward.XZ() },
	m_modelHeightAdjust(0.0f)
{
	//座標
	m_pos = pos;
	DxLib::MV1SetPosition(m_modelHandle, m_pos.ToDxLibVector());
	//アニメーション
	m_animator = std::make_unique<Animator>();
}

Model::~Model()
{
}

void Model::Update()
{
	//アニメーションの更新
	m_animator->PlayAnim(m_modelHandle);

	//向きの更新
	if (m_rotaFrame > 0)
	{
		--m_rotaFrame;
		//回転
		m_rotation = m_rotaQ * m_rotation;
		//正規化
		if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
		{
			m_rotation = m_rotation.NormQ();
			DxLib::MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
		}
		//前ベクトル
		m_forward = m_rotaQ * m_forward;
		//正規化
		if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
	}
	//ヒット効果から元の状態に戻していく
	if (m_hitCountFrame > 0)
	{
		--m_hitCountFrame;
		//もとに戻してく(色)
		m_color.g += 1.0f / kHitFrame;
		m_color.b += 1.0f / kHitFrame;
		SetColor(m_color);
		//大きさ
		m_scale -= m_beforeScaleDif / kHitFrame;
	}
	//行列適用
	ApplyMat();
}

void Model::Draw() const
{
	//描画
	auto err = DxLib::MV1DrawModel(m_modelHandle);
	assert(err != -1 && L"モデルが描画できません");
}

void Model::End()
{
	assert(DxLib::MV1DeleteModel(m_modelHandle) == 0);
}

void Model::SetPos(VECTOR pos)
{
	m_pos = pos;
}

void Model::SetScale(VECTOR scale)
{
	m_scale = scale;
	m_beforeScale = m_scale;
}

void Model::SetRot(VECTOR rot)
{
	//オイラー角から回転
	m_rotaQ = Quaternion::Euler(rot.x, rot.y, rot.z);
	//回転
	m_rotation = m_rotaQ * m_rotation;
	//正規化
	if (m_rotation.Magnitude() > 0.0f && m_rotaQ.w < 1.0f)
	{
		m_rotation = m_rotation.NormQ();
		DxLib::MV1SetRotationMatrix(m_modelHandle, m_rotation.GetMatrix().ToDxLibMATRIX());
	}
	//前ベクトル
	m_forward = m_rotaQ * m_forward;
	//正規化
	if (m_forward.Magnitude() > 0.0f)m_forward = m_forward.Normalize();
}

void Model::SetDir(Vector2 vec)
{
	//向きが決められないのでリターン
	if (vec.Magnitude() <= 0.0f)	return;
	//向きを計算
	Vector2 dir = vec;
	dir = dir.Normalize();
	//前回と同じかつ向きも同じなら何もしない
	if (m_beforeSetDir == dir && m_forward.XZ() == dir)return; 
	m_beforeSetDir = dir; //前回の向きを記録
	//角度
	float angle = Vector2::GetRad(m_forward.XZ(), dir);
	//回転軸
	Vector3 axis = m_forward.Cross(dir.XZ());
	if (axis.SqMagnitude() == 0.0f)
	{
		axis = Vector3::Up();
	}
	//回転クォータニオン作成
	m_rotaQ = Quaternion::AngleAxis(angle/ m_rotaSpeed, axis);
	//フレームをセット
	m_rotaFrame = m_rotaSpeed;
	//次の正面ベクトルを記録
	m_nextForward = dir.XZ();
}

void Model::SetColor(float r, float g, float b, float a)
{
	COLOR_F color = { r, g, b, a };
	m_color = color;
	m_color.r = MathSub::ClampFloat(m_color.r, 0.0f, 1.0f);
	m_color.g = MathSub::ClampFloat(m_color.g, 0.0f, 1.0f);
	m_color.b = MathSub::ClampFloat(m_color.b, 0.0f, 1.0f);
	m_color.a = MathSub::ClampFloat(m_color.a, 0.0f, 1.0f);
	DxLib::MV1SetDifColorScale(m_modelHandle, m_color);
	DxLib::MV1SetSpcColorScale(m_modelHandle, m_color);
	DxLib::MV1SetAmbColorScale(m_modelHandle, m_color);
}

void Model::SetColor(COLOR_F color)
{
	m_color.r = MathSub::ClampFloat(m_color.r, 0.0f, 1.0f);
	m_color.g = MathSub::ClampFloat(m_color.g, 0.0f, 1.0f);
	m_color.b = MathSub::ClampFloat(m_color.b, 0.0f, 1.0f);
	m_color.a = MathSub::ClampFloat(m_color.a, 0.0f, 1.0f);
	m_color = color;
	DxLib::MV1SetDifColorScale(m_modelHandle, m_color);
	DxLib::MV1SetSpcColorScale(m_modelHandle, m_color);
	DxLib::MV1SetAmbColorScale(m_modelHandle, m_color);
}

void Model::ResetColor()
{
	m_color = { 1,1,1,1 };
	DxLib::MV1SetDifColorScale(m_modelHandle, m_color);
	DxLib::MV1SetSpcColorScale(m_modelHandle, m_color);
	DxLib::MV1SetAmbColorScale(m_modelHandle, m_color);
}

void Model::SetModel(int modelHandle)
{
	//モデルの削除
	End();
	//新しいモデル
	m_modelHandle = modelHandle;
}

Vector3 Model::GetDir()
{
	Vector3 dir = m_forward;
	if (dir.Magnitude() > 0.0f)
	{
		dir = dir.Normalize();
	}
	return dir;
}
void Model::ModelHit()
{
	//赤に
	SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	//フレームをセット
	m_hitCountFrame = kHitFrame;
	//少し大きくする
	m_scale = m_beforeScale;						//一旦元の大きさ
	m_scale *= kHiScaleRate;						//大きくする
	m_beforeScaleDif = (m_scale - m_beforeScale);	//差を計算

}

void Model::SetAnim(const char* animName, bool isLoop)
{
	auto animNameT = StringUtil::ToTCHAR(animName);
	//アニメーションを変更
	m_animator->SetAnim(m_modelHandle, DxLib::MV1GetAnimIndex(m_modelHandle, animNameT), isLoop);
}

void Model::SetAnim(const char* animName, bool isLoop, const float& animSpeed)
{
	auto animNameT = StringUtil::ToTCHAR(animName);
	//アニメーションを変更
	m_animator->SetAnim(m_modelHandle, DxLib::MV1GetAnimIndex(m_modelHandle, animNameT), isLoop, animSpeed);
}

void Model::SetAnimSpeed(const float& animSpeed)
{
	//アニメーションの速度
	m_animator->SetAnimSpeed(animSpeed);
}

bool Model::IsFinishAnim()
{
	//アニメーションが終わったかどうか
	return m_animator->IsFinishAnim();
}

float Model::GetTotalAnimFrame()
{
	//全体フレーム
	return m_animator->GetTotalAnimFrame();
}

void Model::SetTotalAnimFrame(float totalFrame)
{
	//全体フレームセット
	m_animator->SetTotalAnimFrame(totalFrame);
}

float Model::GetNowAnimFrame()
{
	//現在のフレーム
	return m_animator->GetAnimFrame();
}

void Model::ReplayAnim()
{
	//アニメーションを再生
	m_animator->Replay();
}

void Model::DeleteAnim()
{
	//削除
	m_animator->AllRemoveAnim(m_modelHandle);
}

void Model::SetFixedLoopFrame(float loopFrame)
{
	//指定ループ
	m_animator->SetFixedLoop(loopFrame);
}

bool Model::IsFinishFixedLoop()
{
	//指定ループ終了
	return m_animator->IsFinishFixedLoop();
}
void Model::ApplyMat()
{
	Matrix4x4 mat;
	auto pMat = Matrix4x4::TranslateMat4x4(m_pos.x, m_pos.y + m_modelHeightAdjust, m_pos.z);
	auto rMat = m_rotation.GetMatrix();
	auto sMat = Matrix4x4::ScaleMatrix4x4(m_scale);
	mat = sMat * rMat * pMat;
	DxLib::MV1SetMatrix(m_modelHandle, mat.ToDxLibMATRIX());
}