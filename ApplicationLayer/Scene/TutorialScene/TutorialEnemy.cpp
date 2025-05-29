#include "TutorialEnemy.h"
#include "AttackCommand.h"
#include "CollisionTypeIdDef.h"
#include "EnemyBullet.h"

#include"TutorialPlayer.h"
#include "Vector3.h"
#include <Easing.h>
#include <Wireframe.h>
#include <algorithm>
#include <imgui.h>
using namespace Easing;

/// -------------------------------------------------------------
///						　コンストラクタ
/// -------------------------------------------------------------
TutorialEnemy::TutorialEnemy() {
	serialNumber_ = nextSerialNumber_;
	nextSerialNumber_++;

	randomEngine.seed(seedGenerator());
}

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void TutorialEnemy::Initialize() {
	// 基底クラスの初期化
	BaseCharacter::Initialize();

	// コライダーの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kEnemy));

	// ワールド変換の初期化
	worldTransform_.Initialize();
	// 画面外に移動させる
	worldTransform_.translate_ = { 0.0f, 100.0f, 8.0f };
	// オブジェクトの生成・初期化
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("Voxel_Enemy.gltf");

	particleManager_ = ParticleManager::GetInstance();
	TextureManager::GetInstance()->LoadTexture("Resources/gradationLine.png");
	// パーティクルグループの追加
	particleManager_->CreateParticleGroup("EnemyHitParticles", "gradationLine.png", ParticleEffectType::Ring);

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(particleManager_, "EnemyHitParticles");
	hitTime_ = 0;

	// 体力を20設定
	hp_ = 20;
}

/// -------------------------------------------------------------
///						　	更新処理
/// -------------------------------------------------------------
void TutorialEnemy::Update() {
	// 基底クラスの更新
	BaseCharacter::Update();

	/*------ヒット時の処理------*/
	if (isHit_) {

		// プレイヤーからの攻撃だったら
		if (isHitFromAttack_) {

			// ヒット時のパーティクルを生成
			HitParticle();

			// ノックバックさせる
			KnockBack();

			hitTime_++;
			// タイマーが最大値に達したらヒットフラグをオフにする
			if (hitTime_ >= hitMaxTime_) {
				isInvincible_ = false; // 無敵状態を解除
				isHit_ = false;
				isHitFromAttack_ = false;
				hitTime_ = 0;
			}
		}
		else {
			isHit_ = false;
		}
	}

	// 無敵時間のカウントダウン
	if (isInvincible_) {

		invincibleTime_ += 1; // 1フレームごとにカウントアップ

		if (invincibleTime_ > invincibleDuration_) {

			invincibleTime_ = 0; // 無敵時間の初期化
		}
	}

	Move();

	// Object3Dの更新
	object3D_->SetScale(worldTransform_.scale_); // スケールを設定
	object3D_->SetRotate(worldTransform_.rotate_);
	object3D_->SetTranslate(worldTransform_.translate_);
	object3D_->Update();
}

/// -------------------------------------------------------------
///						　描画処理
/// -------------------------------------------------------------
void TutorialEnemy::Draw() {
	// 無敵時間中は描画しない
	if (!isInvincible_ || static_cast<int>(invincibleTime_) % 2 == 0) {
		// 基底クラスの描画
		BaseCharacter::Draw();
	}

}

/// -------------------------------------------------------------
///						　移動処理
/// -------------------------------------------------------------
void TutorialEnemy::Move() {

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;

	// 向きを合わせる
	worldTransform_.rotate_.y = std::atan2(-direction_.z, -direction_.x);

	// 移動制限
	worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
	worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);

}

/// -------------------------------------------------------------
///						　ImGuiの描画
/// -------------------------------------------------------------
void TutorialEnemy::ShowImGui(const char* name) {

	ImGui::Begin(name);

	// ImGui::Text("isInvincible : %s", isInvincible_ ? "true" : "false");
	// ImGui::DragFloat3("Rotate", &worldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
	ImGui::DragFloat3("Position", &worldTransform_.translate_.x, 0.01f);
	ImGui::DragFloat3("Direction", &direction_.x, 0.01f);
	// ImGui::SliderFloat("Time", &stateTimer_, 0.0f, 10.0f);
	ImGui::Text("isHit : %s", isHit_ ? "true" : "false");
	ImGui::Text("isHitFromAttack : %s", isHitFromAttack_ ? "true" : "false");
	ImGui::Text("HitTime : %f", hitTime_);
	// ImGui::SliderFloat("HitMaxTime", &hitMaxTime_, 0.0f, 600.0f);
	// ImGui::Text("HP : %d", hp_);
	// ImGui::Text("isEnemyCameraEffect : %s", isEnemyCameraEffect_ ? "true" : "false");
	// ImGui::Text("isCameraBackEffect : %s", isCameraBackEffect_ ? "true" : "false");
	// ImGui::Text("isCameraEffectEnd : %s", isCameraEffectEnd_ ? "true" : "false");
	// ImGui::Text("isDead : %s", isDead_ ? "true" : "false");
	ImGui::End();
}

/// -------------------------------------------------------------
///						　衝突判定
/// -------------------------------------------------------------
void TutorialEnemy::OnCollision(Collider* other) {
	isHit_ = true;

	// Idを取得
	uint32_t typeID = other->GetTypeID();

	// プレイヤーの攻撃だった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kWeapon)) {

		if (!isInvincible_) {
			hp_ -= 1;
			isHitFromAttack_ = true; // プレイヤーの攻撃に当たったフラグを解除
			isInvincible_ = true;    // 無敵状態にする
			invincibleTime_ = 0;     // 無敵時間の初期化
		}
	}
}

/// -------------------------------------------------------------
///						　中心座標を取得
/// -------------------------------------------------------------
Vector3 TutorialEnemy::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // エネミーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}

/// -------------------------------------------------------------
///				　衝突時にパーティクルが発生する
/// -------------------------------------------------------------
void TutorialEnemy::HitParticle() {
	// エネミーの中心位置を取得
	Vector3 enemyCenter = GetCenterPosition();

	// パーティクルエミッターの位置をエネミーの中心に設定
	particleEmitter_->SetPosition(enemyCenter);
	particleEmitter_->SetEmissionRate(0); // パーティクルの発生率を設定
	// パーティクルを生成
	particleEmitter_->Update(1.0f / 60.0f); // deltaTime は 0 で呼び出し
}


/// -------------------------------------------------------------
///					   	ノックバック処理
/// -------------------------------------------------------------
void TutorialEnemy::KnockBack() {

	// プレイヤーの位置を確認
	Vector3 playerPosition = tutorialPlayer_->GetPosition();

	// ノックバック方向のを計算
	Vector3 knockBackDirection = Vector3::Normalize(worldTransform_.translate_ - playerPosition);

	// 敵の向きにノックバックの速度を加算
	velocity_ = knockBackDirection * knockBackSpeed_; // ノックバックの速度を加算
}