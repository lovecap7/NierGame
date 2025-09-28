#pragma once
class ShaderPostProcess
{
public:
	ShaderPostProcess();
	~ShaderPostProcess();
	void Init();
	void Update();
	void Draw(const int rt)const;
	void End();
	//ポストエフェクトの状態
	enum class PostEffectState : int
	{
		Normal = 0,
		Glitch = 1 << 0, //0001
		Gray = 1 << 1,   //0010
		//Damage = 1 << 2, //0100
	};
	//ポストエフェクトの状態を設定
	void SetPostEffectState(const PostEffectState state){ m_glitch->state = static_cast<int>(state); }
	//状態の加算
	void AddPostEffectState(const PostEffectState state) { m_glitch->state |= static_cast<int>(state); }
	//状態の減算
	void SubPostEffectState(const PostEffectState state) { m_glitch->state &= ~static_cast<int>(state); }
	//状態リセット
	void ResetPostEffectState() { m_glitch->state = static_cast<int>(PostEffectState::Normal); }
private:
	//グリッジ用の定数バッファ
	struct Glitch
	{
		float shakeStrength;	//揺れの強さ(でかいほどバグったような表現が強くなる)
		float blockScale;		//ブロックノイズの大きさ
		float noiseSpeed;		//ノイズの動く速さ
		int state; //0:通常 1:グリッジ 2:白黒 3:グリッジ+白黒
	};
	//定数バッファハンドル
	int m_cbuffHandle;
	//定数バッファのポインタ
	Glitch* m_glitch;
	//シェーダハンドル
	int m_psHandle;
};

