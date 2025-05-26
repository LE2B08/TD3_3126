#pragma once
#include "Vector3.h"


/// ---------- 前方宣言 ---------- ///
class Camera;
class Input;
class Player;
class Field;
class DynamicCamera;


/// -------------------------------------------------------------
///					　演出を管理するクラス
/// -------------------------------------------------------------
class EffectManager
{
public: /// ---------- メンバ関数 ---------- ///
	// シングルトンインスタンス取得
	static EffectManager* GetInstance();


	// 初期化処理
	void Initialize(Input* input, Player* player, Field* field);

	// 更新処理
	void Update();

	void StartGameEffect();  // ゲーム開始演出を開始

	void StopShake(); // カメラの揺れを止める

public: /// ---------- ゲッター ---------- ///

	// ゲーム開始フラグを取得
	bool GetIsGameStart() { return isGameStart_; }

	// カメラの揺れフラグを取得
	bool GetIsCameraShaking() { return isCameraShaking_; }

public: /// ---------- セッター ---------- ///

	// カメラを設定
	void SetDynamicCamera(DynamicCamera* dynamicCamera) { dynamicCamera_ = dynamicCamera; }

	// ゲーム開始フラグを設定
	void SetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; }

	// カメラの揺れフラグを設定
	void SetIsCameraShaking(bool isCameraShaking) { isCameraShaking_ = isCameraShaking; }

private: /// ---------- メンバ関数 ---------- ///
	// コンストラクタ・デストラクタをprivateに
	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;

	
	// カメラの揺れ
	void CameraShake();
	
	// ゲーム開始演出
	void GameStart();

private: /// ---------- メンバ変数 ---------- ///

	//Camera* camera_ = nullptr; // カメラ
	Input* input_ = nullptr;   // 入力
	Player* player_ = nullptr; // プレイヤー
	Field* field_ = nullptr;   // フィールド
	DynamicCamera* dynamicCamera_ = nullptr; // ダイナミックカメラ

	// カメラ揺れ用の変数
	bool isCameraShaking_ = false;
	float shakeElapsedTime_ = 0.0f;
	float shakeDuration_ = 0.4f;
	float shakeMagnitude_ = 1.0f;
	Vector3 cameraPosition_;

	// ゲーム開始演出用の変数
	bool isGameStart_ = false;
	bool isStartEasing_ = false;
	bool isPlayerPositionSet_ = false;
	float startTimer_ = 0.0f;
	float maxStartT_ = 40.0f;
	float playerStartTimer_ = 0.0f;
	float maxPlayerStartT_ = 30.0f;

	Vector3 defaultFieldScale_ = { 5.0f, 5.0f, 5.0f };

	bool isGameStartEffectEnabled_ = true;

	// 初期のフィールド
	const Vector3 startFieldScale_ = { 0.0f,0.0f,0.0f };
	const Vector3 startFieldPosition_ = { 0.0f,0.0f,0.0f };

	// 拡大するフィールド
	Vector3 fieldScale_ = startFieldScale_;
	Vector3 fieldPosition_ = startFieldPosition_;
};

