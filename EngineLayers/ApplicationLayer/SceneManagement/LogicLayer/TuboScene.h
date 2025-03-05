#pragma once
#include <BaseScene.h>
#include <TitleScene.h>
#include <GamePlayScene.h>
#include "AbstractSceneFactory.h"
#include"Camera.h"

#include "Player.h"
#include "GamePlayScene/Enemy/Enemy.h"
#include "GamePlayScene/Field/Field.h"

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;
class ImGuiManager;


/// -------------------------------------------------------------
///					　	ツボキ専用クラス
/// -------------------------------------------------------------
class TuboScene : public BaseScene
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

	// Playerクラスのインスタンス
	std::unique_ptr<Player> player_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;
	Vector3 cameraPos_ = {0.0f, 45.0f, 0.0f};
	Vector3 cameraRotate_ = {1.6f, 0.0f, 0.0f};
	Vector3 cameraScale_ = {1.0f, 1.0f, 1.0f};

	// 敵
	std::unique_ptr<Enemy> enemy_;

	// フィールド
	std::unique_ptr<Field> field_;
};

