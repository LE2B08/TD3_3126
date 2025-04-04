#pragma once
#include "Vector3.h"


/// ---------- 前方宣言 ---------- ///
class Camera;
class Input;
class Player;
class Field;


/// -------------------------------------------------------------
///					　演出を管理するクラス
/// -------------------------------------------------------------
class EffectManager
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize(Camera* camera, Input* input, Player* player, Field* field);

	// 更新処理
	void Update();

	void StartGameEffect();  // ゲーム開始演出を開始

public: /// ---------- ゲッター ---------- ///

	// ゲーム開始フラグを取得
	bool GetIsGameStart() { return isGameStart_; }

	// カメラの揺れフラグを取得
	bool GetIsCameraShaking() { return isCameraShaking_; }

public: /// ---------- セッター ---------- ///

	// ゲーム開始フラグを設定
	void SetIsGameStart(bool isGameStart) { isGameStart_ = isGameStart; }

	// カメラの揺れフラグを設定
	void SetIsCameraShaking(bool isCameraShaking) { isCameraShaking_ = isCameraShaking; }

private: /// ---------- メンバ関数 ---------- ///

	// カメラの揺れ
	void CameraShake();

	// ゲーム開始演出
	void GameStart();

private: /// ---------- メンバ変数 ---------- ///

	Camera* camera_ = nullptr; // カメラ
	Input* input_ = nullptr;   // 入力
	Player* player_ = nullptr; // プレイヤー
	Field* field_ = nullptr;   // フィールド

	// カメラ揺れ用の変数
	bool isCameraShaking_ = false; // カメラが揺れているかどうか
	float shakeElapsedTime_ = 0.0f;	// 揺れの経過時間
	float shakeDuration_ = 0.5f; // 揺れの持続時間
	float shakeMagnitude_ = 1.0f; // 揺れの大きさ
	Vector3 cameraPosition_;

	// ゲーム開始演出用の変数
	bool isGameStart_ = false; // ゲーム開始フラグ
	bool isStartEasing_ = false; // イージングを開始したかどうか
	bool isPlayerPositionSet_ = false; // プレイヤーの位置を設定したかどうか
	float startTimer_ = 0.0f; // 開始時間
	float maxStartT_ = 40.0f; // 最大開始時間
	float playerStartTimer_ = 0.0f; // プレイヤーの開始時間
	float maxPlayerStartT_ = 30.0f; // 最大プレイヤー開始時間

	Vector3 defaultFieldScale_ = { 5.0f, 5.0f, 5.0f }; // デフォルトのフィールドスケール

	bool isGameStartEffectEnabled_ = true; // ゲーム開始演出を有効にするかどうか

	// 初期のフィールド
	const Vector3 startFieldScale_ = { 0.0f,0.0f,0.0f };
	const Vector3 startFieldPosition_ = { 0.0f,0.0f,0.0f };

	// 拡大するフィールド
	Vector3 fieldScale_ = startFieldScale_;
	Vector3 fieldPosition_ = startFieldPosition_;
};

