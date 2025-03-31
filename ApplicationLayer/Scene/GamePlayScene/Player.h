#pragma once
#include <Object3D.h>
#include"Collider.h"
#include "Hook.h"
#include "Weapon.h"
#include"PlayerUI.h"
#include "CollisionManager.h"
#include "Enemy/Enemy.h"
#include <ParticleManager.h>
#include <ParticleEmitter.h>

class Player :public Collider {
public:
	///============================
	/// メンバ関数
	/// 
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void DrawImGui();
	// 移動処理
	void Move();

	//回転処理
	void Rotate();

	// 攻撃処理
	void Attack();

	// ヒット時のHP減少処理
	void DecreaseHpOnHit();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	


	/*------ヒット時のパーティクル------*/
	void HitParticle();

	

public:
	///============================
	/// Getter & Setter

	// 位置の取得
	Vector3 GetPosition() const { return position_; }
	// 位置の設定
	void SetPosition(const Vector3& position) { position_ = position; }
	// 回転の取得
	Vector3 GetRotation() const { return rotation_; }
	// 回転の設定
	void SetRotation(const Vector3& rotation) { rotation_ = rotation; }
	// スケールの取得
	Vector3 GetScale() const { return scale_; }
	// スケールの設定
	void SetScale(const Vector3& scale) { scale_ = scale; }
	// 速度の取得
	Vector3 GetVelocity() const { return velocity_; }
	// 速度の設定
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	// 加速度の取得
	Vector3 GetAcceleration() const { return acceleration_; }
	// 加速度の設定
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
	// 角速度の取得
	Vector3 GetAngularVelocity() const { return angularVelocity_; }
	// 角速度の設定
	void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; }

	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }

	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }


	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
  
	

	bool GetIsHit() const { return isHit_; }	

	void SetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; }

	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack() const { return isAttack_; }

	void SetIsHitEnemy(bool isHitEnemy) { isHitEnemy_ = isHitEnemy; }
	bool GetIsHitEnemy() const { return isHitEnemy_; }
  


	void SetHp(const int& hp) { hp_ = hp; }
	int GetHp() const { return hp_; }

private:
	///============================
	/// メンバ変数
	///

	WorldTransform worldTransform_;

	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;

	// 位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// 角速度
	Vector3 angularVelocity_;

	/*----- 移動制限-----*/
	//  移動制限の最大値
	Vector3 maxMoveLimit_ = { 8.0f, 0.0f, 8.0f };
	//  移動制限の最小値
	Vector3 minMoveLimit_ = { -8.0f, 0.0f, -8.0f };

	
	

	/*-----DebugMode-----*/
	//デバッグフラグ
	bool isDebug_ = false;

	/*------当たり判定マネージャ------*/
	std::unique_ptr<CollisionManager> collisionManager_;

	/*------パーティクル------*/
	ParticleManager* particleManager_;
	std::unique_ptr<ParticleEmitter> particleEmitter_;

	/*------敵------*/
	Enemy* enemy_;

	/*------ヒット判定------*/
	//
	bool isHit_ = false;

	/*------ヒットの時間------*/
	float hitTime_ = 0.0f;

	/*------ヒットの最大時間------*/
	float hitMaxTime_ = 2.0f;


	// ゲームが開始しているか
	bool isGameStart_ = false;

	// プレイヤーが敵に当たったか
	bool isHitEnemy_ = false;

	// HP
	int hp_ = 10;
	// 無敵時間
	int invincibleTime_ = 0;
	// 無敵時間の最大値
	const int maxInvincibleTime_ = 60;

	// 無敵状態かどうか
	bool isInvincible_ = false;

	//攻撃しているかどうか
	bool isAttack_ = false;



};
