#define NOMINMAX
#include "GameOverLogo.h"

void GameOverLogo::Initialize()
{
	object3D_ = std::make_unique<Object3D>();
	object3D_->Initialize("GameOverLogo.gltf");

	pressToA_ = std::make_unique<Object3D>();
	pressToA_->Initialize("PressAToTitle.gltf");

	position_ = Vector3(0.0f, 0.0f, 0.0f); // スタート位置に近い値
	rotation_ = Vector3(0.0f, 3.14f, 0.0f);    // 初期角度
	scale_ = Vector3(3.0f, 3.0f, 3.0f);       // 初期スケール

	moveTime_ = 0.0f;
	moveDuration_ = 1.5f; // 再生時間（演出に合わせて調整）
}

void GameOverLogo::Update()
{
	timer_ += 1.0f;

	float t = std::min(timer_ / 120.0f, 1.0f);
	Vector3 start = Vector3(-60, 10, 0);
	Vector3 end = Vector3(0, 0, 6);
	position_ = Vector3::Lerp(start, end, EaseOut(t));

	if (t >= 1.0f)
	{
		// 到達後はずっと揺れる（Z軸）
		float swing = std::sin((timer_ - 120.0f) * 0.1f); // 周期：0.1
		rotation_.z = swing * 0.2f; // 振幅：0.3ラジアン（約17度）
	}
	else
	{
		// 移動中は固定回転
		rotation_.z = 0.0f;
	}

	object3D_->SetTranslate(position_);
	object3D_->SetRotate(rotation_);
	object3D_->SetScale(scale_);
	object3D_->Update();

	// moveTime_ 進めておく（揺れが続く）
	moveTime_ += 1.0f / 60.0f;

	// 上記を組み合わせてUpdate内で適用
	float alpha = 0.5f + 0.5f * sinf(moveTime_ * 2.0f);
	float floatY = sinf(moveTime_ * 2.0f) * 0.1f;
	float scaleFactor = 1.0f + 0.05f * sinf(moveTime_ * 4.0f);

	pressToA_->SetTranslate(Vector3(0.0f, 0.1f + floatY, -8.0f));
	pressToA_->SetScale(Vector3(2.0f, 2.0f, 2.0f) * scaleFactor);
	pressToA_->SetRotate(Vector3(0.0f, std::numbers::pi_v<float>, 0.0f));
	pressToA_->Update();
}

void GameOverLogo::Draw()
{
	pressToA_->Draw();
	object3D_->Draw();
}

void GameOverLogo::DrawImGui()
{
}
