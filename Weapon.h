#pragma once
#include "Object3D.h"
#include"Collider.h"
class Weapon :public Collider {
public:
	///============================
	/// メンバ関数
	///

	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// 終了処理
	void Finalize();
	// ImGui描画処理
	void DrawImGui();
	// 攻撃処理
	void Attack();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	///============================
	/// Getter & Setter
	///
public:
	// 位置
	void SetPlayerPosition(const Vector3& position) { playerPosition_ = position; }
	Vector3 GetPlayerPosition() const { return playerPosition_; }
	// 回転
	void SetPlayerRotation(const Vector3& rotation) { playerRotation_ = rotation; }
	Vector3 GetPlayerRotation() const { return playerRotation_; }
	// スケール
	void SetPlayerScale(const Vector3& scale) { playerScale_ = scale; }
	Vector3 GetPlayerScale() const { return playerScale_; }
	// 攻撃のフラグ
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack() const { return isAttack_; }
	// カメラの設定
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }

private:
	///============================
	/// メンバ変数
	/// 

	WorldTransform worldTransform_;
	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;
	
	// 位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	// プレイヤーの位置
	Vector3 playerPosition_;
	// プレイヤーの回転
	Vector3 playerRotation_;
	// プレイヤーのスケール
	Vector3 playerScale_;
	//半径
	float radius_ = 0.5f;
	// プレイヤーと武器の距離
	float distance_ = 2.0f;

	bool isAttack_ = false;

	/*------攻撃の持続時間------*/
	float attackTime_ = 0.0f;

	float attackMaxTime_ = 60.0f;

	/*-------攻撃中の回転角度------*/
	float attackRotationAngle_ = 0.0f;

	/*------回転速度------*/
	float rotationSpeed_ = 5.0f; // デフォルトの回転速度
};
