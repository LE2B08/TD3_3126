#include "AttackCommand.h"
#include "EnemyBullet.h"

void ShotCommand::Attack(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) {

	// 射撃
	if (attackInterval_ <= 0 && attackInterval_ < 5) {

		// 弾を生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

		// 弾の初期化
		bullet->Initialize();

		// 弾の位置を設定
		bullet->SetPosition(position);

		// 弾に向きを設定
		bullet->SetDirection(direction);

		// 弾をリストに追加
		bullets.push_back(std::move(bullet));

		// 攻撃回数をカウント
		attackCount_++;

		// 攻撃間隔をリセット
		attackInterval_ = 300;

	}
	else {

		// 攻撃間隔を減らす
		attackInterval_--;
	}

	// 5回攻撃したら
	if (attackCount_ >= 5) {
		// 攻撃終了
		isAttackEnd_ = true;
	}
}
