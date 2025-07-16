#include "CollisionWorld.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Hook.h"
#include "Weapon.h"
#include "EnemyBullet.h"


/// -------------------------------------------------------------
///				　		衝突ワールドの初期化
/// -------------------------------------------------------------
void CollisionWorld::Initialize(CollisionManager* collisionManager, const CollisionObjects& objects)
{
	collisionManager_ = collisionManager; // 衝突マネージャへのポインタをセット
	objects_ = objects; // 衝突オブジェクトの構造体をセット
}


/// -------------------------------------------------------------
///				　		衝突ワールドの更新
/// -------------------------------------------------------------
void CollisionWorld::Update()
{
	collisionManager_->Reset(); // 衝突マネージャのリセット

	if (!objects_.player || !objects_.enemy) return; // プレイヤーまたは敵が存在しない場合は更新しない
	if (objects_.player->GetHp() <= 0 || objects_.enemy->GetHp() <= 0) return; // プレイヤーまたは敵が死亡している場合は更新しない

	collisionManager_->AddCollider(objects_.player); // プレイヤーのコライダーを追加
	if (objects_.player->GetIsAttack())
		collisionManager_->AddCollider(objects_.weapon); // プレイヤーが攻撃中なら武器のコライダーを追加

	collisionManager_->AddCollider(objects_.hook); // フックのコライダーを追加
	collisionManager_->AddCollider(objects_.enemy); // 敵のコライダーを追加

	for (const auto& bullet : *objects_.enemyBullets) {
		collisionManager_->AddCollider(bullet.get()); // 敵の弾のコライダーを追加
	}

	// 全衝突判定の判定と応答を行う
	collisionManager_->CheckAllCollisions();

	// 衝突判定の更新
	collisionManager_->Update(); // 衝突マネージャの更新
}
