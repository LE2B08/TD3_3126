#pragma once
#include "Sprite.h"
class TutorialUI {

public: /// ---------- メンバ関数 ---------- ///
	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void StartDraw();
	void RotationDraw();
	void ThrowDraw();
	void ArcDraw();
	void MoveDraw();
	void AttackDraw();
	void EndDraw();

	// ImGui描画処理
	void DrawImGui();

public: /// ---------- ゲッター ---------- ///
	
	// スプライトの位置の取得
	Vector2 GetStartUIPos() { return startUIPos_; }
	Vector2 GetRotationUIPos() { return rotationUIPos_; }
	Vector2 GetThrowUIPos() { return throwUIPos_; }
	Vector2 GetArcUIPos() { return arcUIPos_; }
	Vector2 GetMoveUIPos() { return moveUIPos_; }
	Vector2 GetAttackUIPos() { return attackUIPos_; }
	Vector2 GetEndUIPos() { return endUIPos_; }

public: /// ---------- セッター ---------- ///
	// スプライトの位置の設定
	void SetStartUIPos(Vector2 pos) { startUIPos_ = pos; }
	void SetRotationUIPos(Vector2 pos) { rotationUIPos_ = pos; }
	void SetThrowUIPos(Vector2 pos) { throwUIPos_ = pos; }
	void SetArcUIPos(Vector2 pos) { arcUIPos_ = pos; }
	void SetMoveUIPos(Vector2 pos) { moveUIPos_ = pos; }
	void SetAttackUIPos(Vector2 pos) { attackUIPos_ = pos; }
	void SetEndUIPos(Vector2 pos) { endUIPos_ = pos; }


private: /// ---------- メンバ変数 ---------- ///
	// Start
	std::unique_ptr<Sprite> startUI_ = nullptr;
	Vector2 startUIPos_ = {0.0f, 0.0f};

	// Rotation
	std::unique_ptr<Sprite> rotationUI_ = nullptr;
	Vector2 rotationUIPos_ = {0.0f, 0.0f};

	// Throw
	std::unique_ptr<Sprite> throwUI_ = nullptr;
	Vector2 throwUIPos_ = {0.0f, 0.0f};

	// Arc
	std::unique_ptr<Sprite> arcUI_ = nullptr;
	Vector2 arcUIPos_ = {0.0f, 0.0f};
	// Move
	std::unique_ptr<Sprite> moveUI_ = nullptr;
	Vector2 moveUIPos_ = {0.0f, 0.0f};
	// Attack
	std::unique_ptr<Sprite> attackUI_ = nullptr;
	Vector2 attackUIPos_ = {0.0f, 0.0f};
	// End
	std::unique_ptr<Sprite> endUI_ = nullptr;
	Vector2 endUIPos_ = {0.0f, 0.0f};
};
