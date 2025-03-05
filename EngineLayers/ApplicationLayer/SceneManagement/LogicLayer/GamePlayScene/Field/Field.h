#pragma once
#include "Object3D/Object3D.h"

/// === 地面 === ///
class Field {

///-------------------------------------------/// 
/// メンバ関数
///-------------------------------------------///
public:

	/// 初期化
	void Initialize();

	/// 更新
	void Update();

	/// 描画
	void Draw();

	/// ImGui描画
	void ShowImGui(const char* name);

///-------------------------------------------/// 
/// ゲッター
///-------------------------------------------///
public:

	Vector3 GetMinPosition() { return minPosition_; }

	Vector3 GetMaxPosition() { return maxPosition_; }

///-------------------------------------------/// 
/// メンバ変数
///-------------------------------------------///
private:

	std::unique_ptr<Object3D> objectField_ = nullptr;

	// スケール
	Vector3 scale_;

	// 回転
	Vector3 rotate_;

	// 位置
	Vector3 position_;

	Vector3 minPosition_;
	Vector3 maxPosition_;
};

