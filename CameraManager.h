#pragma once
#include <Camera.h>
#include <map>
#include <memory>
// カメラを管理するクラス
class CameraManager
{
	/*--------------メンバ関数---------------*/
public:
	// シングルインスタンス
	static CameraManager* GetInstance();

	// 終了処理
	void Finalize();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// ImGui描画
	void DrawImGui();

	// カメラの生成
	void AddCamera(const std::string &name);
	/*--------------取得---------------*/
public:
	// カメラの取得
	Camera* GetCamera(const std::string& name) const;

	/*--------------設定---------------*/
	void SetCurrentCamera(const std::string& name);

	/*--------現在のカメラを取得---------*/
	Camera* GetCurrentCamera() const;
private:
	/*------メンバ変数------*/
	
	//　コンストラクタ
	CameraManager() = default;
	
	// カメラのコンテナ
	std::map<std::string, std::unique_ptr<Camera>> cameras_;

	//現在のカメラ名
	std::string currentCameraName_;

	//過去のカメラ名
	std::string previousCameraName_;
};

