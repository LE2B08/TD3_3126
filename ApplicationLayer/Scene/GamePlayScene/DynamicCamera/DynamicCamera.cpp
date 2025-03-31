#include "DynamicCamera.h"
#include "GamePlayScene/Player.h"
#include "GamePlayScene/Enemy/Enemy.h"

#include <imgui.h>

DynamicCamera::DynamicCamera() {
}

void DynamicCamera::Initialize() {

	// ワールド変換初期化
	worldTransform_.Initialize();

	// 向きの設定
	worldTransform_.rotate_ = { 1.57f, 0.0f, 0.0f };
}

void DynamicCamera::Update() {

	// プレイヤーの位置を取得
	Vector3 playerPos = player_->GetPosition();

	// 敵の位置を取得
	Vector3 enemyPos = enemy_->GetPosition();

	// カメラの位置をプレイヤーと敵の中間に設定
	centerPos_ = (playerPos + enemyPos) / 2.0f;

	// xZ平面上の距離を計算
	distance_ = Vector3::Distance(playerPos, enemyPos);

	// ズームの計算
	zoom_ = distance_ * 2.5f;

	// 距離の制限
	zoom_ = std::clamp(zoom_, zoomMin_, zoomMax_);

	// カメラの位置を設定
	worldTransform_.translate_ = { centerPos_.x, zoom_, centerPos_.z };

	// ワールド変換更新
	worldTransform_.Update();
}

void DynamicCamera::ShowImGui(const char* name) {

	ImGui::Begin(name);
	ImGui::DragFloat3("Rotate", &worldTransform_.rotate_.x, 0.01f);
	ImGui::DragFloat3("Translate", &centerPos_.x, 0.01f);
	ImGui::DragFloat("Distance", &distance_, 0.01f);
	ImGui::DragFloat("Zoom", &zoom_, 0.01f, zoomMin_, zoomMax_);
	ImGui::End();
}
