#pragma once
#include <memory>
#include <Object3D.h>
#include <WorldTransform.h>

class Player;
class Camera;
class PlayerDirectionalArrow
{
public:
	/// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// ImGuiの描画処理
	void DrawImGui();

	// プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

private: /// ---------- メンバ変数 ---------- ///

	// プレイヤー
	Player* player_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	/*------矢印------*/
	std::unique_ptr<Object3D> directionalArrow_;
	WorldTransform arrowWorldTransform_;
};

