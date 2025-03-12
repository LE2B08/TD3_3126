#pragma once
#include "Collider.h"
#include "Object3D.h"
#include "GamePlayScene/Enemy/Enemy.h"

#include <optional>

enum class State {

	Idle,  // 待機
	Throw, // 投げる
	Pull,  // 引っ張る
	Back,  // 戻す
};

/// === フック === ///
class Hook : public Collider {

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

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	// ImGui表示
	void ShowImGui();

	///-------------------------------------------/// 
	/// 状態別の処理
	///-------------------------------------------///
	
	// 投げる初期化
	void ThrowInitialize();
	
	// 投げる更新
	void ThrowUpdate();

	// 引っ張る初期化
	void PullInitialize();

	// 引っ張る更新
	void PullUpdate();

	// 戻す初期化
	void BackInitialize();

	// 戻す更新
	void BackUpdate();

	///-------------------------------------------/// 
	/// ゲッター & セッター
	///-------------------------------------------///
public:

	void SetPlayerRotation(const Vector3& playerRotation) { playerRotation_ = playerRotation; }

	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }

	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }

	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }

	Vector3 GetStartPos() { return startPos_; }

	void SetStartPos(Vector3 startPos) { startPos_ = startPos; }

	Vector3 GetEndPos() { return endPos_; }

	float GetSpeed() { return speed_; }

	bool GetIsActive() { return isActive_; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

	State GetState() { return state_; }

	void SetState(State state) { requestState_ = state; }

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
	const float speed_ = 2.0f;

	// 最大距離
	float maxDistance_ = 50.0f;

	// 開始時間
	std::chrono::steady_clock::time_point startTime_;

	// アクティブかどうか
	bool isActive_ = false;

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = { 8.0f, 0.0f, 8.0f };
	//  移動制限の最小値
	Vector3 minMoveLimit_ = { -8.0f, 0.0f, -8.0f };

	// プレイヤーの回転
	Vector3 playerRotation_ = { 0.0f, 0.0f, 0.0f };

	// プレイヤーの位置
	Vector3 playerPosition_ = { 0.0f, 0.0f, 0.0f };

	Vector3 potentialEndPos;

	// 敵のリスト
	std::vector<Enemy*> enemies_;

	// 状態
	State state_ = State::Idle;

	// 状態リクエスト
	std::optional<State> requestState_ = std::nullopt;
};

