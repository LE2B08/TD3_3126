#pragma once
#include "Collider.h"
#include "GamePlayScene/Enemy/Enemy.h"
#include "Object3D.h"

/// === フック === ///
class Hook : public Collider {

	///-------------------------------------------///
	/// 列挙
	///-------------------------------------------///
public:
	enum class Behavior {

		None,   // なし
		Throw,  // 投げる
		Extend, // 伸ばす
		Move,   // 移動
		Back    // 戻す
	};

	///-------------------------------------------///
	/// メンバ関数
	///-------------------------------------------///
public:
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 何もしてない状態の初期化
	void BehaviorNoneInitialize();

	// 何もしてない状態の更新
	void BehaviorNoneUpdate();

	// 投げる状態の初期化
	void BehaviorThrowInitialize();

	// 投げる状態の更新
	void BehaviorThrowUpdate();

	// 伸ばす状態の初期化
	void BehaviorExtendInitialize();

	// 伸ばす状態の更新
	void BehaviorExtendUpdate();

	// Move状態の初期化
	void BehaviorMoveInitialize();

	// Move状態の更新
	void BehaviorMoveUpdate();

	// フックを戻す状態の初期化
	void BehaviorBackInitialize();
	// フックを戻す状態の更新
	void BehaviorBackUpdate();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	void ShowImGui();

	///-------------------------------------------///
	/// ゲッター & セッター
	///-------------------------------------------///
public:
	// プレイヤーの回転
	Vector3 GetPlayerRotation() { return playerRotation_; }
	void SetPlayerRotation(const Vector3& playerRotation) { playerRotation_ = playerRotation; }

	// プレイヤーの位置
	Vector3 GetPlayerPosition() { return playerPosition_; }
	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }

	// プレイヤーの速度
	Vector3 GetPlayerVelocity() { return playerVelocity_; }
	void SetPlayerVelocity(const Vector3& playerVelocity) { playerVelocity_ = playerVelocity; }

	// プレイヤーの加速度
	Vector3 GetPlayerAcceleration() { return playerAcceleration_; }
	void SetPlayerAcceleration(const Vector3& playerAcceleration) { playerAcceleration_ = playerAcceleration; }

	// フックの最大移動制限
	Vector3 GetMaxMoveLimit() { return maxMoveLimit_; }
	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }

	// フックの最小移動制限
	Vector3 GetMinMoveLimit() { return minMoveLimit_; }
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }

	// フックの終点
	Vector3 GetEndPos() { return endPos_; }
	void SetEndPos(const Vector3& endPos) { endPos_ = endPos; }

	// フックの速度
	float GetSpeed() { return speed_; }
	void SetSpeed(float speed) { speed_ = speed; }

	// フックのアクティブかどうか
	bool GetIsActive() { return isActive_; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }

	// フックの伸びているかどうかを取得
	bool GetEnemyHit() { return enemyHit_; }
	void SetEnemyHit(bool enemyHit) { enemyHit_ = enemyHit; }

	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:
	// 開始位置
	Vector3 startPos_;

	// 現在位置
	Vector3 currentPos_;

	// 終了位置
	Vector3 endPos_ = {};

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 角速度
	Vector3 angularVelocity_;

	// 方向
	Vector3 direction_;

	// 速さ
	float speed_ = 5.0f;

	// 最大距離
	float maxDistance_ = 50.0f;

	// 開始時間
	std::chrono::steady_clock::time_point startTime_;

	// アクティブかどうか
	bool isActive_ = false;
	// 伸びているかどうか
	bool isExtending_ = false;

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = {8.0f, 0.0f, 8.0f};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = {-8.0f, 0.0f, -8.0f};

	// プレイヤーの回転
	Vector3 playerRotation_ = {0.0f, 0.0f, 0.0f};

	// プレイヤーの位置
	Vector3 playerPosition_ = {0.0f, 0.0f, 0.0f};
	// プレイヤーの速度
	Vector3 playerVelocity_ = {};
	// プレイヤーの加速度
	Vector3 playerAcceleration_ = {};

	bool isMoving_ = false;

	Vector3 potentialEndPos;

	// 敵のリスト
	std::vector<Enemy*> enemies_;

	bool isThrowing_;

	Vector3 enemyHitPosition_; // 敵に当たった位置を保存するメンバ変数
	bool enemyHit_;            // 敵に当たったかどうかを示すフラグ
	float distanceToEnemy;     // 敵に当たったときの距離を保存するメンバ変数

	// フックを投げる前に敵に当たったかどうか
	bool hookToEnemyHitBeforeThrow_ = false;

	// フックの引っ張るフラグ
	bool isPulling_ = false;

	//================================================
	// Behavior

	// 状態
	Behavior behavior_ = Behavior::None;

	// 状態リクエスト
	std::optional<Behavior> requestBehavior_ = std::nullopt;
};
