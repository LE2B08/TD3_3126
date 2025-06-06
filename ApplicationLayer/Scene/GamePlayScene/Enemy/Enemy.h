#pragma once
#include "BaseCharacter.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include <DynamicCamera.h>
#include <memory>
#include <optional>

#include"EnemyUI.h"

/// ---------- 前方宣言 ---------- ///
class Player;
class TutorialPlayer;
class EnemyBullet;
class AttackCommand;
class ParticleManager;

/// -------------------------------------------------------------
///						　エネミークラス
/// -------------------------------------------------------------
class Enemy : public BaseCharacter {
private: /// ---------- 列挙型 ---------- ///
	// 振る舞い
	enum class Behavior {

		Normal, // 通常
		Sarch,  // 探索
		Attack, // 攻撃
	};

public: /// ---------- メンバ関数 ---------- ///
	/// コンストラクタ
	Enemy();

	/// 初期化
	void Initialize() override;

	/// 更新
	void Update() override;

	/// 描画
	void Draw() override;

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

	// 敵の出現演出
	void SpawnEffect();

	// カメラの動き
	void CameraMove();

	// エネミーが死ぬアニメーション
	void FaildAnimation();

	// エネミーが死んだときのカメラワーク
	void FaildCameraMove();

public: /// ---------- メンバ関数 ・行動別処理 ---------- ///
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
/// クラス内処理
///-------------------------------------------///
private:

	// 攻撃コマンドをランダムに設定
	std::unique_ptr<AttackCommand> RandomAttackCommand();

	// 角度をランダムに設定
	float RandomRadian(float minRadian, float maxRadian);

	/// 移動
	void Move();

	// 壁に当たった時の処理
	void WallHit();

	// 攻撃を受けたときのノックバック処理
	void KnockBack();

public: /// ---------- ゲッター ---------- ///
	bool GetIsHit() const { return isHit_; }

	/*------弾の取得------*/
	std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets_; }

	// 位置の取得
	const Vector3& GetPosition() const { return worldTransform_.translate_; }

	// エネミーのカメラ演出用のゲッター
	const bool& GetIsEnemyCameraEffect() const { return isEnemyCameraEffect_; }

	const bool& GetIsCameraEffectEnd() const { return isCameraEffectEnd_; }

	const bool& IsDead() const { return isDead_; }

	//無敵時間
	const bool& GetIsInvincible() const { return isInvincible_; }

	// ダメージを受けたかどうか
	bool CanGiveDamage();

public: /// ---------- セッター ---------- ///
	void SetPlayer(Player* player) { player_ = player; }

	/*------ヒットの取得、セット------*/
	void SetIsHit(bool isHit) { isHit_ = isHit; }

	void SetIsHitFromAttack(bool isHitFromAttack) { isHitFromAttack_ = isHitFromAttack; }

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

	/// <summary>
	///	エネミーのカメラ演出用のセッター
	/// </summary>
	void SetIsEnemyCameraEffect(bool isEnemyCameraEffect) { isEnemyCameraEffect_ = isEnemyCameraEffect; }

private: /// ---------- メンバ変数 ---------- ///

	// 速度
	Vector3 velocity_;

	// 向き
	Vector3 direction_ = {};

	// 敵の大きさを考慮した座標
	Vector3 minPosition = {};
	Vector3 maxPosition = {};

	// プレイヤー
	Player* player_;

	// チュートリアルプレイヤー
	TutorialPlayer* tutorialPlayer_;

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
	std::unique_ptr<ParticleEmitter> particleEmitter2_;
	std::unique_ptr<ParticleEmitter> particleEmitter3_;

	// 一度だけ出現演出を行うフラグ
	bool hasEmittedDisappearEffect_ = false;

	/*------ヒットフラグ------*/
	bool isHit_ = false;

	/*------無敵時間の設定------*/
	bool isInvincible_ = false;              // 無敵状態かどうか
	const float invincibleDuration_ = 60.0f; // 無敵時間（秒）
	float invincibleTime_ = 0;               // 無敵時間の経過時間

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

	// カメラのタイマー
	float cameraMoveT_ = 0.0f;
	float cameraMoveMaxT_ = 160.0f;

	// エネミーのカメラ演出用フラグ
	bool isEnemyCameraEffect_ = true;

	// エネミーのカメラ演出用のタイマー
	float enemyCameraEffectT_ = 0.0f;

	// カメラ演出の終わり
	bool isCameraEffectEnd_ = false;

	// カメラが戻る演出のフラグ
	bool isCameraBackEffect_ = false;

	// カメラの戻る演出のタイマー
	float cameraBackEffectT_ = 0.0f;
	float cameraBackEffectMaxT_ = 160.0f;

	// 回転の始まり
	float rotationStartT_ = 0.0f;

	// 回転の最大値
	float rotationMaxT_ = 160.0f;

	/*------カメラの移動タイマー------*/
	float DeathCameraMoveT_ = 0.0f; // カメラの移動時間
	float DeathCameraMoveMaxT_ = 80.0f; // カメラの移動時間の最大値

	// 死亡フラグ
	bool isDead_ = false;

	// 移動の速さ
	float moveSpeed_ = 0.1f;

///-------------------------------------------/// 
/// ノックバック処理用の変数
///-------------------------------------------///

	// ノックバック中
	bool isKnockBack_ = false;

	// ノックバックする速さ
	float knockBackSpeed_ = 0.5f;

	// ノックバックの時間
	float knockBackTime_ = 0.0f;
	// ノックバックの最大時間
	float knockBackMaxTime_ = 0.5f; // 0.5秒

///-------------------------------------------/// 
/// 中心に戻る処理用の変数
///-------------------------------------------///

	// 中心に戻るフラグ
	bool isReturnCenter_ = false;

	// 中心に戻る速度
	Vector3 returnVelocity_ = {};

	// 開始位置
	Vector3 returnStartPosition_ = {};

	// 中心座標
	Vector3 centerPosition_ = { 0.0f, 1.0f, 0.0f };

	// 中心に戻る用タイマー
	float returnTimer_ = 0.0f;
	// 中心に戻る最大時間
	float returnMaxTime_ = 1.0f; // 1秒
};
