#define NOMINMAX
#include "CollisionTypeIdDef.h"
#include "Enemy.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "ParticleManager.h"
#include "Weapon.h"
#include "Wireframe.h"
#include <Easing.h>
#include "TutorialPlayer.h"
using namespace Easing;

/// -------------------------------------------------------------
///						　コンストラクタ
/// -------------------------------------------------------------
TutorialPlayer::TutorialPlayer() {
	// シリアルナンバーを振る
	serialNumber_ = nextSerialNumber_;
	// 次のシリアルナンバーに1を足す
	++nextSerialNumber_;
}

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void TutorialPlayer::Initialize() {
	// 基底クラスの初期化
	BaseCharacter::Initialize();

	// プレイヤーのコライダーの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	// パーティクルマネージャの生成
	particleManager_ = ParticleManager::GetInstance();

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/gradationLine.png");

	// パーティクルグループの追加
	particleManager_->CreateParticleGroup("PlayerHitParticles", "gradationLine.png", ParticleEffectType::Ring);

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(particleManager_, "PlayerHitParticles");

	// オブジェクトの生成・初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Voxel_Human.gltf");
	worldTransform_.translate_ = { 8.0f, 0.0f, 8.0f };
}

/// -------------------------------------------------------------
///						　	　更新処理
/// -------------------------------------------------------------
void TutorialPlayer::Update() {
	// 無敵時間のカウントダウン
	if (isInvincible_) {
		isEnemyHit_ = true; // 敵に当たったフラグを立てる

		invincibleTime_ += 1; // 1フレームごとにカウントアップ

		if (invincibleTime_ > invincibleDuration_) {
			isInvincible_ = false; // 無敵状態を解除
			invincibleTime_ = 0;   // 無敵時間の初期化
			isEnemyHit_ = false;   // 敵に当たったフラグを解除
		}
	}

	// 武器の更新
	if (weapon_) {
		weapon_->Update();
	}

	// 攻撃を入力したら攻撃開始
	if (input_->TriggerButton(XButtons.L_Shoulder) && behavior_ == Behavior::kRoot) {
		behaviorRequest_ = Behavior::kAttack;
	}

	// ビヘイビア遷移
	if (behaviorRequest_) {
		// ビヘイビアを変更
		behavior_ = behaviorRequest_.value();

		// 各ビヘイビアごとの初期化を実行
		switch (behavior_) {
			/// ----- ルートビヘイビア ----- ///
		case Behavior::kRoot:
		default:

			// 通常行動の初期化
			BehaviorRootInitialize();
			break;

			/// ----- アタックビヘイビア ----- ///
		case Behavior::kAttack:

			// 攻撃行動の初期化
			BehaviorAttackInitialize();
			break;
		}

		// ビヘイビアリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// ビヘイビアの実行
	switch (behavior_) {
		/// ----- ルートビヘイビア ----- ///
	case Behavior::kRoot:
	default:

		// 通常行動の更新
		BehaviorRootUpdate();
		break;

		/// ----- アタックビヘイビア ----- ///
	case Behavior::kAttack:

		// 攻撃行動の更新
		BehaviorAttackUpdate();
		break;
	}
}

/// -------------------------------------------------------------
///						　	描画処理
/// -------------------------------------------------------------
void TutorialPlayer::Draw() {
	if (!isInvincible_ || static_cast<int>(invincibleTime_) % 2 == 0) {
		// 基底クラスの描画処理
		BaseCharacter::Draw();
	}

	// 武器の描画
	if (isAttack_ && weapon_) {
		weapon_->Draw();
	}
#ifdef _DEBUG
	// プレイヤーの向きを示す線を描画
	Vector3 direction = { cos(worldTransform_.rotate_.y), 0.0f, sin(worldTransform_.rotate_.y) };
	Vector3 endPos = worldTransform_.translate_ + -direction * 5.0f;                                  // 5.0fは線の長さ
	Wireframe::GetInstance()->DrawLine(worldTransform_.translate_, endPos, { 0.0f, 1.0f, 0.0f, 1.0f }); // 緑色の線
#endif
}

/// -------------------------------------------------------------
///							衝突判定処理
/// -------------------------------------------------------------
void TutorialPlayer::OnCollision(Collider* other) {
	// 種別IDを取得
	uint32_t typeID = other->GetTypeID();

	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) // 敵と衝突した場合
	{
		if (!isInvincible_) {
			//hp_ -= 2;

			if (hp_ <= 0) {
				// isDead_ = true; // 死亡フラグを立てる
				isEnemyHit_ = false; // 敵に当たったフラグを解除
			}
			isInvincible_ = true; // 無敵状態にする
			invincibleTime_ = 0;  // 無敵時間の初期化
		}
	}
	else if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemyBullet)) // 敵の弾と衝突した場合
	{
		if (!isInvincible_) {
			//hp_ -= 1;

			// if (hp_ <= 0)
			//{
			//	// 死亡の判定はDeadEffectの方に移動させました
			//	// 演出入れてから画面遷移させたかったのでね byサトウ
			//	//isDead_ = true; // 死亡フラグを立てる
			// }
			isInvincible_ = true; // 無敵状態にする
			invincibleTime_ = 0;  // 無敵時間の初期化
		}
	}
}

/// -------------------------------------------------------------
///				　中心座標を取得する純粋仮想関数
/// -------------------------------------------------------------
Vector3 TutorialPlayer::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // プレイヤーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}

void TutorialPlayer::DrawImGui() {
	ImGui::Begin("PlayerTutorial");
	ImGui::SliderFloat3("playerPosition", &worldTransform_.translate_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("playerRotation", &worldTransform_.rotate_.x, -20.0f, 10.0f);
	ImGui::SliderFloat3("playerScale", &worldTransform_.scale_.x, 0.0f, 10.0f);
	ImGui::SliderInt("hp", &hp_, 0, 10);
	ImGui::End();
}

/// -------------------------------------------------------------
///						　　移動処理
/// -------------------------------------------------------------
void TutorialPlayer::Move() {
	// 速度の加算
	velocity_ += acceleration_;

	// 速度制限
	const float maxSpeed = 10.0f;
	float speed = Vector3::Length(velocity_);
	if (speed > maxSpeed) {
		velocity_ = (velocity_ / speed) * maxSpeed;
	}

	// 減速処理
	velocity_ *= 0.98f; // 摩擦を適用

	// 最終的な移動適用
	worldTransform_.translate_ += velocity_;

	// 加速度をリセット（加速度は一時的なものが多いので）
	acceleration_ = { 0.0f, 0.0f, 0.0f };

	/// ---------- 回転処理 ---------- ///

	// 右スティックの入力があるとき
	// 右スティックの入力があるとき
	if (!input_->RStickInDeadZone() && hp_ > 0) {
		worldTransform_.rotate_.y = -atan2(input_->GetRightStick().x, input_->GetRightStick().y) - std::numbers::pi_v<float> / 2.0f;
	}

	if (worldTransform_.rotate_.y != prevRotateY_) {
		isRotation_ = true;
	}
	else {
		isRotation_ = false;
	}
	prevRotateY_ = worldTransform_.rotate_.y;



	// 移動制限
	worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
	worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);

	// オブジェクトの更新
	object3D_->SetScale(worldTransform_.scale_);
	object3D_->SetRotate(worldTransform_.rotate_);
	object3D_->SetTranslate(worldTransform_.translate_);
	object3D_->Update();
}

/// -------------------------------------------------------------
///					ヒット時のパーティクル処理
/// -------------------------------------------------------------
void TutorialPlayer::HitParticle() {
	// エネミーの中心位置を取得
	Vector3 playerCenter = GetCenterPosition();

	// パーティクルエミッターの位置をエネミーの中心に設定
	particleEmitter_->SetPosition(playerCenter);
	particleEmitter_->SetEmissionRate(3.0f); // パーティクルの発生率を設定
	// パーティクルを生成
	particleEmitter_->Update(1.0f / 60.0f); // deltaTime は 0 で呼び出し
}

void TutorialPlayer::DeadEffect() {
	// プレイヤーを回転させながら小さくなって消滅する
	Vector3 scale = worldTransform_.scale_;
	Vector3 rotation = worldTransform_.rotate_;

	Vector3 rotationEnd = { 0.0f, -17.2f, 0.0f }; // 回転の最終値
	if (rotationStartT_ >= rotationMaxT_) {
		rotationStartT_ = rotationMaxT_; // 回転時間を最大値に設定
		isDead_ = true;                  // 死亡フラグを立てる
	}
	else {
		rotationStartT_ += 1.0f; // 回転時間をカウントアップ
	}
	worldTransform_.rotate_ = Vector3::Lerp(rotation, rotationEnd, Easing::easeInOut(rotationStartT_ / rotationMaxT_)); // 回転を補間
	worldTransform_.scale_ = Vector3::Lerp(scale, Vector3(0.0f, 0.0f, 0.0f), Easing::easeInOutElastic(rotationStartT_ / rotationMaxT_));
}

/// -------------------------------------------------------------
///					通常行動の初期化処理
/// -------------------------------------------------------------
void TutorialPlayer::BehaviorRootInitialize() {
	isAttack_ = false;  // 攻撃フラグをリセット
	attackTime_ = 0.0f; // 攻撃時間をリセット
}

/// -------------------------------------------------------------
///					通常行動の更新処理
/// -------------------------------------------------------------
void TutorialPlayer::BehaviorRootUpdate() {
	// 移動処理
	Move();
}

/// -------------------------------------------------------------
///					攻撃行動の初期化処理
/// -------------------------------------------------------------
void TutorialPlayer::BehaviorAttackInitialize() {
	attackTime_ = 0.0f; // 攻撃時間をリセット
	isAttack_ = true;   // 攻撃フラグを立てる
}

/// -------------------------------------------------------------
///					攻撃行動の更新処理
/// -------------------------------------------------------------
void TutorialPlayer::BehaviorAttackUpdate() {

	// 攻撃中も移動できるようにする
	velocity_ *= 0.5f; // 移動速度を半分にする
	Move();

	// 攻撃時間をカウント
	attackTime_++;

	// 攻撃時間が最大値に達したら攻撃フラグをリセット
	if (attackTime_ >= attackMaxTime_) {
		isAttack_ = false;
		behaviorRequest_ = Behavior::kRoot;
	}
	else {
		weapon_->Attack();
	}
}