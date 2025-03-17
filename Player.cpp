#include "Player.h"
#include "Input.h"
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

	// フックの生成 初期化
	hook_ = std::make_unique<Hook>();
	hook_->SetPlayerPosition(position_);
	hook_->SetMinMoveLimit(minMoveLimit_);
	hook_->SetMaxMoveLimit(maxMoveLimit_);
	hook_->Initialize();
	// 武器の初期化
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize();

	// 衝突マネージャの生成
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();
}

void Player::Update() {

#ifdef _DEBUG

	// コントローラーのStartボタンとBackボタンを同時に押すとデバックモードになる
	if (!isDebug_) {
		if (Input::GetInstance()->TriggerButton(12) && Input::GetInstance()->TriggerButton(13)) {
			isDebug_ = true;
		}
	} else {
		if (Input::GetInstance()->TriggerButton(12) && Input::GetInstance()->TriggerButton(13)) {
			isDebug_ = false;
		}
	}
	// デバックモード
	// プレイヤーをの移動が
	// 左スティックとキーボードで入力できる。

#endif // DEBUG

	hookToEnemyHit_ = hook_->GetEnemyHit();

	// if (Input::GetInstance()->TriggerButton(8)) {
	//	// フックの巻取りを有効
	//	isWindingTheHook = true;
	// }

	// if (isWindingTheHook) {
	//	// フックの巻取りを行う
	//	// 移動処理
	//	Move();
	// }

	// if (!hook_->GetIsActive()) {
	//	isWindingTheHook = false;
	// }

	// 回転処理
	Rotate();

	// 攻撃処理
	Attack();

	// 移動制限
	position_.x = std::clamp(position_.x, minMoveLimit_.x, maxMoveLimit_.x);
	position_.z = std::clamp(position_.z, minMoveLimit_.z, maxMoveLimit_.z);

	// フックの更新処理

	hook_->SetPlayerRotation(rotation_);
	hook_->SetPlayerPosition(position_);
	hook_->SetMinMoveLimit(minMoveLimit_);
	hook_->SetMaxMoveLimit(maxMoveLimit_);
	hook_->Update();
	// フックの位置を取得
	position_ = hook_->GetPlayerPosition();

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

	collisionManager_->Update();
}

void Player::Draw() {

	// 描画処理
	object3D_->Draw();

	// フックの描画
	hook_->Draw();

	// 武器の描画
	weapon_->Draw();

	// 衝突判定の描画
	collisionManager_->Draw();

	// プレイヤーの向きを示す線を描画
	Vector3 direction = {cos(rotation_.y), 0.0f, sin(rotation_.y)};
	Vector3 endPos = position_ + -direction * 5.0f;                                  // 5.0fは線の長さ
	Wireframe::GetInstance()->DrawLine(position_, endPos, {0.0f, 1.0f, 0.0f, 1.0f}); // 緑色の線
}

void Player::Finalize() {}

void Player::DrawImGui() {

	ImGui::Begin("Player");
	ImGui::Text("Position");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::Text("Rotation");
	ImGui::SliderFloat3("Rotate", &rotation_.x, -10.0f, 10.0f);
	ImGui::Text("Scale");
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::Text("Velocity");
	ImGui::SliderFloat3("Velocity", &velocity_.x, -10.0f, 10.0f);
	ImGui::Text("Acceleration");
	ImGui::SliderFloat3("Accel", &acceleration_.x, -10.0f, 10.0f);
	ImGui::Text("AngularVelocity");
	ImGui::SliderFloat3("AngleVelo", &angularVelocity_.x, -10.0f, 10.0f);
	ImGui::Checkbox("IsDebug", &isDebug_);
	ImGui::End();

	hook_->ShowImGui();
}
void Player::Move() {
	///================
	/// プレイヤーの移動処理
	///

	// 最大速度を定義
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

	//
	// プレイヤーの移動処理
	velocity_ += acceleration_;
	position_ += velocity_;
}

void Player::Rotate() {

	///================
	/// プレイヤーの回転処理
	///

	// プレイヤーの向きを左スティックの向きにする

	// 右スティックの入力があるとき
	if (Input::GetInstance()->GetRightStick().x != 0.0f || Input::GetInstance()->GetRightStick().y != 0.0f) {
		// プレイヤーの向きを変える
		rotation_.y = -atan2(Input::GetInstance()->GetRightStick().x, Input::GetInstance()->GetRightStick().y) - std::numbers::pi_v<float> / 2.0f;
	} else {
	}
}

void Player::Attack() {
	///================
	/// 攻撃処理
	///

	// 攻撃ボタンを押した瞬間
	if (Input::GetInstance()->TriggerButton(8)) {
		// 武器の攻撃フラグを立てる
		weapon_->SetIsAttack(true);
	}
}

void Player::OnCollision(Collider* other) {}

Vector3 Player::GetCenterPosition() const {
	const Vector3 offset = {0.0f, 0.0f, 0.0f}; // プレイヤーの中心を考慮
	Vector3 worldPosition = position_ + offset;
	return worldPosition;
}

void Player::CheckAllCollisions(Enemy* enemy) {}
