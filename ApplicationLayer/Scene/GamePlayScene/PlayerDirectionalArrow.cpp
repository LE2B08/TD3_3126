#include "PlayerDirectionalArrow.h"
#include "Object3DCommon.h"
#include "Player.h"
#include <imgui.h>

void PlayerDirectionalArrow::Initialize(){
	// プレイヤーの方向を表す矢印
	directionalArrow_ = std::make_unique<Object3D>();
	directionalArrow_->Initialize("DirectionalArrow.gltf");
	arrowWorldTransform_.Initialize();

	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
}

void PlayerDirectionalArrow::Update(){
	Vector3 playerPos = player_->GetPosition();
	float yAngle = player_->GetRotation().y;

	// 向いている方向ベクトル（Y軸回転のみ考慮）
	Vector3 direction = { cos(yAngle),0.0f,sin(yAngle) };

	// 矢印の距離（プレイヤーから少し前に出す）
	float arrowOffset = 2.0f;
	Vector3 arrowPos = playerPos - direction * arrowOffset;

	arrowWorldTransform_.translate_ = arrowPos;
	arrowWorldTransform_.rotate_ = { 0.0f, yAngle, 0.0f }; // プレイヤーと同じY軸向き
	arrowWorldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };    // 必要に応じてスケール調整
	arrowWorldTransform_.Update();
	// ワールド変換の更新
	directionalArrow_->SetCamera(camera_);
	directionalArrow_->SetTranslate(arrowWorldTransform_.translate_);
	directionalArrow_->SetRotate(arrowWorldTransform_.rotate_);
	directionalArrow_->SetScale(arrowWorldTransform_.scale_);
	directionalArrow_->Update();
}

void PlayerDirectionalArrow::Draw(){
	directionalArrow_->Draw();
}

void PlayerDirectionalArrow::DrawImGui(){

}
