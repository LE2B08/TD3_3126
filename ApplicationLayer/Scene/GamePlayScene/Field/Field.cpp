#include "Field.h"
#include <imgui.h>
#include "Wireframe.h"
#include "Easing.h"

using namespace Easing;

/// -------------------------------------------------------------
///						　初期化処理
/// -------------------------------------------------------------
void Field::Initialize()
{

	// 仮リソース
	objectField_ = std::make_unique<Object3D>();
	objectField_->Initialize("Field.gltf");
}


/// -------------------------------------------------------------
///						　更新処理
///	-------------------------------------------------------------
void Field::Update()
{

	minPosition_ = position_ - scale_;
	maxPosition_ = position_ + scale_;

	objectField_->SetScale(scale_);
	objectField_->SetRotate(rotate_);
	objectField_->SetTranslate(position_);
	objectField_->Update();
}


/// -------------------------------------------------------------
///						　描画処理
/// -------------------------------------------------------------
void Field::Draw()
{

	// ワイヤーフレームの描画
	// Fieldの描画
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, maxPosition_.z}, {minPosition_.x, 0.0f, maxPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, minPosition_.z}, {minPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({maxPosition_.x, 0.0f, maxPosition_.z}, {maxPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});
	Wireframe::GetInstance()->DrawLine({minPosition_.x, 0.0f, maxPosition_.z}, {minPosition_.x, 0.0f, minPosition_.z}, {1.0f, 1.0f, 1.0f, 1.0f});

	objectField_->Draw();
}


/// -------------------------------------------------------------
///						　ImGui描画処理
/// -------------------------------------------------------------
void Field::ShowImGui(const char* name) {

	ImGui::Begin(name);

	ImGui::DragFloat3("Scale", &scale_.x, 0.1f);

	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);

	ImGui::DragFloat3("Position", &position_.x, 0.01f);

	ImGui::End();
}

void Field::ScalingAnimation() {
	
	// タイマー処理
	if (scalingTimer_ >= maxScalingTime_) {

		// タイマーが最大値を超えないようにする
		scalingTimer_ = maxScalingTime_;
	}
	else {
		// タイマーを進める
		scalingTimer_ += 0.5f;
	}

	scale_ = Vector3::Lerp(startScale_, defaultScale_, easeOutBounce(scalingTimer_ / maxScalingTime_));

	// タイマーと最大値が等しい場合
	if (scalingTimer_ == maxScalingTime_) {

		// 拡縮が終わったフラグを立てる
		isScaleEnd_ = true;
	}
}
