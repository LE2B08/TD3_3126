#include "Player.h"
#include "CollisionTypeIdDef.h"
#include "Input.h"
#include "ParticleManager.h"
#include "Wireframe.h"
#include "imgui.h"

#undef max
#undef min

void Player::Initialize() {
	// オブジェクト3D
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("sphere.gltf");
	// 位置
	position_ = {8.0f, 0.0f, 8.0f};
	// 回転
	rotation_ = {0.0f, 0.0f, 0.0f};
	// スケール
	scale_ = {1.0f, 1.0f, 1.0f};
	// 速度
	velocity_ = {0.0f, 0.0f, 0.0f};
	// 加速度
	acceleration_ = {0.0f, 0.0f, 0.0f};
	// 角速度
	angularVelocity_ = {0.0f, 0.0f, 0.0f};

	// Hpの初期化
	hp_ = 100;

	// フックの生成 初期化
	hook_ = std::make_unique<Hook>();
	hook_->SetPlayerPosition(position_);
	hook_->SetPlayerRotation(rotation_);
	hook_->SetPlayerVelocity(velocity_);
	hook_->SetPlayerAcceleration(acceleration_);

	hook_->SetMinMoveLimit(minMoveLimit_);
	hook_->SetMaxMoveLimit(maxMoveLimit_);
	hook_->Initialize();
	// 武器の初期化
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// フックのコライダーの設定
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));

	// パーティクルマネージャの生成
	particleManager_ = ParticleManager::GetInstance();

	// テクスチャの読み込み
	TextureManager::GetInstance()->LoadTexture("Resources/uvChecker.png");

	// パーティクルグループの追加
	particleManager_->CreateParticleGroup("PlayerHitParticles", "Resources/uvChecker.png");

	// パーティクルエミッターの初期化
	particleEmitter_ = std::make_unique<ParticleEmitter>(ParticleManager::GetInstance(), "PlayerHitParticles");
}

void Player::Update() {

	/*--------DebugMode----------*/
#ifdef _DEBUG

	// コントローラーのStartボタンを押すと
	// デバックモードになる
	if (!isDebug_) {
		if (Input::GetInstance()->TriggerButton(12)) {
			isDebug_ = true;
		}
	} else {
		// デバッグモードの場合
		// もう一度押すとデバックモードを解除
		if (Input::GetInstance()->TriggerButton(12)) {
			isDebug_ = false;
		}
	}

#endif // DEBUG

	// ゲームスタート時のみ更新処理を行う
	if (isGameStart_) {

		// フックから移動情報を取得
		position_ = hook_->GetPlayerPosition();
		velocity_ = hook_->GetPlayerVelocity();
		acceleration_ = hook_->GetPlayerAcceleration();

		/*------ヒット時の処理------*/
		if (isHit_) {
			HitParticle();
			hitTime_++;
			if (hitTime_ >= hitMaxTime_) {
				isHit_ = false;
				hitTime_ = 0;
			}
		}

		// 移動処理
		Move();

		// 回転処理
		Rotate();

		// 攻撃処理
		Attack();

		// ヒット時のHP減少処理
		DecreaseHpOnHit();

		collisionManager_->Update();
	}
	// 移動制限
	// Fieldの範囲内に収める
	position_.x = std::clamp(position_.x, minMoveLimit_.x, maxMoveLimit_.x);
	position_.z = std::clamp(position_.z, minMoveLimit_.z, maxMoveLimit_.z);

	// フックの更新処理
	hook_->SetPlayerRotation(rotation_);
	hook_->SetPlayerPosition(position_);
	hook_->SetPlayerVelocity(velocity_);
	hook_->SetPlayerAcceleration(acceleration_);
	hook_->SetMinMoveLimit(minMoveLimit_);
	hook_->SetMaxMoveLimit(maxMoveLimit_);
	hook_->SetIsDebug(isDebug_);
	hook_->SetIsHitPlayerToEnemy(isHitEnemy_);

	// フックの更新処理
	hook_->Update();

	// 武器の更新処理
	weapon_->SetPlayerPosition(position_);
	weapon_->SetPlayerRotation(rotation_);
	weapon_->SetPlayerScale(scale_);
	weapon_->Update();

	// Transform更新処理
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();
}

void Player::Draw() {
	if (isGameStart_) {
		// 描画処理
		object3D_->Draw();
	}
	// フックの描画
	hook_->Draw();

	// 武器の描画
	weapon_->Draw();

	collisionManager_->Draw();

	// プレイヤーの向きを示す線を描画
	Vector3 direction = {cos(rotation_.y), 0.0f, sin(rotation_.y)};
	Vector3 endPos = position_ + -direction * 5.0f;                                  // 5.0fは線の長さ
	Wireframe::GetInstance()->DrawLine(position_, endPos, {0.0f, 1.0f, 0.0f, 1.0f}); // 緑色の線
}

void Player::Finalize() {}

void Player::DrawImGui() {

	ImGui::Begin("Player");
	// デバッグモードの表示
	ImGui::Text("DebugMode : %s", isDebug_ ? "true" : "false");
	ImGui::Checkbox("DebugMode", &isDebug_);
	// 武器が敵に当たったかどうか
	ImGui::Text("isHit : %s", isHit_ ? "true" : "false");
	// 無敵状態かどうか
	ImGui::Text("isInvincible : %s", isInvincible_ ? "true" : "false");
	// HPの表示
	ImGui::Text("HP : %d", hp_);
	// 敵に当たったかどうか
	ImGui::Text("isHitEnemy : %s", isHitEnemy_ ? "true" : "false");
	// プレイヤーの位置
	ImGui::Text("Position");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	// プレイヤーの回転
	ImGui::Text("Rotation");
	ImGui::SliderFloat3("Rotate", &rotation_.x, -10.0f, 10.0f);
	// プレイヤーのスケール
	ImGui::Text("Scale");
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	// プレイヤーの速度
	ImGui::Text("Velocity");
	ImGui::SliderFloat3("Velocity", &velocity_.x, -10.0f, 10.0f);
	// プレイヤーの加速度
	ImGui::Text("Acceleration");
	ImGui::SliderFloat3("Accel", &acceleration_.x, -10.0f, 10.0f);
	// プレイヤーの角速度
	ImGui::Text("AngularVelocity");
	ImGui::SliderFloat3("AngleVelo", &angularVelocity_.x, -10.0f, 10.0f);

	ImGui::End();

	ImGui::Begin("Player_Controller");
	// コントローラーの操作方法
	ImGui::Text("Move");
	ImGui::Text("Rotate: RightStick");
	ImGui::Text("Attack: LeftShoulder");
	ImGui::Text("Hook");
	ImGui::Text("Throw: RightShoulder");
	ImGui::Text("Back: RightShoulder");
	ImGui::Text("Pulling: RightTrigger");
	ImGui::Text("ArcMove: RightStick");
	// debug
	ImGui::Text("DebugMode: StartButton");
	ImGui::End();

	hook_->ShowImGui();
}
void Player::Move() {

	///============================
	/// プレイヤーの移動処理
	///

	if (isDebug_) {
		// デバッグモードの場合
		// プレイヤーの移動が左スティックで、できるようにする
		Vector2 leftStick = Input::GetInstance()->GetLeftStick();
		position_.x += leftStick.x * 0.1f;
		position_.z += leftStick.y * 0.1f;
	}

	// 速度制限
	const float maxSpeed = 10.0f; // 最大速度（調整可能）
	// 速度の大きさを計算
	float speed = Vector3::Length(velocity_);
	// 速度が最大速度を超えた場合、速度を制限
	if (speed > maxSpeed) {
		velocity_ = (velocity_ / speed) * maxSpeed;
	}

	// 減速処理
	const float friction = 0.98f; // 摩擦係数
	velocity_ *= friction;

	// プレイヤーの位置を更新
	velocity_ += acceleration_;
	position_ += velocity_;
}

void Player::Rotate() {
	///================
	/// プレイヤーの回転処理
	///

	// プレイヤーの向きを左スティックの向きにする

	// 右スティックの入力があるとき
	if (!Input::GetInstance()->RStickInDeadZone()) {
		// プレイヤーの向きを変える
		rotation_.y = -atan2(Input::GetInstance()->GetRightStick().x, Input::GetInstance()->GetRightStick().y) - std::numbers::pi_v<float> / 2.0f;
	}
}

void Player::Attack() {
	// 攻撃処理
	// 攻撃ボタンを押した瞬間
	if (Input::GetInstance()->TriggerButton(8)) {
		// 武器の攻撃フラグを立てる
		weapon_->SetIsAttack(true);
	}
}
void Player::DecreaseHpOnHit() {
	// プレイヤーが敵に当たった場合の
	// 無敵時間の処理
	if (isInvincible_) {
		// 無敵時間をカウント
		invincibleTime_++;
		// 無敵時間が最大値を超えた場合
		if (invincibleTime_ >= maxInvincibleTime_) {
			// 無敵状態を解除
			isInvincible_ = false;
			invincibleTime_ = 0;
		}
	}
}


void Player::OnCollision(Collider* other) {
	// 種別IDを取得
	uint32_t typeID = other->GetTypeID();
	// フックがアクティブで、敵と衝突した場合
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {
		// Playerが敵に当たった時の処理
		isHitEnemy_ = true;

		// 無敵状態でない場合にHPを減らす
		if (!isInvincible_) {
			hp_ -= 1;
			isInvincible_ = true;
			invincibleTime_ = 0;
		}
	} else {
		isHitEnemy_ = false;
	}

	if (weapon_->GetIsAttack()) {
		isHit_ = true;
	}
}

Vector3 Player::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	Vector3 worldPosition = position_ + offset;
	return worldPosition;
}

void Player::CheckAllCollisions() {
	// 衝突マネージャのリセット
	collisionManager_->Reset();

	// コライダーをリストに登録
	collisionManager_->AddCollider(weapon_.get());
	collisionManager_->AddCollider(enemy_);

	// 複数について

	// 攻撃中の場合
	if (weapon_->GetIsAttack()) {
		// 衝突判定と応答
		collisionManager_->CheckAllCollisions();
		if (enemy_->GetIsHit()) {
			enemy_->SetIsHitFromAttack(true);
		}
	}
}

void Player::HitParticle() {
	// エネミーの中心位置を取得
	Vector3 playerCenter = GetCenterPosition();

	// パーティクルエミッターの位置をエネミーの中心に設定
	particleEmitter_->SetPosition(playerCenter);
	particleEmitter_->SetEmissionRate(100); // パーティクルの発生率を設定
	// パーティクルを生成
	particleEmitter_->Update(1.0f / 60.0f); // deltaTime は 0 で呼び出し
}
