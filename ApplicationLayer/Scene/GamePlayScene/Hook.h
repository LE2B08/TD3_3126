#pragma once
#include "Object3D.h"
#include "Collider.h"

#include <chrono>
#include <deque>
#include <optional>

/// ---------- 前方宣言 ---------- ///
class Player;
class Enemy;
class Field;


/// -------------------------------------------------------------
///						　フッククラス
/// -------------------------------------------------------------
class Hook : public Collider
{
private: /// ---------- 列挙型 ---------- ///

	// 振る舞い
	enum class Behavior
	{
		None,   // なし
		Throw,  // 投げる
		Extend, // 伸ばす
		Move,   // 移動
		Back    // 戻す
	};

public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

public: /// ---------- ゲッター ---------- ///

	// プレイヤーの回転を取得
	Vector3 GetPlayerRotation() { return playerRotation_; }

	// プレイヤーの位置を取得
	Vector3 GetPlayerPosition() { return playerPosition_; }

	// プレイヤーの速度を取得
	Vector3 GetPlayerVelocity() { return playerVelocity_; }

	// プレイヤーの加速度を取得
	Vector3 GetPlayerAcceleration() { return playerAcceleration_; }

	// 最大移動範囲を取得
	Vector3 GetMaxMoveLimit() { return maxMoveLimit_; }
	
	// 最小移動範囲を取得
	Vector3 GetMinMoveLimit() { return minMoveLimit_; }
	
	// 終点を取得
	Vector3 GetEndPos() { return endPos_; }

	// フックの速さを取得
	float GetSpeed() { return speed_; }

	// フックの状態を取得
	bool GetIsActive() { return isActive_; }
	
	// エネミーの状態を取得
	bool GetEnemyHit() { return enemyHit_; }
	
	// フックを投げる前に敵に当たったかどうかを取得
	bool GetIsHitPlayerToEnemy() { return isHitPlayerToEnemy_; }

public: /// ---------- セッター ---------- ///

	// プレイヤーの位置を設定
	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }

	// プレイヤーの回転を設定
	void SetPlayerRotation(const Vector3& playerRotation) { playerRotation_ = playerRotation; }
	
	// プレイヤーの速度を設定
	void SetPlayerVelocity(const Vector3& playerVelocity) { playerVelocity_ = playerVelocity; }
	
	// プレイヤーの加速度を設定
	void SetPlayerAcceleration(const Vector3& playerAcceleration) { playerAcceleration_ = playerAcceleration; }
	
	// 最大移動範囲を設定
	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }

	// 最小移動範囲を設定
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }
	
	// フックの終点を設定
	void SetEndPos(const Vector3& endPos) { endPos_ = endPos; }
	
	// フックの速さを設定
	void SetSpeed(float speed) { speed_ = speed; }
	
	// フックの状態を設定
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	
	// フックを投げる前に敵に当たったかどうかを設定
	void SetEnemyHit(bool enemyHit) { enemyHit_ = enemyHit; }
	
	// フックを投げる前に敵に当たったかどうかを設定
	void SetIsHitPlayerToEnemy(bool isHitPlayerToEnemy) { isHitPlayerToEnemy_ = isHitPlayerToEnemy; }

	// プレイヤーの設定
	void SetPlayer(Player* player) { player_ = player; }

	// 敵の設定
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

	// フィールドの設定
	void SetField(Field* field) { field_ = field; }

private:  /// ---------- ルートビヘイビア用メンバ関数 ---------- ///

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

private: /// ---------- メンバ変数 ---------- ///

	// プレイヤー
	Player* player_ = nullptr;

	// エネミー
	Enemy* enemy_ = nullptr;

	// フィールド
	Field* field_ = nullptr;

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = { 8.0f, 0.0f, 8.0f };
	//  移動制限の最小値
	Vector3 minMoveLimit_ = { -8.0f, 0.0f, -8.0f };

	// プレイヤーの回転
	Vector3 playerRotation_ = { 0.0f, 0.0f, 0.0f };

	// プレイヤーの位置
	Vector3 playerPosition_ = { 0.0f, 0.0f, 0.0f };

	// プレイヤーの速度
	Vector3 playerVelocity_ = {};
	// プレイヤーの加速度
	Vector3 playerAcceleration_ = {};

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

	bool isHitPlayerToEnemy_ = false;

	// フックの引っ張るフラグ
	bool isPulling_ = false;



	//================================================
	// Behavior

	// 状態
	Behavior behavior_ = Behavior::None;

	// 状態リクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//
	Vector2 rightStick_ = {};
	bool isRightStickRight = false;
	bool isRightStickLeft = false;

	///===================================
	/// 弧
	/// 

	float decelerationRate = 0.95f; // 減速率（調整可能）
	float angle;                    // 角度
	float angularSpeed = 3.0f;      // 角速度（調整可能）
	float initialAngularSpeed = 0.0f;   // 角速度の初期値
	float maxAngularSpeed = 5.0f;       // 角速度の最大値
	float angularSpeedIncrement = 2.0f; // 角速度の増加率

};
