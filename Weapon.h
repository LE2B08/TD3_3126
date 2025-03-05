#pragma once
#include "Player.h"
#include "Object3D.h"
class Weapon {
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

	///============================
	/// Getter & Setter
	///
public:
	void SetPlayer(Player* player) { player_ = player; }
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }

private:
	///============================
	/// メンバ変数
	/// 

	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;
	// プレイヤー
	Player* player_;
	// 位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	//半径
	float radius_ = 0.5f;
	// プレイヤーと武器の距離
	float distance_ = 2.0f;
};
