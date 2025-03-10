#pragma once
#include <Object3D.h>
class SkyDome {

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
};
