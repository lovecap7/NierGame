#pragma once
#include<DxLib.h>
#include<cassert>
#include<array>
namespace MyDraw {
	inline void DrawRotaGraph(int x, int y, float scale, float angle, int handle, int psH, bool alpha, int secondH = -1) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x - w * 0.5 * scale;
		vertices[0].pos.y = y - h * 0.5 * scale;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x + w * 0.5 * scale;
		vertices[1].pos.y = y - h * 0.5 * scale;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x - w * 0.5 * scale;
		vertices[2].pos.y = y + h * 0.5 * scale;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x + w * 0.5 * scale;
		vertices[3].pos.y = y + h * 0.5 * scale;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	inline void DrawGraph(int x, int y, int handle, int psH, bool alpha, int secondH = -1) {
		std::array<DxLib::VERTEX2DSHADER, 4> vertices = {};
		//4頂点に必要な情報を代入していく
		//必要な情報→座標、頂点カラー、UV
		//ただし、rhwは必ず1.0にする必要があります。

		for (auto& v : vertices) {
			v.rhw = 1.0f;//これは必ず1
			v.dif = { 255,255,255,255 };//これは白色にしておく
		}

		int w, h;
		DxLib::GetGraphSize(handle, &w, &h);

		//並べ方はZの字になるように
		//頂点情報(左上)
		vertices[0].pos.x = x;
		vertices[0].pos.y = y;
		vertices[0].u = 0.0f;
		vertices[0].v = 0.0f;

		//頂点情報(右上)
		vertices[1].pos.x = x + w;
		vertices[1].pos.y = y;
		vertices[1].u = 1.0f;
		vertices[1].v = 0.0f;

		//頂点情報(左下)
		vertices[2].pos.x = x;
		vertices[2].pos.y = y + h;
		vertices[2].u = 0.0f;
		vertices[2].v = 1.0f;

		//頂点情報(右下)
		vertices[3].pos.x = x + w;
		vertices[3].pos.y = y + h;
		vertices[3].u = 1.0f;
		vertices[3].v = 1.0f;

		//テクスチャを指定
		DxLib::SetUseTextureToShader(0, handle);
		if (secondH >= 0) {
			DxLib::SetUseTextureToShader(1, secondH);
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

		//シェーダを指定
		DxLib::SetUsePixelShader(psH);

		DxLib::DrawPrimitive2DToShader(vertices.data(), vertices.size(),
			DX_PRIMTYPE_TRIANGLESTRIP);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	inline void DrawPS(int x, int y, float scale, float angle, const int cBuffH, int handle, int psH)
	{
		//更新
		UpdateShaderConstantBuffer(cBuffH);
		//4番にセット
		SetShaderConstantBuffer(cBuffH, DX_SHADERTYPE_PIXEL, 4);
		//画像をノイズ入りで描画
		MyDraw::DrawRotaGraph(x, y, scale, angle, handle, psH, true);
		// シェーダーをオフに
		DxLib::SetUsePixelShader(-1);
	}
}