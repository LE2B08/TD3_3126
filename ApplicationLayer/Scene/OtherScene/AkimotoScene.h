#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <Object3D.h>
#include <WavLoader.h>
#include "ParticleManager.h"
#include <BaseScene.h>
#include "GamePlayScene/Enemy/Enemy.h"
#include "GamePlayScene/Field/Field.h"
#include "Player.h"
#include "Camera.h"
#include "GamePlayScene/DynamicCamera/DynamicCamera.h"

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

private: /// ---------- メンバ変数 ---------- ///
	
	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager = nullptr;
	Input* input_ = nullptr;
	std::unique_ptr<WavLoader> wavLoader_;
	std::vector<std::unique_ptr<Sprite>> sprites_;
	
	// テクスチャのパスをリストで管理
	std::vector<std::string> texturePaths_;
	
	std::unique_ptr<Player> player_;

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// フィールド
	std::unique_ptr<Field> field_;

	// カメラ
	Camera* camera_ = nullptr;

	// 計算用のダイナミックカメラ
	std::unique_ptr <DynamicCamera> dynamicCamera_ = nullptr;
};

