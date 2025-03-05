#include "Enemy.h"
#include <imgui.h>

Enemy::Enemy() {
}

Enemy::~Enemy() {
}

void Enemy::Initialize() {

	objectEnemy_ = std::make_unique<Object3D>();
	objectEnemy_->Initialize("sphere.gltf");

	worldTransform_.Initialize();
	worldTransform_.translate_ = position_;
	worldTransform_.rotate_ = rotate_;
	worldTransform_.scale_ = scale_;
}

void Enemy::Update() {

	Move();

	// WorldTransformの更新
	worldTransform_.translate_ = position_;
	worldTransform_.rotate_ = rotate_;
	worldTransform_.scale_ = scale_;
	worldTransform_.Update();


	objectEnemy_->SetTranslate(position_);

	objectEnemy_->Update();
}

void Enemy::Draw() {

	objectEnemy_->Draw();
}

void Enemy::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Acceleration", &acceleration_.x, 0.01f, -1.0f, 1.0f);

	ImGui::DragFloat3("Velocity", &velocity_.x, 0.01f);

	ImGui::DragFloat3("Position", &position_.x, 0.01f);

	ImGui::End();
}

void Enemy::Move() {

	// 速度に加速度を加算
	velocity_ += acceleration_;

	// 位置に速度を加算
	position_ += velocity_;

	// 加速度が正負両方に最大値を超えないようにクランプ
	acceleration_.x = std::clamp(acceleration_.x, -maxAcceleration_, maxAcceleration_);
	acceleration_.y = std::clamp(acceleration_.y, -maxAcceleration_, maxAcceleration_);
	acceleration_.z = std::clamp(acceleration_.z, -maxAcceleration_, maxAcceleration_);

	// 速度が正負両方に最大値を超えないようにクランプ
	velocity_.x = std::clamp(velocity_.x, -maxVelocity_, maxVelocity_);
	velocity_.y = std::clamp(velocity_.y, -maxVelocity_, maxVelocity_);
	velocity_.z = std::clamp(velocity_.z, -maxVelocity_, maxVelocity_);
}


void Enemy::OnCollision(Collider* other) {}

Vector3 Enemy::GetCenterPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = {0.0f, 1.5f, 0.0f};
	// ワールド座標に変換
	Vector3 worldPosition = Vector3::Transform(offset, worldTransform_.matWorld_);

	return worldPosition;
}
