#include "FadeManager.h"
#include <LogString.h>
#include "Wireframe.h"
#include "Camera.h"
#include "Object3DCommon.h"
#include <WinApp.h>


/// -------------------------------------------------------------
///				　	シングルトンインスタンス
/// -------------------------------------------------------------
FadeManager* FadeManager::GetInstance()
{
	static FadeManager instance;
	return &instance;
}


/// -------------------------------------------------------------
///				　	        初期化処理
/// -------------------------------------------------------------
void FadeManager::Initialize()
{
	camera_ = Object3DCommon::GetInstance()->GetDefaultCamera();
	object3DCommon_ = Object3DCommon::GetInstance();
	fadeState_ = FadeState::None;
	alpha_ = 0.0f;
}


/// -------------------------------------------------------------
///				　	        　更新処理
/// -------------------------------------------------------------
void FadeManager::Update()
{
	if (fadeState_ == FadeState::FadeIn)
	{
		alpha_ += fadeSpeed_ * kDeltaTime;
		if (alpha_ >= 1.0f)
		{
			alpha_ = 1.0f;
			fadeState_ = FadeState::Complete;
		}
	}
	else if (fadeState_ == FadeState::FadeOut)
	{
		alpha_ -= fadeSpeed_ * kDeltaTime;
		if (alpha_ <= 0.0f)
		{
			alpha_ = 0.0f;
			fadeState_ = FadeState::Complete;
		}
	}
}


/// -------------------------------------------------------------
///				　	          描画処理
/// -------------------------------------------------------------
void FadeManager::Draw()
{
	if (fadeState_ == FadeState::None) return; // 状態がNoneなら描画しない

	// ワイヤーフレームでフェードの可視化
	Vector3 pos(0.0f, 0.0f, 0.0f);
	Vector3 size(300.0f, 100.0f, 0.0f);
	float angle = 70.0f * (3.14159f / 180.0f); // 度からラジアンに変換

	// ワイヤーフレームの描画
	Wireframe::GetInstance()->DrawBox(pos, size, { fadeColor_.x,fadeColor_.y,fadeColor_.z,alpha_ }, angle, camera_->GetRotate());
}

void FadeManager::StartFadeIn(float speed, const Vector4& color)
{
	fadeState_ = FadeState::FadeIn;
	fadeSpeed_ = speed;
	fadeColor_ = color;
	alpha_ = 0.0f;
}

void FadeManager::StartFadeOut(float speed, const Vector4& color)
{
	fadeState_ = FadeState::FadeOut;
	fadeSpeed_ = speed;
	fadeColor_ = color;
	alpha_ = 1.0f;
}

bool FadeManager::IsFadeComplete() const
{
	return fadeState_ == FadeState::Complete;
}