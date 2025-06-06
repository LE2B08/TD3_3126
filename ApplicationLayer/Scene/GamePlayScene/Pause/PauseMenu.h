#pragma once
#include <Sprite.h>

/// === 前方宣言 === ///
class Input;
class SceneManager;

// メニューの状態
enum class MenuState {
	ReturnToGame,
	HowToPlay,
	CameraShakeOn,
	ReturnToTitle,
};

/// === ポーズメニュー === ///
class PauseMenu {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// コンストラクタ
	PauseMenu();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// imGui描画
	void ShowImGui();

	///-------------------------------------------/// 
	/// ゲッター
	///-------------------------------------------///
public:

	MenuState GetMenuState() const { return menuState_; }

	bool IsCameraShakeOn() const {return isCameraShakeOn_;}

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// 入力
	Input* input_ = nullptr;

	// 黒のフィルター
	std::unique_ptr<Sprite> blackFilter_ = nullptr;

	// ポーズテキスト
	std::unique_ptr<Sprite> pauseText_ = nullptr;

	// ポーズテキストの位置
	Vector2 pauseTextPos_ = { 0.0f, 0.0f };

	// ゲームに戻るテキスト
	std::unique_ptr<Sprite> returnToGameText_ = nullptr;

	// ゲームに戻るテキストの位置
	Vector2 returnToGameTextPos_ = { 0.0f, 0.0f };

	// 操作方法テキスト
	std::unique_ptr<Sprite> howToPlayText_ = nullptr;

	// 操作方法テキストの位置
	Vector2 howToPlayTextPos_ = { 0.0f, 0.0f };

	// タイトルに戻るテキスト
	std::unique_ptr<Sprite> returnToTitleText_ = nullptr;

	// タイトルに戻るテキストの位置
	Vector2 returnToTitleTextPos_ = { 0.0f, 0.0f };

	// 決定ボタン
	std::unique_ptr<Sprite> decisionButton_ = nullptr;

	// 決定ボタンの位置
	Vector2 decisionButtonPos_ = { 0.0f, 0.0f };

	// 選択している矢印
	std::unique_ptr<Sprite> selectionArrow_ = nullptr;

	// 選択している矢印の位置
	Vector2 selectionArrowPos_ = { 0.0f, 0.0f };

	// 操作方法
	std::unique_ptr<Sprite> HowToPlay = nullptr;

	// 操作方法の位置
	Vector2 HowToPlayPos_ = { 0.0f, 0.0f };

	// カメラの揺れ : ON
	std::unique_ptr<Sprite> cameraShakeOn_ = nullptr;

	// カメラの揺れの位置
	Vector2 cameraShakeOnTextPos_ = { 0.0f, 0.0f };

	// カメラの揺れ : OFF
	std::unique_ptr<Sprite> cameraShakeOff_ = nullptr;

	// カメラの揺れの位置
	Vector2 cameraShakeOffTextPos_ = { 0.0f, 0.0f };

	// 選択しているメニューの状態
	MenuState menuState_ = MenuState::ReturnToGame;

	// 操作方法の表示フラグ
	bool isHowToPlay_ = false;

	// カメラ揺れON/OFFフラグ
	bool isCameraShakeOn_ = true;

	SceneManager* sceneManager_ = nullptr;
};