#pragma once
#include "Object3D.h"
#include "TextureManager.h"
#include"Camera.h"

class Skydome {

	public: /// ---------- メンバ関数 ---------- ///
	// 初期化処理
	void Initialize();
	// 更新処理
	void Update();
	// 描画処理
	void Draw();
	// ImGuiの描画処理
	void DrawImGui();

	public: /// ---------- ゲッター ---------- ///

		Vector3 GetScale() const { return scale_; }
	    Vector3 GetRotation() const { return rotation_; }
	    Vector3 GetPosition() const { return position_; }


		public: /// ---------- セッター ---------- ///
	    void SetScale(const Vector3& scale) { scale_ = scale; }
	        void SetRotation(const Vector3& rotation) { rotation_ = rotation; }
	    void SetPosition(const Vector3& position) { position_ = position; }
	        // カメラをセット
	    void SetCamera(Camera* camera) { camera_ = camera; }

		private: /// ---------- メンバ変数 ---------- ///

			// 全体の位置
	        Vector3 position_ = {0.0f, 0.0f, 0.0f};
	        // 全体のスケール
	        Vector3 scale_ = {1.0f, 1.0f, 1.0f};
	        // 全体の回転
	        Vector3 rotation_ = {0.0f, 0.0f, 0.0f};

			// スカイドームのオブジェクト
	        std::unique_ptr<Object3D> object3D_ = nullptr;

			// カメラ
	        Camera* camera_ = nullptr;
};
