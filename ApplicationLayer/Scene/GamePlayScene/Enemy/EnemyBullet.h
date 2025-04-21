#pragma once
#include "Collider.h"
#include "Object3D.h"


/// -------------------------------------------------------------
///					　エネミーバレットクラス
/// -------------------------------------------------------------
class EnemyBullet : public Collider
{
public: /// ---------- メンバ関数 ---------- ///

	/// コンストラクタ
	EnemyBullet();

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// 移動
	void Move();

	/// 衝突判定
	void OnCollision(Collider* other) override;

	/// 中心座標を取得する純粋仮想関数
	Vector3 GetCenterPosition() const override;

public: /// ---------- ゲッター ---------- ///

	/// <summary>
	/// 位置のゲッター
	/// </summary>
	/// <returns></returns>
	Vector3 GetPosition() const { return worldTransform_.translate_; }

public: /// ---------- セッター ---------- ///

	/// <summary>
	/// 位置のセッター
	/// </summary>
	/// <param name="position"></param>
	void SetPosition(const Vector3& position) { worldTransform_.translate_ = position; }

	/// <summary>
	/// 向きのセッター
	/// </summary>
	/// <param name="direction"></param>
	void SetDirection(const Vector3& direction) { direction_ = direction; }

	/// <summary>
	/// 生存フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsAlive() const { return isAlive_; }

	void SetIsMove(bool isMove) { isMove_ = isMove; }

private: /// ---------- メンバ変数 ---------- ///

	// ワールド変換
	WorldTransform worldTransform_;

	// 弾のオブジェクト
	std::unique_ptr<Object3D> objectBullet_;

	// 速度
	Vector3 velocity_ = {};

	// 向き
	Vector3 direction_ = {};

	// 生存フラグ
	bool isAlive_ = true;

	// 移動フラグ
	bool isMove_ = false;

	// 生存フレーム
	uint32_t lifeFrame_ = 360;

	// 速度
	const float speed_ = 0.2f;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;
	// 次のシリアルナンバー
	uint32_t nextSerialNumber_ = 0;
};

