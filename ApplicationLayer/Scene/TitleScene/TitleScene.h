#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <WavLoader.h>
#include <SRVManager.h>
#include "Input.h"
#include <BaseScene.h>

#include "TitleObject.h"


/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Camera;


/// -------------------------------------------------------------
///					　ゲームタイトルシーンクラス
/// -------------------------------------------------------------
class TitleScene : public BaseScene
{
private: /// ---------- 列挙型 ---------- ///
	
	// シーンの状態
	enum class TitleState
	{
		Appear,     // 登場演出中
		Idle,       // A待ち状態
		Exit,       // ロゴ退場演出中
		Transition, // シーン遷移フェード中
		Operate,    // 操作方法
	};

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
	TextureManager* textureManager = nullptr;
	Input* input = nullptr;
	Camera* camera_ = nullptr;


	TitleState titleState_ = TitleState::Appear;
	float exitTimer_ = 0.0f;

	std::unique_ptr<WavLoader> wavLoader_;

	std::unique_ptr<TitleObject> titleObject_;

	std::vector<std::unique_ptr<Sprite>> sprites_;

	// テクスチャのパスをリストで管理
	std::vector<std::string> texturePaths_;

	bool cameraShakeEnabled_;

	std::unique_ptr<Sprite> cameraShakeOnSprite_;

	std::unique_ptr<Sprite> cameraShakeOffSprite_;
};

