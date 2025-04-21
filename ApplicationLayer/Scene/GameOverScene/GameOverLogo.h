#pragma once
#include "Object3D.h"


/// -------------------------------------------------------------
///				　		ゲームオーバーログクラス
/// -------------------------------------------------------------
class GameOverLogo
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// ImGui描画処理
	void DrawImGui();

private: /// ---------- メンバ変数 ---------- ///

	float EaseOut(float t) {
		return 1 - powf(1 - t, 3); // 三乗で減速
	}

	std::unique_ptr<Object3D> object3D_; // 3Dオブジェクト
	std::unique_ptr<Object3D> pressToA_; // PressToAオブジェクト

	float timer_ = 0.0f; // タイマー

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
};

