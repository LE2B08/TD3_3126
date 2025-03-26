#pragma once
#include "Vector3.h"
#include <list>
#include <memory>
#include <random>

class EnemyBullet;

/// === 攻撃コマンド基底クラス === ///
class AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 仮想デストラクタ
	virtual ~AttackCommand() = default;

	// 初期化
	virtual void Initialize() = 0;

	// 更新
	virtual void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) = 0;

	// 終了フラグのゲッター
	bool GetIsEnd() const { return isEnd_; }

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	// 終了フラグ
	bool isEnd_ = false;

	// タイマー(秒)
	float timer_ = 0.0f;

	// タイマーのリセット値(秒)
	float intervalReset_ = 0.0f;

	// 攻撃回数
	uint32_t attackCount_ = 0;

	// Δt
	const float kDeltaTime = 1.0f / 60.0f;
};

/// === 攻撃コマンド === ///
class ShotCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:
};

/// === 扇形の射撃コマンド === ///
class FanShotCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:
};

/// === 回転してる風射撃コマンド === ///
class RotateShotCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:
};

/// === 呼び戻しコマンド === ///
class RecallCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	// ランダムに位置を決める
	Vector3 RandomPosition(const Vector3& position, float maxRadius);

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// 位置
	Vector3 position_ = {};

	// 弾が出る最大半径
	float maxRadius_ = 0.0f;

	// 乱数生成器
	std::random_device seedGenerator;
	std::mt19937 randomEngine;
};

class SpreadCenterShotCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	///-------------------------------------------///
	/// メンバ変数
	///-------------------------------------------///
private:

	// 1回の攻撃で発射する弾数
	uint32_t bulletCount_ = 0;
};
