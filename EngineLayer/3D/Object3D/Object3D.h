#pragma once
#include "DX12Include.h"
#include "WorldTransform.h"
#include "TextureManager.h"
#include "Material.h"
#include "VertexData.h"
#include "ModelData.h"
#include "Camera.h"
#include "LightManager.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <numbers>

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;
class Model;
class Object3DCommon;

/// -------------------------------------------------------------
///						オブジェクト3Dクラス
/// -------------------------------------------------------------
class Object3D
{
public: /// ---------- 構造体 ---------- ///

	// シェーダー側のカメラ構造体
	struct CameraForGPU
	{
		Vector3 worldPosition;
	};

public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize(const std::string& fileName);

	// 更新処理
	void Update();

	// ImGui
	void DrawImGui();

	// 描画処理
	void Draw();

public: /// ---------- 設定処理 ---------- ///

	// モデルの追加
	void SetModel(const std::string& filePath);

	// スケールを設定
	void SetScale(const Vector3& scale) { worldTransform.scale_ = scale; }

	// 回転を設定
	void SetRotate(const Vector3& rotate) { worldTransform.rotate_ = rotate; }

	// 位置を設定
	void SetTranslate(const Vector3& translate) { worldTransform.translate_ = translate; }

	// 色の設定
	void SetColor(const Vector4& color) { color_ = color; }

	// アルファ値の設定
	void SetAlpha(float alpha) { alpha_ = alpha; }

	// ライティングを有効にするかどうかを設定
	void SetEnableLighting(bool enableLighting) { enableLighting_ = enableLighting; }

	// カメラの設定
	void SetCamera(Camera* camera) { camera_ = camera; }

public: /// ---------- ゲッタ ---------- ///

	// α値の取得
	float GetAlpha() const { return alpha_; }

private: /// ---------- メンバ変数 ---------- ///

	// 頂点データの初期化処理
	void InitializeMaterial();

	// 頂点バッファデータの初期化
	void InitializeVertexBufferData();

	// カメラ用のリソース生成
	void InitializeCameraResource();

private: /// ---------- メンバ変数 ---------- ///

	// 分割数
	uint32_t kSubdivision = 32;

	// 緯度・経度の分割数に応じた角度の計算
	float kLatEvery = std::numbers::pi_v<float> / float(kSubdivision);
	float kLonEvery = 2.0f * std::numbers::pi_v<float> / float(kSubdivision);

	// 球体の頂点数の計算
	uint32_t TotalVertexCount = kSubdivision * kSubdivision * 6;

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon = nullptr;
	LightManager lightManager_;
	Camera* camera_ = nullptr;

	std::shared_ptr<Model> model_;

	// ワールドトランスフォーム
	WorldTransform worldTransform;

	// 色
	Vector4 color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	float alpha_ = 1.0f; // アルファ値

	// ライティングを有効にするかどうか
	bool enableLighting_ = false;

	// バッファリソースの作成
	ComPtr <ID3D12Resource> vertexResource;
	ComPtr <ID3D12Resource> materialResource;
	ComPtr <ID3D12Resource> cameraResource;

	// カメラにデータを書き込む
	CameraForGPU* cameraData = nullptr;

	// OBJファイルのデータ
	ModelData modelData;

	// 頂点リソース内のデータを指すポインタ
	VertexData* vertexData = nullptr;

	// バッファリソースの使い道を補足するバッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
};
