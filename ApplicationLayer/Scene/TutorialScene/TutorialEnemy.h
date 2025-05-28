#pragma once
#include "BaseCharacter.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "TextureManager.h"
#include <DynamicCamera.h>
#include <memory>
#include <optional>

#include "EnemyUI.h"

/// ---------- 前方宣言 ---------- ///
class Player;
class TutorialPlayer;
class EnemyBullet;
class AttackCommand;
class ParticleManager;


/// -------------------------------------------------------------
///						　エネミークラス
/// -------------------------------------------------------------
class TutorialEnemy : public BaseCharacter {
private: /// ---------- 列挙型 ---------- ///


public: /// ---------- メンバ関数 ---------- ///
	/// コンストラクタ
	TutorialEnemy();

	/// 初期化
	void Initialize() override;

	/// 更新
	void Update() override;

	/// 描画
	void Draw() override;

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

	// 攻撃を受けたときのノックバック処理
	void KnockBack();

public: /// ---------- メンバ関数 ・行動別処理 ---------- ///

public: /// ---------- ゲッター ---------- ///
	bool GetIsHit() const { return isHit_; }


	// 位置の取得
	const Vector3& GetPosition() const { return worldTransform_.translate_; }

	// エネミーのカメラ演出用のゲッター
	const bool& GetIsEnemyCameraEffect() const { return isEnemyCameraEffect_; }

	const bool& GetIsCameraEffectEnd() const { return isCameraEffectEnd_; }

	const bool& IsDead() const { return isDead_; }

	// 無敵時間
	const bool& GetIsInvincible() const { return isInvincible_; }

public: /// ---------- セッター ---------- ///
	void SetTutorialPlayer(TutorialPlayer* player) { tutorialPlayer_ = player; }

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

	ParticleManager* particleManager_ = nullptr;


	// チュートリアルプレイヤー
	TutorialPlayer* tutorialPlayer_;


	//  移動制限の最大値
	Vector3 maxMoveLimit_ = {};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = {};

	/*------パーティクル------*/
	std::unique_ptr<ParticleEmitter> particleEmitter_;

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
	float DeathCameraMoveT_ = 0.0f;     // カメラの移動時間
	float DeathCameraMoveMaxT_ = 80.0f; // カメラの移動時間の最大値

	// 死亡フラグ
	bool isDead_ = false;

	// ノックバックする速さ
	float knockBackSpeed_ = 0.2f;
};