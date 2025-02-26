#include "Player.h"
#include "Input.h"
#include "imgui.h"
#include "Wireframe.h"

void Player::Initialize() {
	// オブジェクト3D
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("sphere.gltf");
	// 位置
	position_ = {0.0f, 0.0f, 0.0f};
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
}

void Player::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		HookThrow();
	}
	// フックがアクティブな場合、フックの移動処理を行う
	if (isHookActive_) {
		// 原点への移動処理
		MoveToOrigin();
	}

	// 移動処理
	Move();

	

	// Transform更新処理
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();
}

void Player::Draw() {
	// 描画処理
	object3D_->Draw();
	Wireframe::GetInstance()->DrawLine(hookStartPos_, hookEndPos_, {1.0f,1.0f,1.0f,1.0f});

	// ワイヤーフレームの描画
	Wireframe::GetInstance()->DrawGrid(100.0f, 20.0f, {0.25f, 0.25f, 0.25f, 1.0f});
}

void Player::Finalize() {}

void Player::DrawImGui() {

	ImGui::Begin("Player");
	ImGui::Text("Position");
	ImGui::SliderFloat3("", &position_.x, -10.0f, 10.0f);
	ImGui::Text("Rotation");
	ImGui::SliderFloat3("", &rotation_.x, -10.0f, 10.0f);
	ImGui::Text("Scale");
	ImGui::SliderFloat3("", &scale_.x, 0.0f, 10.0f);
	ImGui::Text("Velocity");
	ImGui::SliderFloat3("", &velocity_.x, -10.0f, 10.0f);
	ImGui::Text("Acceleration");
	ImGui::SliderFloat3("", &acceleration_.x, -10.0f, 10.0f);
	ImGui::Text("AngularVelocity");
	ImGui::SliderFloat3("", &angularVelocity_.x, -10.0f, 10.0f);
	ImGui::End();
}

void Player::Move() {
	///============================
	/// キーボード入力による移動処理
	///

	// 入力処理
	if (Input::GetInstance()->PushKey(DIK_W)) {
		acceleration_.z = 0.005f;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		acceleration_.z = -0.005f;
	} else {
		acceleration_.z = 0.0f;
	}

	if (Input::GetInstance()->PushKey(DIK_A)) {
		acceleration_.x = -0.005f;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {
		acceleration_.x = 0.005f;
	} else {
		acceleration_.x = 0.0f;
	}

	///============================
	/// コントローラー入力による移動処理
	///

	// 入力処理

	/*acceleration_.x = Input::GetInstance()->GetLeftStick().x * 0.01f;
	acceleration_.z = Input::GetInstance()->GetLeftStick().y * 0.01f;*/

	///============================
	/// 移動処理
	///
	acceleration_.y = -0.0f;
	velocity_ += acceleration_;
	position_ += velocity_;

	// 減速処理
	const float friction = 0.98f; // 摩擦係数
	velocity_ *= friction;
}
void Player::HookThrow() {
	// フックの位置をプレイヤーの位置に設定
	hookStartPos_ = position_;
	hookEndPos_ = {};                   // 初期位置はプレイヤーの位置
	hookVelocity_ = {0.0f, 0.0f, 1.0f}; // フックの初速度
	isHookActive_ = true;               // フックが発射されたことを示すフラグ
}

void Player::HookUpdate() {
	// フックの移動
	hookEndPos_ += hookVelocity_;

	// フックが対象に到達したかどうかの判定（例として距離で判定）
	if (Vector3::Distance(hookEndPos_, targetPosition_) < hookReachThreshold_) {
		isHookActive_ = false;
		isHookAttached_ = true;
	}

	// フックが対象に到達した場合、プレイヤーを引っ張る
	if (isHookAttached_) {
		HookPull();
	}
}

void Player::HookPull() {
	// フックの位置に向かってプレイヤーを移動させる
	Vector3 direction = Vector3::Normalize(hookEndPos_ - position_);
	velocity_ = direction * hookPullSpeed_;
	position_ += velocity_;

	// フックの位置に到達したらフックを解除
	if (Vector3::Distance(position_, hookEndPos_) < hookDetachThreshold_) {
		isHookAttached_ = false;
		velocity_ = {0.0f, 0.0f, 0.0f};
	}
}
void Player::MoveToOrigin() {
	Vector3 targetPosition = {0.0f, 0.0f, 0.0f};
	Vector3 direction = Vector3::Normalize(targetPosition - position_);
	float speed = 0.1f; // 移動速度を設定
	velocity_ = direction * speed;
	position_ += velocity_;

	// 目標位置に到達したら速度をゼロにする
	if (Vector3::Distance(position_, targetPosition) < 0.01f) {
		position_ = targetPosition;
		velocity_ = {0.0f, 0.0f, 0.0f};
		speed = 0.0f;
	}
}
