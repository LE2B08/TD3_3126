#pragma once
#include <list>
#include <memory>

/// ---------- 前方宣言 ---------- ///
class CollisionManager;
class Player;
class Enemy;
class Hook;
class Weapon;
class EnemyBullet;

/// ---------- 衝突オブジェクトの構造体 ---------- ///
struct CollisionObjects
{
	Player* player = nullptr;          // プレイヤー
	Weapon* weapon = nullptr;          // 武器
	Hook* hook = nullptr;              // フック
	Enemy* enemy = nullptr;            // 敵
	std::list<std::unique_ptr<EnemyBullet>>* enemyBullets = nullptr; // 敵の弾
};

/// -------------------------------------------------------------
///				　		衝突ワールドクラス
/// -------------------------------------------------------------
class CollisionWorld
{
public: /// ---------- メンバ関数 ---------- ///

	// 衝突ワールドの初期化
	void Initialize(CollisionManager* collisionManager, const CollisionObjects& objects);

	// 衝突ワールドの更新
	void Update();

private: /// ---------- メンバ変数 ---------- ///

	CollisionManager* collisionManager_ = nullptr; // 衝突マネージャへのポインタ
	CollisionObjects objects_;                     // 衝突オブジェクトの構造体

};

