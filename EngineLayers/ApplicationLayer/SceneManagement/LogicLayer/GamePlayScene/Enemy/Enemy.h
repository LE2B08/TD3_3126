#pragma once
#include "Object3D/Object3D.h"
#include "Collider.h"
#include <memory>
#include <optional>

/// ===== 前方宣言 ===== ///
class Player;
class EnemyBullet;

/// === 敵 === ///
class Enemy :public Collider {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// コンストラクタ
	Enemy();

	/// デストラクタ
	~Enemy();

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// 移動
	void Move();

	/// ImGui
	void ShowImGui(const char* name);

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	///-------------------------------------------/// 
	/// 行動別処理
	///-------------------------------------------///
public:

	/// <summary>
	/// 通常時初期化
	/// </summary>
	void BehaviorNormalInitialize();

	/// <summary>
	/// 通常時更新
	/// </summary>
	void BehaviorNormalUpdate();

	/// <summary>
	/// 攻撃時初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃時更新
	/// </summary>
	void BehaviorAttackUpdate();

	/// <summary>
	/// 離脱時初期化
	/// </summary>
	void BehaviorLeaveInitialize();

	/// <summary>
	/// 離脱時更新
	/// </summary>
	void BehaviorLeaveUpdate();

	///-------------------------------------------/// 
	/// ゲッター&セッター
	///-------------------------------------------///
public:

	void SetPlayer(Player* player) { player_ = player; }

	///-------------------------------------------/// 
	/// 列挙
	///-------------------------------------------///
public:

	enum class Behavior {

		Normal, // 通常
		Attack, // 攻撃
		Leave,  // 離脱
	};

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// ワールド変換
	WorldTransform worldTransform_;

	// 敵のオブジェクト
	std::unique_ptr<Object3D> objectEnemy_;

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 速度の最大値
	float velocityLimit_ = 0.1f;

	// 加速度の最大値
	float accelerationLimit_ = 0.01f;

	// プレイヤー
	Player* player_;

	// 状態
	Behavior behavior_ = Behavior::Normal;

	// 状態リクエスト
	std::optional<Behavior> requestBehavior_ = std::nullopt;

	// 離脱時の向き
	Vector3 leaveDirection_ = {};

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = {};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = {};

	// 弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// 弾の発射間隔
	uint32_t attackInterval_ = 120;

	// 弾の発射回数
	uint32_t attackCount_ = 0;

	const float maxDistance_ = 10.0f;
};

