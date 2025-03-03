#include "Player.h"
#include "Input.h"
#include "Wireframe.h"
#include "imgui.h"

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
	// if (Input::GetInstance()->TriggerButton(DIK_SPACE)) {.
	//	HookThrow();
	// }
	//// フックがアクティブな場合、フックの移動処理を行う
	// if (isHookActive_) {
	//	// 原点への移動処理
	//	MoveToOrigin();
	// }

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
	Wireframe::GetInstance()->DrawLine(hookStartPos_, hookEndPos_, {1.0f, 1.0f, 1.0f, 1.0f});

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
	/// 移動処理
	/// はデバック用に残しておく
	/// 最終的には消す
	///
#ifdef _DEBUG

	// コントローラーが接続されてる時
	if (Input::GetInstance()->GetGamePadState().Gamepad.sThumbLX || Input::GetInstance()->GetGamePadState().Gamepad.sThumbLY) {
		// 左スティックの入力があるとき
		// プレイヤーを移動させる
		acceleration_.x = Input::GetInstance()->GetLeftStick().x * 0.01f;
		acceleration_.z = Input::GetInstance()->GetLeftStick().y * 0.01f;

	} else {
		// コントローラーが接続されてないとき

		// WとSキーでプレイヤーを上下に移動
		if (Input::GetInstance()->PushKey(DIK_W)) {
			acceleration_.z = 0.005f;
		} else if (Input::GetInstance()->PushKey(DIK_S)) {
			acceleration_.z = -0.005f;
		} else {

			acceleration_.z = 0.0f;
		}
		// AとDキーでプレイヤーを左右に移動
		if (Input::GetInstance()->PushKey(DIK_A)) {
			acceleration_.x = -0.005f;
		} else if (Input::GetInstance()->PushKey(DIK_D)) {
			acceleration_.x = 0.005f;
		} else {
			acceleration_.x = 0.0f;
		}
	}
	// Y軸は固定
	acceleration_.y = -0.0f;

#endif // _DEBUG
	   ///================
	   /// プレイヤーの回転処理
	   ///

	//========================================
	// プレイヤーの向きを左スティックの向きにする
	if (Input::GetInstance()->GetRightStick().x != 0.0f || Input::GetInstance()->GetRightStick().y != 0.0f) {
		rotation_.y = -atan2(Input::GetInstance()->GetRightStick().x, Input::GetInstance()->GetRightStick().y) - 3.14159265358979323846f / 2.0f;
	}
	// 右スティックの向きを優先

	// プレイヤーの移動処理
	velocity_ += acceleration_;
	position_ += velocity_;

	// 減速処理
	const float friction = 0.98f; // 摩擦係数
	velocity_ *= friction;
}
