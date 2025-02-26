#pragma once
#include "Object3D/Object3D.h"
#include <memory>
#include <imgui.h>

/// === 敵 === ///
class Enemy {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// コンストラクタ
	Enemy();

	/// デストラクタ
	~Enemy();

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 描画
	void Draw();

    /// ImGui
    void ShowImGui(const char* name);

	/// 移動
	void Move();

	///-------------------------------------------/// 
	/// ゲッター&セッター
	///-------------------------------------------///
public:


	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// 敵のオブジェクト
	std::unique_ptr<Object3D> objectEnemy_;

	// スケール
	Vector3 Scale_;

	// 回転
	Vector3 Rotate_;

	// 位置
	Vector3 position_;

	// 速度
	Vector3 velocity_;

	// 加速度
	Vector3 acceleration_;

	// 速度の最大値
	float maxVelocity_ = 0.1f;

	// 加速度の最大値
	float maxAcceleration_ = 0.01f;
};

