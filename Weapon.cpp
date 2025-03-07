#include "Weapon.h"
#include <cmath>
#include <imgui.h>

#include <chrono>
#include <thread>

void Weapon::Initialize() {
	// 位置
	position_ = playerPosition_;
	// 回転
	rotation_ = playerRotation_;
	// スケール
	scale_ = playerScale_;

	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("sphere.gltf");
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);

	attackTime_ = 0;
	attackRotationAngle_ = 0.0f;
}

void Weapon::Update() {
	// 攻撃処理
	if (isAttack_) {
		Attack();
	}
	// 位置
	position_ = playerPosition_;
	// 回転
	rotation_ = playerRotation_;
	// スケール
	scale_ = playerScale_;

	// プレイヤーの向いている方向に武器を配置

	// プレイヤーの向いている方向に武器を配置
	Vector3 offset = { distance_ * std::cos(attackRotationAngle_), 0.0f, distance_ * std::sin(attackRotationAngle_) };
	Vector3 weaponPosition = position_ - offset;

	// Transform更新処理
	object3D_->SetTranslate(weaponPosition);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();
}

void Weapon::Draw() {
	// 描画処理
	if (isAttack_) {
		object3D_->Draw();
	}
}

void Weapon::Finalize() {}

void Weapon::DrawImGui() {

	ImGui::Begin("Weapon");
	ImGui::Text("Weapon");
	ImGui::SliderFloat3("Position", &position_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &rotation_.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Scale", &scale_.x, 0.0f, 10.0f);
	ImGui::SliderFloat("Rotation Speed", &rotationSpeed_,0.0f,5.0f);
	ImGui::End();
}

void Weapon::Attack() {
	/*------攻撃時間をカウント------*/
	attackTime_++;
	/*------攻撃時間が最大値を超えたら攻撃を終了------*/
	if (attackTime_ > attackMaxTime_) {
		isAttack_ = false;
		attackTime_ = 0;
		attackRotationAngle_ = 0.0f;
	} else {
		/*------回転角度を更新------*/
		attackRotationAngle_ += (2.0f * 3.14159265358979323846f * rotationSpeed_) / attackMaxTime_;
	}
}


void Weapon::OnCollision(Collider* other) {}

Vector3 Weapon::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPosition = Vector3::Transform(offset, worldTransform_.matWorld_);

	return worldPosition;
}
