#pragma once
#include "Object3D.h"
#include "Collider.h"
#include "ContactRecord.h"
#include"ParticleEmitter.h"
#include"ParticleManager.h"


/// ---------- 前方宣言 ---------- ///
class Player;
class TutorialPlayer;
class Enemy;
class TutorialEnemy;

/// -------------------------------------------------------------
///						　プレイヤークラス
/// -------------------------------------------------------------
class Weapon : public Collider
{
public: /// ---------- メンバ関数 ---------- ///

	// コンストラクト
	Weapon();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();
	
	// 描画処理
	void Draw();
	
	// ImGui描画処理
	void DrawImGui();
	
	// 攻撃処理
	void Attack();

	// 衝突判定
	void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

	///============================
	/// Getter & Setter
	///
public:
	
	// 攻撃のフラグ
	void SetIsAttack(bool isAttack) { isAttack_ = isAttack; }
	bool GetIsAttack() const { return isAttack_; }
	// カメラの設定
	void SetCamera(Camera* camera) { object3D_->SetCamera(camera); }

	// プレイヤーの設定
	void SetPlayer(Player* player) { player_ = player; }
	void SetTutorialPlayer(TutorialPlayer* player) { tutorialPlayer_ = player; }

	// エネミーの設定
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	void SetTutorialEnemy(TutorialEnemy* enemy) { tutorialEnemy_ = enemy; }

private: /// ---------- メンバ変数 ---------- ///

	WorldTransform worldTransform_;
	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;
	
	// プレイヤー
	Player* player_ = nullptr;
	TutorialPlayer* tutorialPlayer_ = nullptr;

	// エネミー
	Enemy* enemy_ = nullptr;
	TutorialEnemy* tutorialEnemy_ = nullptr;

	bool isEnemyHit_ = false; // 敵に当たったかのフラグ

	// 接触記録
	ContactRecord contactRecord_;

	// 位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	
	//半径
	float radius_ = 0.5f;
	// プレイヤーと武器の距離
	float distance_ = 2.0f * 2.0f;

	bool isAttack_ = false;

	/*------攻撃の持続時間------*/
	float attackTime_ = 0.0f;

	float attackMaxTime_ = 60.0f;

	/*-------攻撃中の回転角度------*/
	float attackRotationAngle_ = 0.0f;

	/*------回転速度------*/
	float rotationSpeed_ = 5.0f; // デフォルトの回転速度

	// シリアルナンバー
	uint32_t serialNumber_ = 0;
	// 次のシリアルナンバー
	static inline uint32_t nextSerialNumber_ = 0;
};
