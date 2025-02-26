#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <Object3D.h>
#include <WavLoader.h>
#include "ParticleManager.h"
#include <BaseScene.h>

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;
class ImGuiManager;


/// -------------------------------------------------------------
///				　		アキモト専用シーン
/// -------------------------------------------------------------
class SatouScene : public BaseScene
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

	// ImGui描画処理
	void DrawImGui() override;

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager = nullptr;
	Input* input_ = nullptr;

	std::unique_ptr<WavLoader> wavLoader_;

	std::vector<std::unique_ptr<Sprite>> sprites_;

	// テクスチャのパスをリストで管理
	std::vector<std::string> texturePaths_;
};

