#pragma once
#include <Object3D.h>
#include "WorldTransform.h"
#include "Collider.h"

#include <memory>
#include <numbers>


/// ---------- 前方宣言 ---------- ///
class Input;
class Camera;


/// -------------------------------------------------------------
///					　キャラクターの基底クラス
/// -------------------------------------------------------------
class BaseCharacter : public Collider
{
public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	virtual void Initialize();

	// 更新処理WorldTransform
	virtual void Update();

	// 描画処理
	virtual void Draw();

	// 衝突判定
	virtual void OnCollision(Collider* other) override;

	// 中心座標を取得する純粋仮想関数
	virtual Vector3 GetCenterPosition() const override;

public: /// ---------- ゲッター ---------- ///

	// ヒットポイントを取得
	int GetHp() const { return hp_; }

	// スケールを取得
	Vector3 GetScale() const { return worldTransform_.scale_; }

	// 回転を取得
	Vector3 GetRotation() const { return  worldTransform_.rotate_; }

	// 座標を取得
	Vector3 GetPosition() { return  worldTransform_.translate_; }

	// シリアルナンバーを取得
	uint32_t GetSerialNumber() const { return serialNumber_; }

	

public: /// ---------- セッター ---------- ///

	// ヒットポイントを設定
	void SetHp(int hp) { hp_ = hp; }

	// スケールを設定
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }

	// 回転を設定
	void SetRotation(const Vector3& rotation) { worldTransform_.rotate_ = rotation; }

	// 座標を設定
	void SetPosition(const Vector3& position) { worldTransform_.translate_ = position; }

	// シリアルナンバーを設定
	void SetSerialNumber(uint32_t serialNumber) { serialNumber_ = serialNumber; }

protected: /// ---------- メンバ変数 ---------- ///

	// オブジェクト
	std::unique_ptr<Object3D> object3D_;

	// ワールド変換
	WorldTransform worldTransform_;

	// 入力
	Input* input_ = nullptr;

	// カメラ
	Camera* camera_ = nullptr;

	// ヒットポイント
	int hp_ = 10;

	uint32_t serialNumber_ = 0; // シリアルナンバー
	static inline uint32_t nextSerialNumber_ = 0; // 次のシリアルナンバー
};

