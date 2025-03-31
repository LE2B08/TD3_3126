#pragma once
#include <Sprite.h>
#include "Hook.h"

/// ===== コントローラー用UI ===== ///
class ControllerUI {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// コンストラクタ
	ControllerUI();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// ImGui描画
	void DrawImGui();

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// Rボタン
	std::unique_ptr<Sprite> RButton_;

	// Rボタン位置
	Vector2 RButtonPosition_;

	// Lボタン
	std::unique_ptr<Sprite> LButton_;

	// Lボタン位置
	Vector2 LButtonPosition_;

	// Rトリガー
	std::unique_ptr<Sprite> RTrigger_;

	// Rトリガー位置
	Vector2 RTriggerPosition_;

	// Rスティック
	std::unique_ptr<Sprite> RStick_;

	// Rスティック位置
	Vector2 RStickPosition_;

	// Aボタン
	std::unique_ptr<Sprite> AButton_;

	// Aボタン位置
	Vector2 AButtonPosition_;

	// 投げる
	std::unique_ptr<Sprite> throw_;

	// 投げる位置
	Vector2 throwPosition_;

	// 戻す
	std::unique_ptr<Sprite> back_;

	// 戻す位置
	Vector2 backPosition_;

	// 巻く
	std::unique_ptr<Sprite> wind_;

	// 巻く位置
	Vector2 windPosition_;

	// 攻撃
	std::unique_ptr<Sprite> attack_;

	// 攻撃位置
	Vector2 attackPosition_;

	// 向き
	std::unique_ptr<Sprite> direction_;

	// 向き位置
	Vector2 directionPosition_;

	// ゲームスタート
	std::unique_ptr<Sprite> gameStart_;

	// ゲームスタート位置
	Vector2 gameStartPosition_;

	// フック借りポインタ
	Hook* hook_ = nullptr;
};

