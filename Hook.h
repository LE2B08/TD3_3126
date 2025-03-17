#pragma once
#include "Collider.h"
#include "Object3D.h"
#include "GamePlayScene/Enemy/Enemy.h"


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

	// 投げる処理
	void Throw();

	// 移動処理
	void Move();

	// 元の位置に戻る処理
	void Extend();

	// 衝突判定
	void OnCollision(Collider* other) override;
	
	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	

	void ShowImGui();

///-------------------------------------------/// 
/// ゲッター & セッター
///-------------------------------------------///
public:

	void SetPlayerRotation(const Vector3& playerRotation) { playerRotation_ = playerRotation; }

	void SetPlayerPosition(const Vector3& playerPosition) { playerPosition_ = playerPosition; }
	
	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }

	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }

	Vector3 GetEndPos() { return endPos_; }

	float GetSpeed() { return speed_; }

	bool GetIsActive() { return isActive_; }

	void SetIsActive(bool isActive) { isActive_ = isActive; }

	bool GetEnemyHit() { return enemyHit_; }

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
	const float speed_ = 5.0f;

	// 最大距離
	float maxDistance_ = 50.0f;

	// 開始時間
	std::chrono::steady_clock::time_point startTime_;

	// アクティブかどうか
	bool isActive_ = false;
	// 伸びているかどうか
	bool isExtending_ = false;

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = { 8.0f, 0.0f, 8.0f };
	//  移動制限の最小値
	Vector3 minMoveLimit_ = { -8.0f, 0.0f, -8.0f };

	// プレイヤーの回転
	Vector3 playerRotation_ = { 0.0f, 0.0f, 0.0f };

	// プレイヤーの位置
	Vector3 playerPosition_ = { 0.0f, 0.0f, 0.0f };

	bool isMoving_ = false;

	Vector3 potentialEndPos;

	// 敵のリスト
	std::vector<Enemy*> enemies_;

	// 新しいフラグ
	bool isThrowing_;

	Vector3 enemyHitPosition_; // 敵に当たった位置を保存するメンバ変数
	bool enemyHit_;            // 敵に当たったかどうかを示すフラグ
	float distanceToEnemy;     // 敵に当たったときの距離を保存するメンバ変数

	// フックを投げる前に敵に当たったかどうか
	bool hookToEnemyHitBeforeThrow_ = false;

};

