#pragma once
#include "BaseCharacter.h"
#include "PlayerUI.h"
#include <ParticleManager.h>
#include <ParticleEmitter.h>

#include <optional>
#include <Easing.h>

/// ---------- 前方宣言 ---------- ///
class Camera;
class Enemy;
class Weapon;
class Hook;


/// -------------------------------------------------------------
///						　プレイヤークラス
/// -------------------------------------------------------------
class Player : public BaseCharacter
{
private: /// ---------- 構造体 ---------- ///

	/// ---------- 振る舞い ---------- ///
	enum class Behavior
	{
		kRoot,	 // 通常状態
		kAttack, // 攻撃状態
	};

public: /// ---------- メンバ関数 ---------- ///

	// コンストラクタ
	Player();

	// 初期化処理
	void Initialize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	// 
	void AppearFromAbove(float t);

	void DeathCameraMove();

	// ImGUi描画処理
	void DrawImGui();

	// スタート時の落下してくる挙動
	void FallingAnimation();

public: /// ---------- ゲッター ---------- ///

	Vector3 GetVelocity() const { return velocity_; }// 速度の取得
	Vector3 GetAcceleration() const { return acceleration_; }// 加速度の取得
	Vector3 GetAngularVelocity() const { return angularVelocity_; }// 角速度の取得

	bool IsDead() const { return isDead_; } // 死亡フラグの取得

	bool GetIsAttack() const { return isAttack_; } // 攻撃の取得
	bool GetIsHitEnemy() const { return isEnemyHit_; }// 敵に当たったかの取得

	bool GetIsFallEnd() const { return isFallEnd_; } // 落下が終わったかの取得

public: /// ---------- セッター ---------- ///

	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }								// 速度の設定
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }				// 加速度の設定
	void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; } // 角速度の設定

	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; } // 移動制限の最大値の設定
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; } // 移動制限の最小値の設定

	void SetIsHitEnemy(bool isHitEnemy) { isEnemyHit_ = isHitEnemy; } // 敵に当たったかの設定
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; } // 攻撃の設定
	void SetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; } // ゲームが開始しているかの設定

	void SetCamera(Camera* camera) { camera_ = camera; } // カメラの設定

	void SetEnemy(Enemy* enemy) { enemy_ = enemy; } // 敵の設定
	void SetWeapon(Weapon* weapon) { weapon_ = weapon; } // 武器の設定
	void SetHook(Hook* hook) { hook_ = hook; } // フックの設定

private: /// ---------- メンバ関数 ---------- ///

	// 移動処理
	void Move();

	// ヒット時のパーティクル
	void HitParticle();

	// 死亡時の演出
	void DeadEffect();

private: /// ---------- ルートビヘイビア用メンバ関数 ---------- ///

	// 通常行動の初期化処理
	void BehaviorRootInitialize();

	// 通常行動の更新処理
	void BehaviorRootUpdate();

	// 攻撃行動の初期化処理
	void BehaviorAttackInitialize();

	// 攻撃行動の更新処理
	void BehaviorAttackUpdate();

private: /// ---------- メンバ変数 ---------- ///

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot; // 現在の行動

	// 次の振る舞いをリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// エネミー
	Enemy* enemy_ = nullptr;

	// 武器
	Weapon* weapon_ = nullptr;

	// フック
	Hook* hook_ = nullptr;

	// 速度
	Vector3 velocity_{};
	// 加速度
	Vector3 acceleration_{};
	// 角速度
	Vector3 angularVelocity_{};

	/*----- 移動制限-----*/
	//  移動制限の最大値
	Vector3 maxMoveLimit_ = { 8.0f * 3.0f, 0.0f, 8.0f * 3.0f};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = { -8.0f * 3.0f, 0.0f, -8.0f * 3.0f };

	/*------パーティクル------*/
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<ParticleEmitter> particleEmitter_;

private: /// ---------- 定数 ---------- ///

	// 無敵時間の設定
	bool isInvincible_ = false;				// 無敵状態かどうか
	const float invincibleDuration_ = 60.0f; // 無敵時間（秒）
	float invincibleTime_ = 0;				// 無敵時間の経過時間

	// 死亡フラグ
	bool isDead_ = false;

	//攻撃しているかどうか
	bool isAttack_ = false;

	// 攻撃の持続時間
	float attackTime_ = 0.0f;

	// 攻撃の最大時間
	float attackMaxTime_ = 60.0f;

	// ゲームが開始しているか
	bool isGameStart_ = false;

	// プレイヤーが敵に当たったか
	bool isEnemyHit_ = false;

	// 回転の始まり
	float rotationStartT_ = 0.0f;

	// 回転の最大値
	float rotationMaxT_ = 160.0f;

	/*------カメラの移動タイマー------*/
	float cameraMoveT_ = 0.0f; // カメラの移動時間
	float cameraMoveMaxT_ = 80.0f; // カメラの移動時間の最大値

///-------------------------------------------/// 
/// 演出用の変数
///-------------------------------------------///

	// タイマー
	float fallingTimer_ = 0;

	// 落下の最大時間(フレーム)
	const float maxFallingTime = 40;

	// 落下アニメーションが終わったかどうか
	bool isFallEnd_ = false;
};
