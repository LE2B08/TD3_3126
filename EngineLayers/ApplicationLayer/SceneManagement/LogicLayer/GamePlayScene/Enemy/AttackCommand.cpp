#include "AttackCommand.h"
#include "EnemyBullet.h"
#include <numbers>

void ShotCommand::Initialize() {

	// 初回は２秒後に攻撃
	timer_ = 2.0f;

	// ２回目以降は１秒後に攻撃
	intervalReset_ = 1.0f;

	// 攻撃回数を設定
	attackCount_ = 5;
}

void ShotCommand::Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) {

	// タイマーが0になったら
	if (timer_ <= 0) {

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

		// 攻撃回数を1減らす
		attackCount_--;

		// タイマーをリセット
		timer_ = intervalReset_;

	}
	else {

		// タイマーを減らす
		timer_ -= kDeltaTime;
	}

	// 攻撃回数が0になったら
	if (attackCount_ <= 0) {

		// 攻撃終了
		isEnd_ = true;
	}
}

void FanShotCommand::Initialize() {

	// 初回は2秒後に攻撃
	timer_ = 2.0f;

	// ２回目以降は2秒後に攻撃
	intervalReset_ = 2.0f;

	// 攻撃回数を設定
	attackCount_ = 3;
}

void FanShotCommand::Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) {

	// タイマーが0になったら
	if (timer_ <= 0) {

		// 扇形の弾を5発生成
		for (int32_t i = -2; i < 3; i++) {

			// 弾を生成
			std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

			// 弾の初期化
			bullet->Initialize();

			// 弾の位置を設定
			bullet->SetPosition(position);

			// 向きをもらう
			Vector3 dir = { direction.x, direction.y, direction.z };

			// 向きをY軸回転でずらす
			float angle = i * 0.2f;
			dir.x = direction.x * cos(angle) - direction.z * sin(angle);
			dir.z = direction.x * sin(angle) + direction.z * cos(angle);

			// 向きをセットする
			bullet->SetDirection(dir);

			// 弾をリストに追加
			bullets.push_back(std::move(bullet));
		}

		// 攻撃回数を1減らす
		attackCount_--;

		// タイマーをリセット
		timer_ = intervalReset_;
	}
	else {

		// タイマーを減らす
		timer_ -= kDeltaTime;
	}

	// 攻撃回数が0になったら
	if (attackCount_ <= 0) {

		// 攻撃終了
		isEnd_ = true;
	}
}

void RotateShotCommand::Initialize() {

	// 初回は4秒後に攻撃
	timer_ = 4.0f;

	// 2回目以降は0.1秒後
	intervalReset_ = 0.1f;

	// 18回攻撃
	attackCount_ = 36;
}

void RotateShotCommand::Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) {

	// タイマーが0になったら
	if (timer_ <= 0) {

		// 弾を生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

		// 弾の初期化
		bullet->Initialize();

		// 弾の位置を設定
		bullet->SetPosition(position);

		// 向きをもらう
		Vector3 dir = { direction.x, direction.y, direction.z };

		// 向きを弾数分、分割する
		float angle = std::numbers::pi_v<float> / 9; // 20°
		angle *= attackCount_; // 20°ずつずれる
		dir.x = direction.x * cos(angle) - direction.z * sin(angle);
		dir.z = direction.x * sin(angle) + direction.z * cos(angle);

		// 向きをセットする
		bullet->SetDirection(dir);

		// 弾をリストに追加
		bullets.push_back(std::move(bullet));

		// 攻撃回数を1減らす
		attackCount_--;

		// タイマーをリセット
		timer_ = intervalReset_;
	}
	else {

		// タイマーを減らす
		timer_ -= kDeltaTime;
	}

	// 攻撃回数が0になったら
	if (attackCount_ <= 0) {

		// 攻撃終了
		isEnd_ = true;
	}
}

void RecallCommand::Initialize() {

	// 初回は2秒後に攻撃
	timer_ = 2.0f;

	// 2回目以降は2秒後に攻撃
	intervalReset_ = 2.0f;

	// 攻撃回数を設定
	attackCount_ = 4;

	randomEngine.seed(seedGenerator());
}

void RecallCommand::Update(const Vector3& position, const Vector3& direction, std::list<std::unique_ptr<EnemyBullet>>& bullets) {

	// タイマーが0になったら
	if (timer_ <= 0) {

		// 弾を生成
		std::unique_ptr<EnemyBullet> bullet = std::make_unique<EnemyBullet>();

		// 弾の初期化
		bullet->Initialize();

		// 位置をランダムに決める
		position_ = RandomPosition(position, -10.0f, 10.0f);

		// 弾の位置を設定
		bullet->SetPosition(position_);

		if (bullet->GetPosition() == position) {

			bullet->SetPosition(position);
		}

		// 向きを発生源に向ける
		Vector3 dir = Vector3::Normalize(position - position_);

		// 向きを設定
		bullet->SetDirection(dir);

		// 弾をリストに追加
		bullets.push_back(std::move(bullet));

		// 攻撃回数を1減らす
		attackCount_--;

		// タイマーをリセット
		timer_ = intervalReset_;
	}
	else {

		// タイマーを減らす
		timer_ -= kDeltaTime;
	}
	
	// 攻撃回数が0になったら
	if (attackCount_ <= 0) {

		// 攻撃終了
		isEnd_ = true;
	}
}


Vector3 RecallCommand::RandomPosition(const Vector3& position, float min, float max) {

	// XZ平面上のランダムな位置を生成
	std::uniform_real_distribution<float> dist(min, max);
	Vector3 randomPosition = { dist(randomEngine), 0.0f, dist(randomEngine) };

	// 発生源を中心にするために引数の位置を加算
	randomPosition += position;

	return randomPosition;
}
