#include "ShaderPostProcess.h"
#include "ShaderDrawUtil.h"
#include "Math/MyMath.h"
#include "Game.h"
#include <DxLib.h>

ShaderPostProcess::ShaderPostProcess() :
	m_psHandle(-1),
	m_cbuffHandle(-1),
	m_glitch(nullptr)
{
}

ShaderPostProcess::~ShaderPostProcess()
{
}

void ShaderPostProcess::Init()
{
	//定数バッファ
	m_cbuffHandle = CreateShaderConstantBuffer(sizeof(Glitch));
	m_glitch = static_cast<Glitch*>(GetBufferShaderConstantBuffer(m_cbuffHandle));
	m_glitch->shakeStrength = 0.0f;
	m_glitch->blockScale = 0.0f;
	m_glitch->noiseSpeed = 0.0f;
	m_glitch->state = static_cast<int>(PostEffectState::Normal);
	//ポストエフェクト
	m_psHandle = LoadPixelShader(L"Shader/PostEffects.pso");
	assert(m_psHandle >= 0);
}
void ShaderPostProcess::Update()
{
	if (m_glitch == nullptr)return;
	if (m_glitch->state & static_cast<int>(PostEffectState::Glitch))
	{
		//更新
		m_glitch->shakeStrength = MyMath::GetRandF(0.0f,0.5f);
		m_glitch->blockScale = MyMath::GetRandF(0.0f, 10.0f);
		m_glitch->noiseSpeed = MyMath::GetRandF(0.0f, 10.0f);
	}
}

void ShaderPostProcess::Draw(const int rt) const
{
	MyDraw::DrawPS(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_cbuffHandle, rt, m_psHandle);
}

void ShaderPostProcess::End()
{
	//解放
	DeleteShader(m_psHandle);
	DeleteShaderConstantBuffer(m_cbuffHandle);
}
