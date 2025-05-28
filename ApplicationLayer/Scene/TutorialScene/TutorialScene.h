#pragma once
#include "AnimationManager.h"
#include "ParticleManager.h"
#include <BaseScene.h>
#include <Object3D.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <WavLoader.h>

#include "CollisionManager.h"
#include "Field.h"
#include "PlayerDirectionalArrow.h"
#include "TutorialEnemy.h"
#include "TutorialPlayer.h"
#include "Weapon.h"

#include "ControllerUI.h"
#include "DynamicCamera.h"
#include "Pause/PauseMenu.h"
#include "TutorialUI.h"

#include "EffectManager.h"

#include "AABB.h"
#include "OBB.h"
#include <SkyBox.h>

// チュートリアルの流れ
enum class TutorialSteps {
	Start,            // 初め
	PlayerRotation,   // プレイヤーの回転
	HookThrowAndBack, // フックの投げ、戻す
	HookArcMove,      // フックの弧を描く移動
	HookMove,         // フックの移動
	Attack,           // 攻撃
	End,              // 終わり
};

enum class SceneStatus {
	Play,
	Pause,
};

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Input;
class Camera;

/// -------------------------------------------------------------
///				　		チュートリアルシーン
/// -------------------------------------------------------------
class TutorialScene : public BaseScene {
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

public: /// ---------- クラス内関数 ---------- ///

	// 衝突判定と応答
	void CheckAllCollisions();

	// ゲーム開始時の初期化
	void TutorialStartInitialize();
	// チュートリアル開始時の更新
	void TutorialStartUpdate();

	// プレイヤーの回転の初期化
	void TutorialPlayerRotationInitialize();
	// プレイヤーの回転初期化の更新
	void TutorialPlayerRotationUpdate();

	// フックを投げて戻す初期化
	void TutorialHookThrowAndBackInitialize();
	// フックを投げて戻す更新
	void TutorialHookThrowAndBackUpdate();

	// フックの弧を描く移動初期化
	void TutorialHookArcMoveInitialize();
	// フックの弧を描く移動更新
	void TutorialHookArcMoveUpdate();

	// フックの移動初期化
	void TutorialHookMoveInitialize();
	// フックの移動更新
	void TutorialHookMoveUpdate();

	// 攻撃初期化
	void TutorialAttackInitialize();
	// 攻撃更新
	void TutorialAttackUpdate();

	// ゲーム開始初期化
	void PlayInitialize();
	// ゲーム開始更新
	void PlayUpdate();
	// ポーズ初期化
	void PauseInitialize();
	// ポーズ更新
	void PauseUpdate();

	// チュートリアル終了初期化
	void TutorialEndInitialize();
	// チュートリアル終了更新
	void TutorialEndUpdate();

	void SetTutorialStep(TutorialSteps step);

	// ゲージの進行度
	void GaugeProgress(float count, float maxCount);

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Camera* camera_ = nullptr;
	TextureManager* textureManager = nullptr;
	ParticleManager* particleManager = nullptr;

	std::unique_ptr<WavLoader> wavLoader_;
	std::vector<std::unique_ptr<Sprite>> sprites_;
	std::unique_ptr<CollisionManager> collisionManager_;

	std::string particleGroupName;

	// デバッグカメラのON/OFF用
	bool isDebugCamera_ = false;

	// Playerクラスのインスタンス
	std::unique_ptr<TutorialPlayer> player_ = nullptr;

	// プレイヤーの武器
	std::unique_ptr<Weapon> weapon_ = nullptr;

	// フック
	std::unique_ptr<Hook> hook_ = nullptr;

	// 敵
	std::unique_ptr<TutorialEnemy> enemy_;

	// フィールド
	std::unique_ptr<Field> field_;

	// コントローラー用UI
	std::unique_ptr<ControllerUI> controllerUI_ = nullptr;

	// 敵の弾
	std::list<std::unique_ptr<EnemyBullet>>* enemyBullets_;

	std::unique_ptr<SkyBox> skyBox_;

	/*------カメラの座標------*/
	Vector3 cameraPosition_ = { 0.0f, 50.0f, 0.0f };

	/*------ゲーム開始演出------*/

	// 計算用のダイナミックカメラ
	std::unique_ptr<DynamicCamera> dynamicCamera_ = nullptr;

	// ゲームの状態
	SceneStatus sceneStatus_ = SceneStatus::Play; // 例: 初期値
	std::optional<SceneStatus> nextGameState_ = std::nullopt;

	// チュートリアルの進行状態
	TutorialSteps tutorialSteps_ = TutorialSteps::Start;

	// ポーズメニュー
	std::unique_ptr<PauseMenu> pauseMenu_ = nullptr;

	// プレイヤーの矢印
	std::unique_ptr<PlayerDirectionalArrow> playerDirectionalArrow_;

	///-------------------------------------------/// 
	/// チュートリアル用UI
	///-------------------------------------------///

		// チュートリアルUIのスプライト
	std::unique_ptr<TutorialUI> tutorialUI_ = nullptr;

	// 回った時間
	float rotationStepTimer_ = 0.0f;
	// プレイヤーのフックを投げる回数
	int hookThrowCount_ = 0;
	// フックの戻す回数
	int hookBackCount_ = 0;
	// フックの弧移動時の時間
	float arcMoveStepTimer_ = 0.0f;

	// フックの移動時の回数
	int hookActiveCount_ = 0;
	// 攻撃が行われた回数
	int playerAttackCount_ = 0;

	// フックを投げる最大回数
	const int maxHookThrowCount_ = 2;
	// フックを戻す最大回数
	const int maxHookBackCount_ = 2;
	// フックの移動最大回数
	const int maxHookActiveCount_ = 3;
	// プレイヤーの攻撃最大回数
	const int maxPlayerAttackCount_ = 3;

	// ゲージ用スプライト
	std::unique_ptr<Sprite> gauge_;

	// ゲージの位置(左上)
	Vector2 gaugePosition_ = { 0.0f, 0.0f };

	// ゲージのデフォルトサイズ
	const Vector2 gaugeDefaultSize_ = { 640.0f, 32.0f };
};