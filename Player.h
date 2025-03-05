#pragma once
#include <Object3D.h>
#include"Collider.h"
class Player:public Collider {

public:
	///============================
	/// メンバ関数
	/// 
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void DrawImGui();
	// 移動処理
	void Move();
	// フックの投げる処理
	void HookThrow();
	// フックの更新処理
	void MoveToHook();
	// フックの元の位置に戻る処理
	void ExtendHook();
	
	

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

	// 移動制限
	//  移動制限の最大値
	Vector3 maxMoveLimit_ = {8.0f, 0.0f, 8.0f};
	//  移動制限の最小値
	Vector3 minMoveLimit_ = {-8.0f, 0.0f, -8.0f};

	///===========================
	//フック
	//  フックの開始位置
	Vector3 hookStartPos_;
	//  フックの現在位置
	Vector3 hookCurrentPos_;
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

	// フックの状態を管理する変数
	bool isHookActive_ = false;
	// フックが伸びているかどうか
	bool isHookExtending_ = false;

	// フックの開始時間
	std::chrono::time_point<std::chrono::steady_clock> hookStartTime_;

	// フックの移動速度
	float hookSpeed_ = 5.0f; 

	///============================
	/// Debug
	/// 
	//デバッグフラグ
	// デバッグモードになるとプレイヤーの移動ができるようになる
	bool isDebug_ = false;
};
