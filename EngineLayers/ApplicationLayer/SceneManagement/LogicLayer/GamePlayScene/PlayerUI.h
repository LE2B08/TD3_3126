#pragma once
#include <Object3D.h>
#include <ParticleEmitter.h>
#include <ParticleManager.h>
#include <Sprite.h>
#include <TextureManager.h>
#include <Wireframe.h>

class PlayerUI {

public:
	///-------------------------------------------///
	/// メンバ関数
	/// 

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void DrawImGui();

public:
	///-------------------------------------------///
	/// Getter & Setter
	///

	// 位置
	void SetPosition(const Vector2& position) { position_ = position; }
	Vector2 GetPosition() const { return position_; }

	// 回転
	void SetScale(const Vector2& scale) { scale_ = scale; }
	Vector2 GetScale() const { return scale_; }

	// スケール
	void SetRotation(const Vector2& rotation) { rotation_ = rotation; }
	Vector2 GetRotation() const { return rotation_; }

	//-----------------Hp-----------------//

	// Hp
	void SetHp(const int& hp) { hp_ = hp; }
	int GetHp() const { return hp_; }

	// 最大HP
	void SetMaxHp(const int& maxHp) { maxHp_ = maxHp; }
	int GetMaxHp() const { return maxHp_; }

	// 最小HP
	void SetMinHp(const int& minHp) { minHp_ = minHp; }
	int GetMinHp() const { return minHp_; }

	//-----------------Speed-----------------//

	// 速度
	void SetSpeed(const float& speed) { speed_ = speed; }
	float GetSpeed() const { return speed_; }

	// 最大速度
	void SetMaxSpeed(const float& maxSpeed) { maxSpeed_ = maxSpeed; }
	float GetMaxSpeed() const { return maxSpeed_; }

	// 最小速度
	void SetMinSpeed(const float& minSpeed) { minSpeed_ = minSpeed; }
	float GetMinSpeed() const { return minSpeed_; }


private:
	///-------------------------------------------///
	/// メンバ変数
	/// 

	// 全体の位置
	Vector2 position_;
	// 全体のスケール
	Vector2 scale_;
	// 全体の回転
	Vector2 rotation_;

	//-------Hp-------//

	// プレイヤーのHP
	int hp_;
	// HPの最大値
	int maxHp_;
	// HPの最小値
	int minHp_;

	// UIの位置
	Vector2 hpPosition_;
	// UIのスケール
	Vector2 hpScale_;
	// UIの回転
	float hpRotation_;

	// Hpの枠のスプライト
	std::unique_ptr<Sprite> hpSprite_;

	// Hpのゲージのスプライト
	std::unique_ptr<Sprite> hpGaugeSprite_;

	//-------Speed-------//

	// 速度
	float speed_;
	// 速度の最大値
	float maxSpeed_;
	// 速度の最小値
	float minSpeed_;

	Vector2 speedPosition_;
	Vector2 speedScale_;
	float speedRotation_;

	// SpeedSprite
	std::unique_ptr<Sprite> speedSprite_;
};
