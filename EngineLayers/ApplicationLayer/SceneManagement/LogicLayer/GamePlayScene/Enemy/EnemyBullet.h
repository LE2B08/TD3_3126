#pragma once
#include "Colliders/Collider.h"
#include "Object3D/Object3D.h"

class EnemyBullet : public Collider {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	/// コンストラクタ
	EnemyBullet();

	/// デストラクタ
	~EnemyBullet();

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

	///-------------------------------------------/// 
	/// getter&setter
	///-------------------------------------------///
public:

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
	/// フラグのゲッター
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// ワールド変換
	WorldTransform worldTransform_;

	// 弾のオブジェクト
	std::unique_ptr<Object3D> objectBullet_;

	// 速度
	Vector3 velocity_ = {};

	// 向き
	Vector3 direction_ = {};
	
	// 生存フラグ
	bool isDead_ = false;
};

