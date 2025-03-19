#pragma once
#include "Vector4.h"


/// -------------------------------------------------------------
///			　フェード・イン・アウトを管理するクラス
/// -------------------------------------------------------------
class FadeManager
{
public: /// ---------- 列挙型 ---------- ///

	// フェードの種類
	enum class FadeState
	{
		None,
		FadeIn,
		FadeOut,
		Complete
	};

public: /// ---------- メンバ関数 ---------- ///

	// シングルトンインスタンス
	static FadeManager* GetInstance();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// フェード開始
	void StartFadeIn(float speed = 1.0f, const Vector4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	void StartFadeOut(float speed = 1.0f, const Vector4& color = { 1.0f, 1.0, 1.0f, 1.0f });

	bool IsFadeComplete() const;

	// 現在のフェード状態を取得
	FadeState GetFadeState() const { return fadeState_; }

private: /// ---------- メンバ変数 ---------- ///

	FadeState fadeState_ = FadeState::None;

	float alpha_ = 0.0f;
	float fadeSpeed_ = 1.0f;
	Vector4 fadeColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	float kDeltaTime = 1.0f / 60.0f;

private: /// ---------- コピー禁止 ---------- ///

	FadeManager() = default;
	~FadeManager() = default;
	FadeManager(const FadeManager&) = delete;
	FadeManager& operator=(const FadeManager&) = delete;
};

