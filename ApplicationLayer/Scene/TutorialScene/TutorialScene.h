#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <Object3D.h>
#include <BaseScene.h>

#include <memory>

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;
class Camera;


/// -------------------------------------------------------------
///				　		ゲームオーバーシーン
/// -------------------------------------------------------------
class TutorialScene : public BaseScene
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize() override;

	// 更新処理
	void Update() override;

	// 描画処理
	void Draw() override;

	// 終了処理
	void Finalize() override;

	void DrawImGui() override;

private: /// ---------- メンバ変数 ---------- ///
	
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
};
