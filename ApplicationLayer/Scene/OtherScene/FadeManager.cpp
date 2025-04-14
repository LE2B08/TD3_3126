#include "FadeManager.h"
#include "TextureManager.h"


/// -------------------------------------------------------------
///				　	        初期化処理
/// -------------------------------------------------------------
void FadeManager::Initialize()
{
	// テクスチャの初期化
	TextureManager::GetInstance()->LoadTexture("Resources/white.png");

	fadeSprite_ = std::make_unique<Sprite>();
	fadeSprite_->Initialize("Resources/white.png");
	//fadeSprite_->SetSize({ static_cast<float>(width), static_cast<float>(height) });
	//fadeSprite_->SetPosition({ static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f });
	fadeSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ });
}


/// -------------------------------------------------------------
///				　	        　更新処理
/// -------------------------------------------------------------
void FadeManager::Update()
{
	// フェード状態に応じて処理を分岐
	if (fadeState_ == FadeState::FadeIn)
	{
		alpha_ -= fadeSpeed_; // アルファ値を減少

		if (alpha_ <= 0.0f)
		{
			alpha_ = 0.0f;					  // アルファ値を0.0に設定
			fadeState_ = FadeState::Complete; // フェード完了状態に遷移
			if (onFinished_) onFinished_();   // コールバック関数を呼び出す
		}
	}
	else if (fadeState_ == FadeState::FadeOut)
	{
		alpha_ += fadeSpeed_; // アルファ値を増加
		if (alpha_ >= 1.0f)
		{
			alpha_ = 1.0f; // アルファ値を1.0に設定
			fadeState_ = FadeState::Complete; // フェード完了状態に遷移
			if (onFinished_) onFinished_(); // コールバック関数を呼び出す
		}
	}

	fadeSprite_->SetColor({ 1.0f, 1.0f, 1.0f, alpha_ }); // スプライトの色を更新
	fadeSprite_->Update(); // スプライトの更新
}


/// -------------------------------------------------------------
///				　	          描画処理
/// -------------------------------------------------------------
void FadeManager::Draw()
{
	if (fadeState_ == FadeState::FadeIn || fadeState_ == FadeState::FadeOut ||
		(fadeState_ == FadeState::Complete && alpha_ > 0.0f)) {
		fadeSprite_->Draw();
	}
}


/// -------------------------------------------------------------
///				　	      フェードイン開始
/// -------------------------------------------------------------
void FadeManager::StartFadeToWhite(float fadeSpeed, std::function<void()> onFinished)
{
	fadeState_ = FadeState::FadeOut; // フェードイン状態に遷移
	fadeSpeed_ = fadeSpeed;			// フェード速度を設定
	alpha_ = 0.0f;					// アルファ値を1.0に設定	
	onFinished_ = onFinished;		// コールバック関数を設定
}


/// -------------------------------------------------------------
///				　	      フェードアウト開始
/// -------------------------------------------------------------
void FadeManager::StartFadeFromWhite(float fadeSpeed, std::function<void()> onFinished)
{
	fadeState_ = FadeState::FadeIn; // フェードアウト状態に遷移
	fadeSpeed_ = fadeSpeed;			 // フェード速度を設定
	alpha_ = 1.0f;					 // アルファ値を0.0に設定
	onFinished_ = onFinished;		 // コールバック関数を設定
}
