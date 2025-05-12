#include "Enemy.h"
#include "AttackCommand.h"
#include "CollisionTypeIdDef.h"
#include "EnemyBullet.h"
#include "Player.h"
#include "Vector3.h"
#include <Easing.h>
#include <Wireframe.h>
#include <algorithm>
#include <imgui.h>
using namespace Easing;

/// -------------------------------------------------------------
///						　コンストラクタ
/// -------------------------------------------------------------
Enemy::Enemy() {
	serialNumber_ = nextSerialNumber_;
	nextSerialNumber_++;

	randomEngine.seed(seedGenerator());
}

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void Enemy::Initialize() {
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
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");
	// パーティクルグループの追加
	particleManager_->CreateParticleGroup("EnemyHitParticles", "Resources/uvChecker.png");

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(particleManager_, "EnemyHitParticles");
	hitTime_ = 0;

	// 体力を20設定
	hp_ = 20;
}

/// -------------------------------------------------------------
///						　	更新処理
/// -------------------------------------------------------------
void Enemy::Update() {
	// 基底クラスの更新
	BaseCharacter::Update();


	// 状態の変更がリクエストされている場合
	if (requestBehavior_) {

		// 状態を変更する
		behavior_ = requestBehavior_.value();

		// 状態ごとの初期化を行う
		switch (behavior_) {

		case Enemy::Behavior::Normal:
			BehaviorNormalInitialize();
			break;

		case Enemy::Behavior::Sarch:
			BehaviorSarchInitialize();
			break;

		case Enemy::Behavior::Attack:
			BehaviorAttackInitialize();
			break;

		default:
			break;
		}

		// リクエストをクリア
		requestBehavior_ = std::nullopt;
	}

	// 状態ごとの更新を行う
	switch (behavior_) {

	case Enemy::Behavior::Normal:
		BehaviorNormalUpdate();
		break;

	case Enemy::Behavior::Sarch:
		BehaviorSarchUpdate();
		break;

	case Enemy::Behavior::Attack:
		if (hp_ >= 1) {
			BehaviorAttackUpdate();
		}
		break;

	default:
		break;
	}

	// 弾の削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		// 弾が死んでいる場合
		if (!bullet->IsAlive()) {

			// リセット
			bullet.reset();
			return true;
		}

		return false;
		});

	// カメラの演出が終わるまでかhpが無くなったときは移動しない
	if (isCameraEffectEnd_ && hp_ >= 1) {
		// 移動
		Move();
	}

	// 弾の更新
	for (auto& bullet : bullets_) {
		bullet->Update();
	}

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

			invincibleTime_ = 0;      // 無敵時間の初期化

		}
	}



	// Object3Dの更新
	object3D_->SetScale(worldTransform_.scale_); // スケールを設定
	object3D_->SetRotate(worldTransform_.rotate_);
	object3D_->SetTranslate(worldTransform_.translate_);
	object3D_->Update();
}

/// -------------------------------------------------------------
///						　描画処理
/// -------------------------------------------------------------
void Enemy::Draw() {
	//無敵時間中は描画しない
	if (!isInvincible_ || static_cast<int>(invincibleTime_) % 2 == 0) {
		// 基底クラスの描画
		BaseCharacter::Draw();
	}

	// 弾の描画
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}

#ifdef _DEBUG
	// ワイヤーフレームの処理
	Wireframe::GetInstance()->DrawCircle(worldTransform_.translate_, foundDistance_, 64, { 1.0f, 1.0f, 1.0f, 1.0f });
#endif // _DEBUG
}

/// -------------------------------------------------------------
///						　移動処理
/// -------------------------------------------------------------
void Enemy::Move() {

	// 速度を位置に加算
	worldTransform_.translate_ += velocity_;

	// 向きを合わせる
	worldTransform_.rotate_.y = std::atan2(-direction_.z, -direction_.x);

	// 移動制限
	worldTransform_.translate_.x = std::clamp(worldTransform_.translate_.x, minMoveLimit_.x, maxMoveLimit_.x);
	worldTransform_.translate_.z = std::clamp(worldTransform_.translate_.z, minMoveLimit_.z, maxMoveLimit_.z);

	// 壁に到達したらVelocityを0にする
	if (worldTransform_.translate_.x >= maxMoveLimit_.x || worldTransform_.translate_.x <= minMoveLimit_.x) {
		velocity_.x = 0.0f;
		velocity_.z = 0.0f;
	}

	if (worldTransform_.translate_.z >= maxMoveLimit_.z || worldTransform_.translate_.z <= minMoveLimit_.z) {
		velocity_.x = 0.0f;
		velocity_.z = 0.0f;
	}
}

/// -------------------------------------------------------------
///						　ImGuiの描画
/// -------------------------------------------------------------
void Enemy::ShowImGui(const char* name) {

	ImGui::Begin(name);

	// 状態の表示
	switch (behavior_) {

	case Behavior::Normal:
		ImGui::Text("Behavior: Normal");
		break;

	case Behavior::Sarch:
		ImGui::Text("Behavior: Sarch");
		break;

	case Behavior::Attack:
		ImGui::Text("Behavior: Attack");
		break;

	default:
		break;
	}

	// ボタンを押したらノックバック
	if (ImGui::Button("KnockBack")) {
		KnockBack();
	}

	//ImGui::Text("isInvincible : %s", isInvincible_ ? "true" : "false");
	//ImGui::DragFloat3("Rotate", &worldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);
	//ImGui::DragFloat3("Position", &worldTransform_.translate_.x, 0.01f);
	ImGui::DragFloat3("Direction", &direction_.x, 0.01f);
	//ImGui::SliderFloat("Time", &stateTimer_, 0.0f, 10.0f);
	ImGui::Text("isHit : %s", isHit_ ? "true" : "false");
	ImGui::Text("isHitFromAttack : %s", isHitFromAttack_ ? "true" : "false");
	ImGui::Text("HitTime : %f", hitTime_);
	//ImGui::SliderFloat("HitMaxTime", &hitMaxTime_, 0.0f, 600.0f);
	//ImGui::Text("HP : %d", hp_);
	//ImGui::Text("isEnemyCameraEffect : %s", isEnemyCameraEffect_ ? "true" : "false");
	//ImGui::Text("isCameraBackEffect : %s", isCameraBackEffect_ ? "true" : "false");
	//ImGui::Text("isCameraEffectEnd : %s", isCameraEffectEnd_ ? "true" : "false");
	//ImGui::Text("isDead : %s", isDead_ ? "true" : "false");
	ImGui::End();
}

/// -------------------------------------------------------------
///						　衝突判定
/// -------------------------------------------------------------
void Enemy::OnCollision(Collider* other) {
	isHit_ = true;

	// Idを取得
	uint32_t typeID = other->GetTypeID();

	// プレイヤーの攻撃だった場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kWeapon)) {

		if (!isInvincible_) {
			hp_ -= 1;
			isHitFromAttack_ = true; // プレイヤーの攻撃に当たったフラグを解除
			isInvincible_ = true; // 無敵状態にする
			invincibleTime_ = 0;  // 無敵時間の初期化
		}

	}
}

/// -------------------------------------------------------------
///						　中心座標を取得
/// -------------------------------------------------------------
Vector3 Enemy::GetCenterPosition() const {
	const Vector3 offset = { 0.0f, 0.0f, 0.0f }; // エネミーの中心を考慮
	Vector3 worldPosition = worldTransform_.translate_ + offset;
	return worldPosition;
}

/// -------------------------------------------------------------
///				　衝突時にパーティクルが発生する
/// -------------------------------------------------------------
void Enemy::HitParticle() {
	// エネミーの中心位置を取得
	Vector3 enemyCenter = GetCenterPosition();

	// パーティクルエミッターの位置をエネミーの中心に設定
	particleEmitter_->SetPosition(enemyCenter);
	particleEmitter_->SetEmissionRate(3); // パーティクルの発生率を設定
	// パーティクルを生成
	particleEmitter_->Update(1.0f / 60.0f, ParticleEffectType::Slash); // deltaTime は 0 で呼び出し
}

/// -------------------------------------------------------------
///						ランダム方向処理
/// -------------------------------------------------------------
Vector3 Enemy::RondomDirection(float min, float max) {

	// XZ平面上のランダムな方向を生成
	std::uniform_real_distribution<float> dist(min, max);
	Vector3 direction = { dist(randomEngine), 0.0f, dist(randomEngine) };
	direction = Vector3::Normalize(direction);

	return direction;
}

/// -------------------------------------------------------------
///					攻撃パターンのコマンド処理
/// -------------------------------------------------------------
std::unique_ptr<AttackCommand> Enemy::RandomAttackCommand() {

	// ランダムに攻撃コマンドを選択
	std::uniform_int_distribution<int> dist(0, 3);

	// インデックスに結果を代入
	int commandIndex = dist(randomEngine);

	// 選ばれたコマンドを生成する
	switch (commandIndex) {

	case 0:
		return std::make_unique<FanShotCommand>();

	case 1:
		return std::make_unique<RotateShotCommand>();

	case 2:
		return std::make_unique<RecallCommand>();

	case 3:
		return std::make_unique<SpreadCenterShotCommand>();

	default:
		return std::make_unique<ShotCommand>();
	}
}

void Enemy::SpawnEffect() {
	worldTransform_.translate_ = { 0.0f, 20.0f, 8.0f }; // エネミーの位置を初期化

	// カメラの現在の位置を取得
	Vector3 cameraPosition = camera_->GetTranslate();

	// 移動させるカメラの座標
	Vector3 moveCameraPosition = cameraPosition;

	// カメラの移動後の位置を計算
	Vector3 cameraOffset = { worldTransform_.translate_.x, 1.56f, worldTransform_.translate_.z - 12.0f };

	// カメラの回転を取得
	Vector3 cameraRotation = camera_->GetRotate();

	// 移動させるカメラの回転
	Vector3 moveCameraRotation = cameraRotation;

	if (cameraMoveT_ >= cameraMoveMaxT_) {
		cameraMoveT_ = cameraMoveMaxT_;    // カメラの移動時間を最大値に設定
		enemyCameraEffectT_ += kDeltaTime; // カメラの演出時間をカウントアップ
		// 2秒数えてフラグをオフにする
		if (enemyCameraEffectT_ >= 2.0f) {
			enemyCameraEffectT_ = 0.0f;
			isCameraBackEffect_ = true; // カメラの演出フラグをオンにする
		}
	}
	else {
		cameraMoveT_ += 1.0f; // カメラの移動時間をカウントアップ
		worldTransform_.rotate_.y = 1.5f;
	}

	moveCameraPosition = Vector3::Lerp(cameraPosition, cameraOffset, easeIn(cameraMoveT_ / cameraMoveMaxT_)); // カメラの位置を補間

	float t = cameraMoveT_ / cameraMoveMaxT_;
	moveCameraRotation.x = bezierCurve(t, 0.0f, -1.0f, -1.0f, 0.0f); // カメラの回転をベジエ曲線で補間

	worldTransform_.translate_ = Vector3::Lerp(worldTransform_.translate_, Vector3(0.0f, 1.0f, 8.0f), easeIn(cameraMoveT_ / cameraMoveMaxT_)); // エネミーの位置を補間
	// moveCameraRotation = Vector3::Lerp(cameraRotation, Vector3(0.0f, 0.0f, 0.0f), -1.0f * easeOutBounce(cameraMoveT_ / cameraMoveMaxT_)); // カメラの回転を補間
	//  カメラの位置をプレイヤーの位置に設定
	camera_->SetTranslate(cameraOffset);

	camera_->SetRotate(moveCameraRotation); // カメラの回転をリセット

	/*------カメラが上に戻る演出------*/
	if (isCameraBackEffect_) {
		if (cameraBackEffectT_ >= cameraBackEffectMaxT_) {
			cameraBackEffectT_ = cameraBackEffectMaxT_; // カメラの移動時間を最大値に設定
			isCameraEffectEnd_ = true;                  // カメラの演出フラグをオンにする
		}
		else {
			cameraBackEffectT_ += 1.0f; // カメラの移動時間をカウントアップ
		}
		// 新しく移動させるカメラの座標
		cameraPosition = moveCameraPosition;
		cameraRotation = moveCameraRotation;
		moveCameraPosition = Vector3::Lerp(cameraPosition, Vector3(0.0f, 50.0f, 0.0f), easeInSine(cameraBackEffectT_ / cameraBackEffectMaxT_)); // カメラの位置を補間
		moveCameraRotation = Vector3::Lerp(cameraRotation, Vector3(1.57f, 0.0f, 0.0f), easeOut(cameraBackEffectT_ / cameraBackEffectMaxT_));     // カメラの回転を補間
		camera_->SetTranslate(moveCameraPosition);                                                                                               // カメラの位置をリセット
		camera_->SetRotate(moveCameraRotation);                                                                                                  // カメラの回転をリセット
	}
}

void Enemy::CameraMove() {
	// カメラの現在の位置を取得
	Vector3 cameraPosition = camera_->GetTranslate();

	// 移動させるカメラの座標
	Vector3 moveCameraPosition = cameraPosition;

	// カメラの移動後の位置を計算
	Vector3 cameraOffset = { 0.0f, 0.0f, 0.0f };

	// カメラの回転を取得
	Vector3 cameraRotation = camera_->GetRotate();

	// 移動させるカメラの回転
	Vector3 moveCameraRotation = cameraRotation;

	float cameraMoveT = 0.0f;

	float cameraMoveMaxT = 160.0f;

	if (cameraMoveT >= cameraMoveMaxT) {
		cameraMoveT = cameraMoveMaxT; // カメラの移動時間を最大値に設定
		isEnemyCameraEffect_ = false;
	}
	else {
		cameraMoveT += 1.0f; // カメラの移動時間をカウントアップ
	}

	moveCameraPosition = Vector3::Lerp(cameraPosition, cameraOffset, easeIn(cameraMoveT_ / cameraMoveMaxT_)); // カメラの位置を補間

	moveCameraRotation = Vector3::Lerp(cameraRotation, Vector3(1.57f, 0.0f, 0.0f), easeInSine(cameraMoveT_ / cameraMoveMaxT_)); // カメラの回転を補間

	camera_->SetTranslate(moveCameraPosition); // カメラの位置をリセット

	camera_->SetRotate(moveCameraRotation); // カメラの回転をリセット
}

void Enemy::FaildAnimation()
{
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

void Enemy::FaildCameraMove()
{
	//  プレイヤーの位置を取得
	Vector3 playerPosition = GetPosition();

	// カメラの現在の位置を取得
	Vector3 cameraPosition = camera_->GetTranslate();

	// 移動させるカメラの座標
	Vector3 moveCameraPosition = cameraPosition;

	// カメラの移動後の位置を計算
	Vector3 cameraOffset = playerPosition + Vector3(0.0f, 0.56f, -10.0f);

	// カメラの回転を取得
	Vector3 cameraRotation = camera_->GetRotate();

	// 移動させるカメラの回転
	Vector3 moveCameraRotation = cameraRotation;

	if (DeathCameraMoveT_ >= DeathCameraMoveMaxT_) {
		DeathCameraMoveT_ = DeathCameraMoveMaxT_; // カメラの移動時間を最大値に設定
		FaildAnimation();                   // 死亡エフェクトを実行
	}
	else {
		DeathCameraMoveT_ += 1.0f; // カメラの移動時間をカウントアップ
		worldTransform_.rotate_.y = 1.5f;
	}

	moveCameraPosition = Vector3::Lerp(cameraPosition, cameraOffset, easeIn(DeathCameraMoveT_ / DeathCameraMoveMaxT_)); // カメラの位置を補間

	moveCameraRotation = Vector3::Lerp(cameraRotation, Vector3(0.0f, 0.0f, 0.0f), easeIn(DeathCameraMoveT_ / DeathCameraMoveMaxT_)); // カメラの回転を補間
	// カメラの位置をプレイヤーの位置に設定
	camera_->SetTranslate(moveCameraPosition);

	camera_->SetRotate(moveCameraRotation); // カメラの回転をリセット
}

/// -------------------------------------------------------------
///					   	ノックバック処理
/// -------------------------------------------------------------
void Enemy::KnockBack() {

	// プレイヤーの位置を確認
	Vector3 playerPosition = player_->GetPosition();

	// ノックバック方向のを計算
	Vector3 knockBackDirection = Vector3::Normalize(worldTransform_.translate_ - playerPosition);

	// 敵の向きにノックバックの速度を加算
	velocity_ = knockBackDirection * knockBackSpeed_; // ノックバックの速度を加算
}

/// -------------------------------------------------------------
///						通常状態の初期化処理
/// -------------------------------------------------------------
void Enemy::BehaviorNormalInitialize() {

	// タイマー初期化
	stateTimer_ = 5.0f;
}

/// -------------------------------------------------------------
///						　通常状態の更新処理
/// -------------------------------------------------------------
void Enemy::BehaviorNormalUpdate() {

	// タイマーが0になったら
	if (stateTimer_ <= 0) {

		// 探索状態にする
		requestBehavior_ = Behavior::Sarch;
	}
	else {

		// タイマーを減らす
		stateTimer_ -= kDeltaTime;
	}
}

/// -------------------------------------------------------------
///						詮索状態の初期化処理
/// -------------------------------------------------------------
void Enemy::BehaviorSarchInitialize() {

	// タイマー初期化
	stateTimer_ = 5.0f;

	// 初回の向きを決める
	direction_ = RondomDirection(-1.0f, 1.0f);
}

/// -------------------------------------------------------------
///					　　詮索状態の更新処理
/// -------------------------------------------------------------
void Enemy::BehaviorSarchUpdate() {

	// タイマーが0になったら
	if (stateTimer_ <= 0) {

		// ランダムに向きを決める
		direction_ = RondomDirection(-1.0f, 1.0f);

		// タイマーをリセット
		stateTimer_ = 2.0f;
	}
	else {

		// タイマーを減らす
		stateTimer_ -= kDeltaTime;
	}

	// 速度を向きに合わせる
	velocity_ = direction_ * 0.1f;

	// プレイヤーとの距離を計算
	const float distance = Vector3::Distance(player_->GetPosition(), worldTransform_.translate_);

	// プレイヤーが発見までの距離に入ったら
	if (distance < foundDistance_) {
		// 攻撃状態にする
		requestBehavior_ = Behavior::Attack;
	}
}

/// -------------------------------------------------------------
///						攻撃状態の初期化処理
/// -------------------------------------------------------------
void Enemy::BehaviorAttackInitialize() {

	// 攻撃方法を選択
	attackCommand_ = RandomAttackCommand();

	// 攻撃を初期化
	attackCommand_->Initialize();

	// 移動をストップ
	velocity_ = { 0.0f, 0.0f, 0.0f };
}

/// -------------------------------------------------------------
///						攻撃状態の更新処理
/// -------------------------------------------------------------
void Enemy::BehaviorAttackUpdate() {

	// プレイヤーの方向を向く
	direction_ = Vector3::Normalize(player_->GetPosition() - worldTransform_.translate_);

	// 攻撃を実行
	attackCommand_->Update(worldTransform_.translate_, direction_, bullets_);

	// 攻撃が終了したら
	if (attackCommand_->GetIsEnd()) {

		// 通常状態にする
		requestBehavior_ = Behavior::Normal;
	}
}
