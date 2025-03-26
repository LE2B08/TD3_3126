#include "FadeManager.h"
#include <LogString.h>
#include "Wireframe.h"
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
	Wireframe::GetInstance()->DrawBox({ -200.0f, -200.0f, 0.0f }, { WinApp::kClientWidth,WinApp::kClientHeight,0.0f }, { fadeColor_.x,fadeColor_.y,fadeColor_.z,alpha_ });
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