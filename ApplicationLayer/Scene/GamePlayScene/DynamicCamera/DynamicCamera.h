#pragma once
#include "WorldTransform.h"

/// ---------- 前方宣言 ---------- ///
class Player;
class TutorialPlayer;
class Enemy;
class TutorialEnemy;
class BasePlayer;
class BaseEnemy; 

/// -------------------------------------------------------------
///					ダイナミックカメラクラス
/// -------------------------------------------------------------
class DynamicCamera
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// ImGui表示
	void ShowImGui(const char* name);

public: /// ---------- ゲッター ---------- ///

	// スケールの取得
	const Vector3& GetScale() const { return worldTransform_.scale_; }

	// 回転の取得
	const Vector3& GetRotate() const { return worldTransform_.rotate_; }

public: /// ---------- セッター ---------- ///

	// 移動の取得
	const Vector3& GetTranslate() const { return worldTransform_.translate_; }

	// 座標のセット
	void SetTranslate(const Vector3& translate) { worldTransform_.translate_ = translate; }

	void SetPlayer(BasePlayer* player) { player_ = player; }

	// 敵の設定

	void SetEnemy(BaseEnemy* enemy) { enemy_ = enemy; }

private: /// ---------- メンバ変数 ---------- ///

	// ワールド変換
	WorldTransform worldTransform_;

	// プレイヤー借りポインタ
	BasePlayer* player_ = nullptr;

	// 敵の借りポインタ
	BaseEnemy* enemy_ = nullptr;
	
	// 中心座標
	Vector3 centerPos_ = {};

	// 距離
	float distance_ = 0.0f;

	// ズーム
	float zoom_ = 0.0f;

	// ズーム最小値
	const float zoomMin_ = 50.0f + 20.0f;

	// ズーム最大値
	const float zoomMax_ = 150.0f + 30.0f;
};

