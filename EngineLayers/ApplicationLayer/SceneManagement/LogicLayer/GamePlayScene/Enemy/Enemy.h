#pragma once
#include "Object3D/Object3D.h"
#include <memory>
#include "Collider.h"

/// === 敵 === ///
class Enemy:public Collider {

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

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;


	///-------------------------------------------/// 
	/// ゲッター&セッター
	///-------------------------------------------///
public:


	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	WorldTransform worldTransform_;

	// 敵のオブジェクト
	std::unique_ptr<Object3D> objectEnemy_;

	// スケール
	Vector3 scale_;

	// 回転
	Vector3 rotate_;

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

