#pragma once
#include "Collider.h"
#include "Object3D.h"

#include <chrono>
#include <deque>
#include <optional>

/// ---------- 前方宣言 ---------- ///
class Player;
class TutorialPlayer;
class Enemy;
class TutorialEnemy;
class Field;

/// -------------------------------------------------------------
///						　フッククラス
/// -------------------------------------------------------------
class Hook : public Collider {
private:
	///----------------------------
	/// 列挙型
	///----------------------------
	// 振る舞い
	enum class Behavior {
		None,   // なし
		Throw,  // 投げる
		Extend, // 伸ばす
		Move,   // 移動
		Back    // 戻す
	};

public:
	///----------------------------
	/// メンバ関数
	///----------------------------

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// ImGui描画
	void ImGuiDraw();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

public:
	///----------------------------
	/// ゲッター
	///----------------------------

	Vector3 GetPlayerRotation() { return playerRotation_; }
	Vector3 GetPlayerPosition() { return playerPosition_; }
	Vector3 GetPlayerVelocity() { return playerVelocity_; }
	Vector3 GetPlayerAcceleration() { return playerAcceleration_; }
	Vector3 GetMaxMoveLimit() { return maxMoveLimit_; }
	Vector3 GetMinMoveLimit() { return minMoveLimit_; }
	Vector3 GetEndPos() { return endPos_; }
	float GetPullSpeed() { return pullSpeed_; }
	bool GetIsActive() { return isActive_; }
	bool GetIsPulling() { return isPulling_; }
	bool GetIsExtending() { return isExtending_; }
	bool GetIsMoving() { return isMoving_; }
	bool GetIsThrowing() { return isThrowing_; }
	bool GetIsBack() { return isBack_; }
	bool GetEnemyHit() { return enemyHit_; }
	bool GetIsHitPlayerToEnemy() { return isHitPlayerToEnemy_; }
	Vector3 GetEnemyPosition() { return enemyPosition_; }
	bool GetIsArcMove() const { return isArcMove_; }

public:
	///----------------------------
	/// セッター
	///----------------------------

	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }
	void SetPlayerRotation(const Vector3& playerRotation) { playerRotation_ = playerRotation; }
	void SetPlayerVelocity(const Vector3& playerVelocity) { playerVelocity_ = playerVelocity; }
	void SetPlayerAcceleration(const Vector3& playerAcceleration) { playerAcceleration_ = playerAcceleration; }
	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }
	void SetEndPos(const Vector3& endPos) { endPos_ = endPos; }
	void SetPullSpeed(float speed) { pullSpeed_ = speed; }
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	void SetEnemyHit(bool enemyHit) { enemyHit_ = enemyHit; }
	void SetIsHitPlayerToEnemy(bool isHitPlayerToEnemy) { isHitPlayerToEnemy_ = isHitPlayerToEnemy; }
	void SetEnemyPosition(const Vector3& enemyPosition) { enemyPosition_ = enemyPosition; }

	// プレイヤーの設定
	void SetPlayer(Player* player) { player_ = player; }
	void SetTutorialPlayer(TutorialPlayer* player) { tutorialPlayer_ = player; }

	// 敵の設定
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	void SetTutorialEnemy(TutorialEnemy* enemy) { tutorialEnemy_ = enemy; }

	// フィールドの設定
	void SetField(Field* field) { field_ = field; }

private:
	///----------------------------
	/// ビヘイビア状態ごとのメンバ関数
	///----------------------------

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

private:
	///----------------------------
	/// 参照・管理オブジェクト
	///----------------------------

	Player* player_ = nullptr; // プレイヤー
	TutorialPlayer* tutorialPlayer_ = nullptr; // チュートリアルプレイヤー
	Enemy* enemy_ = nullptr;   // エネミー
	TutorialEnemy* tutorialEnemy_ = nullptr; // チュートリアルエネミー
	Field* field_ = nullptr;   // フィールド

	///----------------------------
	/// 移動制限・プレイヤー情報
	///----------------------------

	Vector3 maxMoveLimit_ = {8.0f, 0.0f, 8.0f};   // 移動制限の最大値
	Vector3 minMoveLimit_ = {-8.0f, 0.0f, -8.0f}; // 移動制限の最小値

	Vector3 playerRotation_ = {0.0f, 0.0f, 0.0f}; // プレイヤーの回転
	Vector3 playerPosition_ = {0.0f, 0.0f, 0.0f}; // プレイヤーの位置
	Vector3 playerVelocity_ = {};                 // プレイヤーの速度
	Vector3 playerAcceleration_ = {};             // プレイヤーの加速度

	///----------------------------
	/// フックの座標・物理量
	///----------------------------

	Vector3 startPos_;    // 開始位置
	Vector3 currentPos_;  // 現在位置
	Vector3 endPos_ = {}; // 終了位置

	Vector3 velocity_;        // 速度
	Vector3 acceleration_;    // 加速度
	Vector3 angularVelocity_; // 角速度
	Vector3 direction_;       // 方向

	///----------------------------
	/// フックの速度・挙動パラメータ
	///----------------------------

	float extendSpeed_ = 5.0f;         // フックを張る速さ
	float pullSpeed_ = 30.0f;          // フックを引っ張る速さ
	float backSpeed_ = 15.0f;          // フックの戻る速度
	float backDecelerationRate = 0.9f; // フックの戻る速度の減速率
	float maxDistance_ = 50.0f;        // 最大距離

	std::chrono::steady_clock::time_point startTime_; // 開始時間

	///----------------------------
	/// フックの状態フラグ
	///----------------------------

	bool isActive_ = false;    // アクティブかどうか
	bool isExtending_ = false; // 伸びているかどうか
	bool isMoving_ = false;    // 移動中かどうか
	bool isThrowing_;          // 投げ中かどうか
	bool isPulling_ = false;   // フックの引っ張りフラグ
	bool isBack_ = false;      // フックを戻すフラグ

	///----------------------------
	/// 敵・当たり判定関連
	///----------------------------

	Vector3 enemyHitPosition_;           // 敵に当たった位置
	bool enemyHit_;                      // 敵に当たったかどうか
	float distanceToEnemy;               // 敵に当たったときの距離
	bool isHitEnemyBeforeThrow_ = false; // フックを投げる前に敵に当たったか
	bool isHitPlayerToEnemy_ = false;    // プレイヤーが敵に当たったか
	Vector3 enemyPosition_;              // 敵の位置
	std::vector<Enemy*> enemies_;        // 敵のリスト

	///----------------------------
	/// 弧の移動・引っ張り移動
	///----------------------------

	Vector3 arcMoveVelocity_; // 弧の速度
	Vector3 pullVelocity_;    // 引っ張り移動の速度
	bool isArcMove_ = false;

	///----------------------------
	/// 状態管理
	///----------------------------

	Behavior behavior_ = Behavior::None;                     // 現在の状態
	std::optional<Behavior> behaviorRequest_ = std::nullopt; // 状態リクエスト

	///----------------------------
	/// 入力・壁判定・弧のパラメータ
	///----------------------------

	Vector2 leftStick_ = {};       // 右スティック入力
	bool isLeftStickRight = false; // 右スティック右入力フラグ
	bool isLeftStickLeft = false;  // 右スティック左入力フラグ

	// 壁ヒット判定
	bool isHitWallTop = false;
	bool isHitWallBottom = false;
	bool isHitWallLeft = false;
	bool isHitWallRight = false;

	///----------------------------
	/// 弧のパラメータ
	///----------------------------

	float decelerationRate = 0.95f;     // 減速率（調整可能）
	float angle;                        // 角度
	float angularSpeed = 3.0f;          // 角速度（調整可能）
	float initialAngularSpeed = 0.0f;   // 角速度の初期値
	float maxAngularSpeed = 5.0f;       // 角速度の最大値
	float angularSpeedIncrement = 2.0f; // 角速度の増加率

	///----------------------------
	/// その他
	///----------------------------

	Vector3 potentialEndPos; // 伸ばしきったときの仮の終点
};
