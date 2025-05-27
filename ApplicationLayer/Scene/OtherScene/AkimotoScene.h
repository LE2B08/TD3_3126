#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <Object3D.h>
#include <WavLoader.h>
#include "ParticleManager.h"
#include <BaseScene.h>
#include "Camera.h"

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;

/// -------------------------------------------------------------
///				　		アキモト専用シーン
/// -------------------------------------------------------------
class AkimotoScene : public BaseScene
{
public: /// ---------- メンバ関数 ---------- ///

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

	// ゲージの進行度
	void GaugeProgress(int count, int maxCount);

private: /// ---------- メンバ変数 ---------- ///
	
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager = nullptr;
	Input* input_ = nullptr;
	std::unique_ptr<WavLoader> wavLoader_;
	std::vector<std::unique_ptr<Sprite>> sprites_;
	
	// テクスチャのパスをリストで管理
	std::vector<std::string> texturePaths_;

	// カメラ
	Camera* camera_ = nullptr;

	// ゲージ用スプライト
	std::unique_ptr<Sprite> gauge_;

	// ゲージのデフォルトサイズ
	const Vector2 gaugeDefaultSize_ = { 640.0f, 32.0f };

	// 進行度(回数)
	int count_ = 0;

	// 最大値(回数)
	int maxCount_ = 10;
};

