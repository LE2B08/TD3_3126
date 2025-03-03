#pragma once
#include <Object3D.h>
class Player {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void DrawImGui();
	// 移動処理
	void Move();
	//// フックの投げる処理
	//void HookThrow();
	//// フックの更新処理
	//void HookUpdate();
	//
	//// フックの引っ張り処理
	//void HookPull();

	//void MoveToOrigin();
	

public:
	///============================
	/// Getter & Setter
	
	// 位置の取得
	Vector3 GetPosition() const { return position_; }
	// 位置の設定
	void SetPosition(const Vector3& position) { position_ = position; }
	// 回転の取得
	Vector3 GetRotation() const { return rotation_; }
	// 回転の設定
	void SetRotation(const Vector3& rotation) { rotation_ = rotation; }
	// スケールの取得
	Vector3 GetScale() const { return scale_; }
	// スケールの設定
	void SetScale(const Vector3& scale) { scale_ = scale; }
	// 速度の取得
	Vector3 GetVelocity() const { return velocity_; }
	// 速度の設定
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }
	// 加速度の取得
	Vector3 GetAcceleration() const { return acceleration_; }
	// 加速度の設定
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }
	// 角速度の取得
	Vector3 GetAngularVelocity() const { return angularVelocity_; }
	// 角速度の設定
	void SetAngularVelocity(const Vector3& angularVelocity) { angularVelocity_ = angularVelocity; }

	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }

private:
	///============================
	/// メンバ変数
	///

	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;
	// 位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// 角速度
	Vector3 angularVelocity_;

	///===========================
	//フック
	//  フックの開始位置
	Vector3 hookStartPos_;
	//  フックの終了位置
	Vector3 hookEndPos_ = {};
	//  フックの回転
	Vector3 hookRotation_;
	//  フックのスケール
	Vector3 hookScale_;
	//  フックの速度
	Vector3 hookVelocity_;
	//  フックの加速度
	Vector3 hookAcceleration_;
	//  フックの角速度
	Vector3 hookAngularVelocity_;
	//  フックのオブジェクト3D
	std::unique_ptr<Object3D> hookObject3D_ = nullptr;
	//  フックのフラグ
	bool isHookActive_ = false;
	//  フックが対象に当たったかどうかのフラグ
	bool isHookAttached_ = false;
	//  フックが対象に到達したかどうかの閾値
	float hookReachThreshold_ = 1.0f;
	//  フックの引っ張り速度
	float hookPullSpeed_ = 0.1f;
	//  フックの解除距離
	float hookDetachThreshold_ = 0.5f;

	// フックの対象位置
	Vector3 targetPosition_ = {0.0f, 0.0f, 0.0f};
};
