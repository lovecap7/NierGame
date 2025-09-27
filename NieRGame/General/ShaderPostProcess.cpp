#include "ShaderPostProcess.h"
#include "ShaderDrawUtil.h"
#include "Game.h"
#include <DxLib.h>

ShaderPostProcess::ShaderPostProcess():
	m_isGlitch(false),
	m_isGray(false),
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
	m_glitch->time = 0.0f;
	m_glitch->blockScale = 0.0f;
	m_glitch->noiseSpeed = 0.0f;
	m_glitch->state = 0;
	//ポストエフェクト
	m_psHandle = LoadPixelShader(L"Shader/PostEffects.pso");
	assert(m_psHandle >= 0);
}
void ShaderPostProcess::Update()
{
	//更新
	m_glitch->time = static_cast<float>(GetRand(50)) / 1000.0f;
	m_glitch->blockScale = static_cast<float>(GetRand(10));
	m_glitch->noiseSpeed = static_cast<float>(GetRand(10));
	m_glitch->state = 0;
}

void ShaderPostProcess::Draw(const int& rt) const
{
	MyDraw::DrawPS(Game::kScreenCenterX, Game::kScreenCenterY, 1.0, 0.0, m_cbuffHandle, rt, m_psHandle);
}

void ShaderPostProcess::End()
{
	//解放
	DeleteShader(m_psHandle);
	DeleteShaderConstantBuffer(m_cbuffHandle);
}
