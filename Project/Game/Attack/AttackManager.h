#pragma once
#include <list>
#include <memory>
#include <map>
#include "../../General/Collision/Collidable.h"

class Actor;
class AttackBase;
class AttackManager : public std::enable_shared_from_this<AttackManager>
{
public:
	AttackManager();
	~AttackManager();
	//登録
	void Entry(std::shared_ptr<AttackBase> attack);
	//解除
	void Exit(std::shared_ptr<AttackBase> attack);
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw()const;
	//終了処理
	void End();
private:
	std::list<std::shared_ptr<AttackBase>> m_attacks;
private:
	//削除チェック
	void CheckDelete();
};

