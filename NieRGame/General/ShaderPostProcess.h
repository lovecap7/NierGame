#pragma once
class ShaderPostProcess
{
public:
	ShaderPostProcess();
	~ShaderPostProcess();
	void Init();
	void Update();
	void Draw(const int& rt)const;
	void End();
private:
	//グリッジを行うか
	bool m_isGlitch;
	//白黒にするか
	bool m_isGray;
	//グリッジ用の定数バッファ
	struct Glitch
	{
		float time;
		float blockScale;
		float noiseSpeed;
		int state = 0; //0:通常 1:グリッジ 2:白黒 3:グリッジ+白黒
	};
	//定数バッファハンドル
	int m_cbuffHandle;
	//定数バッファのポインタ
	Glitch* m_glitch;
	//シェーダハンドル
	int m_psHandle;
};

