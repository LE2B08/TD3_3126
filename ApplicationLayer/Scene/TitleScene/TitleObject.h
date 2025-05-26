#pragma once
#include <Object3D.h>
#include <Vector3.h>

#include <numbers>

class TitleObject
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// カメラの揺れがオンの描画
	void DrawCameraShakeOn();

	// カメラの揺れがオフの描画
	void DrawCameraShakeOff();

	// 終了処理
	void Finalize();

	// ImGui描画処理
	void DrawImGui();

	// ロゴの退場アニメーションを開始
	void StartExitAnimation();

	// ロゴの退場アニメーションが完了したかどうか
	bool IsExitAnimationComplete() const { return isExiting_ && exitTime_ >= exitDuration_; }

	// α値の設定
	void SetAlpha(float alpha);

	// オフセットY座標の設定
	void SetOffsetY(float offsetY);

private:

	// 線形補間
	float EaseOutBack(float t, float overshoot = 1.70158f)
	{
		t -= 1.0f;
		return (t * t * ((overshoot + 1) * t + overshoot)) + 1.0f;
	}

	float EaseOut(float t) {
		return 1 - powf(1 - t, 3); // 三乗で減速
	}

	float EaseOutElastic(float t) {
		const float c4 = (2 * 3.14159f) / 3;
		return t == 0 ? 0 : t == 1 ? 1 : powf(2, -10 * t) * sinf((t * 10 - 0.75f) * c4) + 1;
	}

	float EaseOutBounce(float t) {
		if (t < 1 / 2.75f) {
			return 7.5625f * t * t;
		}
		else if (t < 2 / 2.75f) {
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		}
		else if (t < 2.5 / 2.75) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		}
		else {
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	}

	float EaseOutCubic(float t) {
		return 1.0f - powf(1.0f - t, 3);
	}

	float EaseInBack(float t, float overshoot = 1.70158f)
	{
		return t * t * ((overshoot + 1.0f) * t - overshoot);
	}

private: /// ---------- メンバ変数 ---------- ///

	std::unique_ptr<Object3D> object3D_; // 3Dオブジェクト
	std::unique_ptr<Object3D> pressToA_; // 3Dオブジェクト
	std::unique_ptr<Object3D> pressBToGuide_; // 3Dオブジェクト
	std::unique_ptr<Object3D> cameraShakeOn_; // 3Dオブジェクト
	std::unique_ptr<Object3D> cameraShakeOff_; // 3Dオブジェクト

	Vector3 position_{}; // 位置
	Vector3 rotation_{}; // 回転
	Vector3 scale_ = { 4.0f,4.0f,4.0f }; // スケール
	
	Vector4 objectColor_ = { 1.0f,1.0f,1.0f,1.0f }; // オブジェクトの色
	Vector4 pressToAColor_ = { 1.0f,1.0f,1.0f,1.0f }; // PressToAの色
	float baseY_ = 0.0f; // 基準Y座標

	float moveTime_ = 0.0f;
	float moveDuration_ = 1.2f; // 1.2秒で到達（調整可能）

	bool isExiting_ = false; // 退場中かどうか
	float exitTime_ = 0.0f; // 退場時間
	const float exitDuration_ = 1.0f; // 退場時間（調整可能）
};

