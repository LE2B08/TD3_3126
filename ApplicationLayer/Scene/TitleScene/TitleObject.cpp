#define NOMINMAX
#include "TitleObject.h"

#include <imgui.h>

void TitleObject::Initialize()
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("GameTitle.gltf");

	pressToA_ = std::make_unique<Object3D>();
	pressToA_->Initialize("PressToA.gltf");

	pressBToGuide_ = std::make_unique<Object3D>();
	pressBToGuide_->Initialize("PressBToGuide.gltf");

	position_ = Vector3(-20.0f, 0.0f, 20.0f); // スタート位置に近い値
	rotation_ = Vector3(0.0f, 3.14f, 0.0f);    // 初期角度
	scale_ = Vector3(3.0f, 3.0f, 3.0f);       // 初期スケール

	moveTime_ = 0.0f;
	moveDuration_ = 1.5f; // 再生時間（演出に合わせて調整）

	baseY_ = position_.y; // 基準Y座標を保存
}

void TitleObject::Update()
{
	if (isExiting_)
	{
		exitTime_ += 1.0f / 60.0f;
		float t = std::min(exitTime_ / exitDuration_, 1.0f);

		// 勢いをつけたEaseInBackで上にビューン
		float easedT = EaseInBack(t, 2.3f);

		float moveOffset = 20.0f * easedT;   // 画面上方向に勢いよく移動
		float alpha = 1.0f - t;              // 徐々にフェードアウト

		// タイトルロゴ
		Vector3 pos = position_;
		pos.y = baseY_ + moveOffset;
		object3D_->SetTranslate(pos);
		objectColor_.w = alpha;
		object3D_->SetColor(objectColor_);

		// PressToA
		Vector3 pressPos = Vector3(0.0f, 0.1f + moveOffset, -6.0f);
		pressToA_->SetTranslate(pressPos);
		pressToAColor_.w = alpha;
		pressToA_->SetColor(pressToAColor_);

		// PressBToGuide
		Vector3 pressBPos = Vector3(0.0f, 0.1f + moveOffset, -10.0f);
		pressBToGuide_->SetTranslate(pressBPos);
		pressBToGuide_->SetColor(objectColor_);
		pressBToGuide_->SetAlpha(alpha);

		object3D_->Update();
		pressToA_->Update();
		pressBToGuide_->Update();
		return;
	}

	if (moveTime_ < moveDuration_)
	{
		moveTime_ += 1.0f / 60.0f;

		float t = std::min(moveTime_ / moveDuration_, 1.0f);
		float easedT = EaseOutBack(t); // 勢いつけて"ビューン"

		// ===== 円弧移動（XZ） =====
		Vector3 center = Vector3(0.0f, 0.0f, -16.0f);
		float radius = 20.0f;
		float startAngle = 180.0f * (std::numbers::pi_v<float> / 180.0f);
		float endAngle = 90.0f * (std::numbers::pi_v<float> / 180.0f);

		float angle = startAngle + (endAngle - startAngle) * easedT;

		position_.x = center.x + cosf(angle) * radius;
		position_.z = center.z + sinf(angle) * radius;

		// ===== Y軸は弧を描くように浮いて落ちる =====
		float yStart = 10.0f;
		float yEnd = 0.0f;
		float parabola = 1.0f - powf(2.0f * easedT - 1.0f, 2.0f); // 放物線
		position_.y = yEnd + (yStart - yEnd) * parabola;

		// ===== スケール演出（小 → 大） =====
		float scaleStart = 2.5f;
		float scaleEnd = 4.0f;
		float currentScale = scaleStart + (scaleEnd - scaleStart) * easedT;
		scale_ = Vector3(currentScale, currentScale, currentScale);

		// ===== 回転（少し揺れる感じでもOK） =====
		Vector3 startRot = Vector3(0.0f, 1.0f, 0.0f);
		Vector3 endRot = Vector3(0.0f, std::numbers::pi_v<float>, 0.0f);
		rotation_ = startRot + (endRot - startRot) * easedT;
	}
	else
	{
		// ===== 常時ループアニメーション =====
		// 揺れる回転（Y軸）
		float time = static_cast<float>(moveTime_); // 秒数で代用
		rotation_.y = std::numbers::pi_v<float> +sinf(time * 2.0f) * 0.05f;

		// ゆっくり上下する（浮いてる感じ）
		position_.y = 0.3f + sinf(time * 1.5f) * 0.2f;

		// moveTime_ 進めておく（揺れが続く）
		moveTime_ += 1.0f / 60.0f;
	}

	// 位置、回転、スケールを設定
	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();


	// 上記を組み合わせてUpdate内で適用
	float alpha = 0.5f + 0.5f * sinf(moveTime_ * 2.0f);
	float floatY = sinf(moveTime_ * 2.0f) * 0.1f;
	float scaleFactor = 1.0f + 0.05f * sinf(moveTime_ * 4.0f);

	pressToA_->SetTranslate(Vector3(0.0f, 0.1f + floatY, -6.0f));
	pressToA_->SetScale(Vector3(2.0f, 2.0f, 2.0f) * scaleFactor);
	pressToA_->SetRotate(Vector3(0.0f, std::numbers::pi_v<float>, 0.0f));
	pressToA_->Update();

	pressBToGuide_->SetScale(Vector3(2.0f, 2.0f, 2.0f) * scaleFactor);
	pressBToGuide_->SetRotate(Vector3(0.0f, std::numbers::pi_v<float>, 0.0f));
	pressBToGuide_->SetTranslate(Vector3(0.0f, 0.1f + floatY, -10.0f));
	pressBToGuide_->Update();
}

void TitleObject::Draw()
{
	object3D_->Draw();

	pressToA_->Draw();

	pressBToGuide_->Draw();
}

void TitleObject::Finalize()
{

}

void TitleObject::DrawImGui()
{

}

void TitleObject::StartExitAnimation()
{
	isExiting_ = true;
	exitTime_ = 0.0f;
}

void TitleObject::SetAlpha(float alpha)
{
	objectColor_.w = alpha;
	pressToAColor_.w = alpha;

	if (object3D_) object3D_->SetColor(objectColor_);
	if (pressToA_) pressToA_->SetColor(pressToAColor_);
}

void TitleObject::SetOffsetY(float offsetY)
{
	position_.y = baseY_ + offsetY;
	if (object3D_) object3D_->SetTranslate(position_);
}
