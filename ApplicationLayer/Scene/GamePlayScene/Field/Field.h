#pragma once
#include "Object3D.h"

/// -------------------------------------------------------------
///						フィールドクラス
/// -------------------------------------------------------------
class Field 
{
public: /// ---------- メンバ関数 ---------- ///

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// ImGui描画
	void ShowImGui(const char* name);

	// スタート時の拡縮の挙動
	void ScalingAnimation();

	void TutorialScale();

public: /// ---------- ゲッター ---------- ///

	Vector3 GetMinPosition() { return minPosition_; }

	Vector3 GetMaxPosition() { return maxPosition_; }

	// 拡縮が終わったかどうかの取得
	bool IsScaleEnd() { return isScaleEnd_; }

public: /// ---------- セッター ---------- ///

	void SetMinPosition(const Vector3& minPosition) { minPosition_ = minPosition; }

	void SetMaxPosition(const Vector3& maxPosition) { maxPosition_ = maxPosition; }

	void SetScale(const Vector3& scale) { scale_ = scale; }

	void SetRotate(const Vector3& rotate) { rotate_ = rotate; }

	void SetPosition(const Vector3& position) { position_ = position; }

private: /// ---------- メンバ変数 ---------- ///

	// オブジェクト
	std::unique_ptr<Object3D> objectField_ = nullptr;

	// スケール
	Vector3 scale_;

	// 回転
	Vector3 rotate_;

	// 位置
	Vector3 position_;

	Vector3 minPosition_;
	Vector3 maxPosition_;

///-------------------------------------------/// 
/// 演出用の変数
///-------------------------------------------///

	// タイマー
	float scalingTimer_ = 0.0f;

	// 拡縮の最大時間(フレーム)
	const float maxScalingTime_ = 60.0f;

	// 拡縮アニメーションが終わったかどうか
	bool isScaleEnd_ = false;

	const Vector3 startScale_ = { 0.0f,0.0f,0.0f };

	const Vector3 defaultScale_ = { 10.0f,1.0f,10.0f };
};

