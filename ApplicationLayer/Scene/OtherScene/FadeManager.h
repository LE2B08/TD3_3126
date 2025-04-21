#pragma once
#include "Sprite.h"
#include "Vector4.h"
#include <functional>


/// -------------------------------------------------------------
///			　フェード・イン・アウトを管理するクラス
/// -------------------------------------------------------------
class FadeManager
{
public: /// ---------- 列挙型 ---------- ///

	// フェードの種類
	enum class FadeState
	{
		None,     // フェードなし
		FadeIn,   // フェードイン
		FadeOut,  // フェードアウト
		Complete, // フェード完了
	};

public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// フェードイン開始
	void StartFadeToWhite(float fadeSpeed = 0.02f, std::function<void()> onFinished = nullptr);

	// フェードアウト開始
	void StartFadeFromWhite(float fadeSpeed = 0.02f, std::function<void()> onFinished = nullptr);

	// 現在のフェード状態を取得
	FadeState GetFadeState() const { return fadeState_; }

private: /// ---------- メンバ変数 ---------- ///

	std::unique_ptr<Sprite> fadeSprite_ = nullptr; // フェード用スプライト

	// フェード状態
	FadeState fadeState_ = FadeState::None;

	float alpha_ = 0.0f; // アルファ値
	float fadeSpeed_ = 0.02f; // フェード速度
	std::function<void()> onFinished_ = nullptr; // フェード完了時のコールバック関数
};

