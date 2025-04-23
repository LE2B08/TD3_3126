#pragma once
#include <Sprite.h>
#include <TextureManager.h>
#include <Object3D.h>
#include <WavLoader.h>
#include "ParticleManager.h"
#include <BaseScene.h>
#include "AnimationManager.h"

#include "CollisionManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "Weapon.h"
#include "EnemyBullet.h"
#include"Skydome.h"

#include "ControllerUI.h"
#include "DynamicCamera.h"

#include "EffectManager.h"

#include "AABB.h"
#include "OBB.h"
#include <SkyBox.h>

enum class GameSceneState {
	Start,
	Play,
	GameClear,
	GameOver
};

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;
class Camera;

/// -------------------------------------------------------------
///				　		ゲームプレイシーン
/// -------------------------------------------------------------
class GamePlayScene : public BaseScene
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


private: /// ---------- メンバ関数 ---------- ///

	// 衝突判定と応答
	void CheckAllCollisions();

	/*------カメラのシェイク------*/
	void CameraShake();

	// ゲームスタート初期化
	void GameStartInitialize();

	// ゲームスタート更新
	void GameStartUpdate();

	// ゲームプレイ初期化
	void GamePlayInitialize();

	// ゲームプレイ更新
	void GamePlayUpdate();

	// ゲームクリア初期化
	void GameClearInitialize();

	// ゲームクリア更新
	void GameClearUpdate();

	// ゲームオーバー初期化
	void GameOverInitialize();

	// ゲームオーバー更新
	void GameOverUpdate();

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager = nullptr;
	Input* input_ = nullptr;
	ParticleManager* particleManager = nullptr;
	Camera* camera_ = nullptr;

	std::unique_ptr<WavLoader> wavLoader_;
	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<CollisionManager> collisionManager_;

	std::unique_ptr<EffectManager> effectManager_;

	std::string particleGroupName;

	// デバッグカメラのON/OFF用
	bool isDebugCamera_ = false;

	// Playerクラスのインスタンス
	std::unique_ptr<Player> player_ = nullptr;
	// プレイヤーUI
	std::unique_ptr<PlayerUI> playerUI_;
	// プレイヤーの武器
	std::unique_ptr<Weapon> weapon_ = nullptr;

	// フック
	std::unique_ptr<Hook> hook_ = nullptr;

	// 敵
	std::unique_ptr<Enemy> enemy_;
	//敵のUI
	std::unique_ptr<EnemyUI> enemyUI_ = nullptr;

	// フィールド
	std::unique_ptr<Field> field_;

	// コントローラー用UI
	std::unique_ptr<ControllerUI> controllerUI_ = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>>* enemyBullets_;


	std::unique_ptr<SkyBox> skyBox_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_ = nullptr;

	/*------カメラの座標------*/
	Vector3 cameraPosition_ = { 0.0f, 50.0f, 0.0f };

	// カメラの揺れを管理する変数
	bool isCameraShaking_ = false;
	// 揺れの持続時間
	float shakeDuration_ = 0.05f;
	// 揺れの強さ
	float shakeMagnitude_ = 0.5f;
	float shakeElapsedTime_ = 0.0f;

	/*------ゲーム開始演出------*/

	// ゲームクリアのフラグ
	bool isGameClear_ = false;

	// イージングがスタートしたか
	bool isStartAnimation_ = false;

	// 計算用のダイナミックカメラ
	std::unique_ptr <DynamicCamera> dynamicCamera_ = nullptr;

	// ゲームの状態
	GameSceneState gameState_ = GameSceneState::Start;

	// 次の状態をリクエスト
	std::optional<GameSceneState> nextGameState_ = std::nullopt;
};
