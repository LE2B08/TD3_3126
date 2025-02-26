#pragma once
#include <Object3D.h>
class Player {
public:
	void Initialize();
	void Update();
	void Draw();
	void Finalize();
	void DrawImGui();

	void Move();

private:
	///============================
	///メンバ変数
	/// 
	
	// オブジェクト3D
	std::unique_ptr<Object3D> object3D_ = nullptr;
	//位置
	Vector3 position_;
	// 回転
	Vector3 rotation_;
	// スケール
	Vector3 scale_;
	// 速度
	Vector3 velocity_;
	// 加速度
	Vector3 acceleration_;
	// 角速度
	Vector3 angularVelocity_;
};
