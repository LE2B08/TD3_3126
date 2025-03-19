#pragma once
#include "Vector3.h"
#include <list>
#include <memory>

class EnemyBullet;

/// === 攻撃コマンド基底クラス === ///
class AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 仮想デストラクタ
	virtual ~AttackCommand() = default;

	// 攻撃
	virtual void Attack(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) = 0;

	// 終了フラグのゲッター
	bool GetIsAttackEnd() const { return isAttackEnd_; }
	
	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
protected:

	// 終了フラグ
	bool isAttackEnd_ = false;
};

/// === 攻撃コマンド === ///
class ShotCommand : public AttackCommand {

	///-------------------------------------------/// 
	/// メンバ関数
	///-------------------------------------------///
public:

	// 攻撃
	void Attack(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) override;

	///-------------------------------------------/// 
	/// メンバ変数
	///-------------------------------------------///
private:

	// 弾の発射間隔
	uint32_t attackInterval_ = 120;

	// 弾の発射回数
	uint32_t attackCount_ = 0;
};