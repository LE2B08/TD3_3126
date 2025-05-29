#pragma once
#include <BaseScene.h>
#include <TitleScene.h>
#include <GamePlayScene.h>
#include "AbstractSceneFactory.h"
#include"Camera.h"

#include "Player.h"
#include "GamePlayScene/Enemy/Enemy.h"
#include "GamePlayScene/Field/Field.h"
#include "CollisionManager.h"

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;


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


private: /// ---------- メンバ関数 ---------- ///

	// 衝突判定と応答
	void CheckAllCollisions();

	/*------カメラのシェイク------*/
	void CameraShake();

	/*------ゲーム開始演出------*/
	void GameStart();

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon_ = nullptr;
	TextureManager* textureManager = nullptr;
	Input* input_ = nullptr;
	ParticleManager* particleManager = nullptr;
	Camera* camera_ = nullptr;

	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<CollisionManager> collisionManager_;

	EffectManager* effectManager_;

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

	// フィールド
	std::unique_ptr<Field> field_;

	// コントローラー用UI
	std::unique_ptr<ControllerUI> controllerUI_ = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>>* enemyBullets_;


	std::unique_ptr<SkyBox> skyBox_;

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
	// スタートしたか
	bool isGameStart_ = false;

	// イージングがスタートしたか
	bool isStartEasing_ = false;

	// プレイヤーの位置がセットされたか
	bool isPlayerPositionSet_ = false;

	// タイマー用のメンバ変数
	float startTimer_ = 0;
	const float maxStartT_ = 40;

	// プレイヤー用のタイマー
	float playerStartTimer_ = 0;
	const float maxPlayerStartT_ = 40;

	// デフォルトのフィールド
	const Vector3 defaultFieldScale_ = { 10.0f,1.0f,10.0f };
	const Vector3 defaultFieldPosition_ = { 0.0f,0.0f,0.0f };

	// 初期のフィールド
	const Vector3 startFieldScale_ = { 0.0f,0.0f,0.0f };
	const Vector3 startFieldPosition_ = { 0.0f,0.0f,0.0f };

	bool isGameStartEffectEnabled_ = true;

	// 拡大するフィールド
	Vector3 fieldScale_ = startFieldScale_;
	Vector3 fieldPosition_ = startFieldPosition_;

	// 計算用のダイナミックカメラ
	std::unique_ptr <DynamicCamera> dynamicCamera_ = nullptr;};

