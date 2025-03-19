#pragma once
#include "Object3D/Object3D.h"
#include "Collider.h"
#include <memory>
#include <optional>
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"

/// ===== 前方宣言 ===== ///
class Player;
class EnemyBullet;
class AttackCommand;
class ParticleManager;

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

	// ヒット時のパーティクル
	void HitParticle();

	// シリアルナンバーを取得
	uint32_t GetSerialNumber() const { return serialNumber_; }

	// シリアルナンバーを設定
	void SetSerialNumber(uint32_t serialNumber) { serialNumber_ = serialNumber; }

	// 向きをランダムに設定
	Vector3 RondomDirection(float min, float max);

	// 攻撃コマンドをランダムに設定
	std::unique_ptr<AttackCommand> RandomAttackCommand();

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
	/// 探索時初期化
	/// </summary>
	void BehaviorSarchInitialize();

	/// <summary>
	/// 探索時更新
	/// </summary>
	void BehaviorSarchUpdate();

	/// <summary>
	/// 攻撃時初期化
	/// </summary>
	void BehaviorAttackInitialize();

	/// <summary>
	/// 攻撃時更新
	/// </summary>
	void BehaviorAttackUpdate();

	///-------------------------------------------/// 
	/// ゲッター&セッター
	///-------------------------------------------///
public:

	void SetPlayer(Player* player) { player_ = player; }

	/*------ヒットの取得、セット------*/
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	bool GetIsHit() const { return isHit_; }

	void SetIsHitFromAttack(bool isHitFromAttack) { isHitFromAttack_ = isHitFromAttack; }

	/*------弾の取得------*/
	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	/// <summary>
	/// 移動制限の最大値のセッター
	/// </summary>
	/// <param name="maxMoveLimit"></param>
	void SetMaxMoveLimit(const Vector3& maxMoveLimit) { maxMoveLimit_ = maxMoveLimit; }

	/// <summary>
	/// 移動制限の最小値のセッター
	/// </summary>
	/// <param name="minMoveLimit"></param>
	void SetMinMoveLimit(const Vector3& minMoveLimit) { minMoveLimit_ = minMoveLimit; }

	///-------------------------------------------/// 
	/// 列挙
	///-------------------------------------------///
public:

	enum class Behavior {

		Normal, // 通常
		Sarch, 	// 探索
		Attack, // 攻撃
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

	// 向き
	Vector3 direction_ = {};

	ParticleManager* particleManager_ = nullptr;

	// プレイヤー
	Player* player_;

	// 状態
	Behavior behavior_ = Behavior::Normal;

	// 状態リクエスト
	std::optional<Behavior> requestBehavior_ = std::nullopt;

	//  移動制限の最大値
	Vector3 maxMoveLimit_ = {};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = {};

	// 弾のリスト
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	// コマンド配列
	std::vector<std::unique_ptr<AttackCommand>> attackCommands_;

	// アタックコマンド
	std::unique_ptr<AttackCommand> attackCommand_;

	// 発見までの距離
	const float foundDistance_ = 4.0f;

	// 各状態で使うカウントダウンタイマー
	float stateTimer_ = 5.0f;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;

	/*------パーティクル------*/
	std::unique_ptr<ParticleEmitter> particleEmitter_;

	/*------ヒットフラグ------*/
	bool isHit_ = false;

	/*------プレイヤーの攻撃によるヒット------*/
	bool isHitFromAttack_ = false;

	/*------ヒットの時間------*/
	float hitTime_ = 0.0f;

	/*------ヒットの最大時間------*/
	float hitMaxTime_ = 60.0f;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;
	// 次のシリアルナンバー
	uint32_t nextSerialNumber_;

	// 乱数生成器
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
};

