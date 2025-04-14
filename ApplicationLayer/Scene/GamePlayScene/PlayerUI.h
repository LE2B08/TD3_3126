#pragma once
#include <Object3D.h>
#include <ParticleEmitter.h>
#include <ParticleManager.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Wireframe.h>

/// ---------- 前方宣言 ---------- ///
class Player;

/// -------------------------------------------------------------
///						　プレイヤークラス
/// -------------------------------------------------------------
class PlayerUI
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// ImGuiの描画処理
	void DrawImGui();

	
public: /// ---------- ゲッター ---------- ///

	Vector2 GetScale() const { return scale_; }
	Vector2 GetRotation() const { return rotation_; }
	Vector2 GetPosition() const { return position_; }

	float GetSpeed() const { return speed_; }
	float GetMaxSpeed() const { return maxSpeed_; }
	float GetMinSpeed() const { return minSpeed_; }

public:	/// ---------- セッター ---------- ///

	void SetScale(const Vector2& scale) { scale_ = scale; }
	void SetRotation(const Vector2& rotation) { rotation_ = rotation; }
	void SetPosition(const Vector2& position) { position_ = position; }

	void SetSpeed(const float& speed) { speed_ = speed; }
	void SetMaxSpeed(const float& maxSpeed) { maxSpeed_ = maxSpeed; }
	void SetMinSpeed(const float& minSpeed) { minSpeed_ = minSpeed; }

	// プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

private: /// ---------- メンバ変数 ---------- ///

	// プレイヤー
	Player* player_ = nullptr;


	// 全体の位置
	Vector2 position_ = { 0.0f,0.0f };
	// 全体のスケール
	Vector2 scale_ = { 1.0f,1.0f };
	// 全体の回転
	Vector2 rotation_ = { 0.0f,0.0f };

	// UIの位置
	Vector2 hpPosition_ = { 100.0f, 320.0f };
	// UIのスケール
	Vector2 hpScale_ = { 1.0f,1.0f };
	// UIの回転
	float hpRotation_ = 0.0f;

	// HPの色
	Vector4 hpGaugeColor = {};

	// Hpの枠のスプライト
	std::unique_ptr<Sprite> hpSprite_;

	// Hpのゲージのスプライト
	std::unique_ptr<Sprite> hpGaugeSprite_;

	//Hp
	int hp_ = 10;

	// 速度
	float speed_ = 1.0f;
	// 速度の最大値
	float maxSpeed_ = 10.0f;
	// 速度の最小値
	float minSpeed_ = 0.0f;
};
