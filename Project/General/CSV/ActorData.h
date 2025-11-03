#pragma once
#include "CSVData.h"
#include "../Math/MyMath.h"
#include "../Collision/Collidable.h"
#include <string>
class ActorData :
	public CSVData
{
public:
	ActorData();
	ActorData(std::shared_ptr<CSVData> data);
	~ActorData();
	//アクターを識別するタイプ
	enum class ActorType
	{
		None,
		Character,
		Stage,
		Attack,
		Weapon,
	};
	//名前
	std::wstring GetName() const{ return m_name; };
	//ID
	unsigned int GetActorID() const{ return m_actorID; };
	//アクタータイプ
	ActorType GetActorType()const { return m_actorType; };
	//座標
	Vector3 GetPos()const { return m_pos; };
	//回転
	Vector3 GetRot()const {return m_rot;};
	//大きさ
	Vector3 GetScale()const { return m_scale; };
	void SetScale(Vector3 scale) { m_scale = scale; };
	//モデルのパス
	std::wstring GetModelPath()const { return m_modelPath; };
	void SetModelPath(std::wstring path) { m_modelPath = path; };
	//優先度
	Priority GetPriority()const { return m_priority; };
	//ゲームタグ
	GameTag GetGameTag()const { return m_gameTag; };
	//当たり判定を無視するか
	bool IsTrough()const { return m_isTrough; };
	//トリガー
	bool IsTrigger()const { return m_isTrigger; };
	//重力
	bool IsGravity()const { return m_isGravity; };
	//半径
	float GetCollRadius()const { return m_collRadius; };
	//高さ
	float GetCollHeight()const { return m_collHeight; };
	//様々なのパスをまとめたパスデータ
	std::wstring GetCSVPathData()const { return m_csvPathData; };
private:
	//変換
	void Conversion() override;
	//名前
	std::wstring m_name;
	//ID
	unsigned int m_actorID;
	//アクタータイプ
	ActorType m_actorType;
	//座標
	Vector3 m_pos;
	//回転
	Vector3 m_rot;
	//大きさ
	Vector3 m_scale;
	//モデルのパス
	std::wstring m_modelPath;
	//優先度
	Priority m_priority;
	//ゲームタグ
	GameTag m_gameTag;
	//当たり判定を無視するか
	bool m_isTrough;
	//トリガー
	bool m_isTrigger;
	//重力
	bool m_isGravity;
	//半径
	float m_collRadius;
	//高さ
	float m_collHeight;
	//様々なのパスをまとめたパスデータ
	std::wstring m_csvPathData;
};

