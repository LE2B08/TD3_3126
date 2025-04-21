#pragma once
#include "DX12Include.h"
#include "WorldTransform.h"
#include "Camera.h"

#include "Vector2.h"
#include "Vector4.h"

#include <array>
#include <memory>

/// ---------- 前方宣言 ---------- ///
class DirectXCommon;


/// -------------------------------------------------------------
///					　スカイボックスクラス
/// -------------------------------------------------------------
class SkyBox
{
private: /// ---------- 構造体 ---------- ///

	/// ---------- 頂点数 ( Vertex, Index ) ----------- ///
	static inline const UINT kNumVertex = 36;
	static inline const UINT kNumIndex = 36;

	// マテリアルデータの構造体
	struct Material final
	{
		Vector4 color;
		Matrix4x4 uvTransform;
		float padding[3];
	};

	// 頂点データの構造体
	struct VertexData
	{
		Vector4 position;
		Vector3 texcoord;
	};

	// 座標変換行列データの構造体
	struct TransformationMatrix final
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

public: /// ---------- メンバ関数 ---------- ///

	// 初期化処理
	void Initialize(const std::string& filePath);

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// ImGui描画処理
	void DrawImGui();

public: /// ---------- ゲッター ---------- ///
	
	// デバッグカメラの有無を取得
	bool GetDebugCamera() { return isDebugCamera_; }

public: /// ---------- セッター ---------- ///

	// デバッグカメラの有無
	void SetDebugCamera(bool isDebugCamera) { isDebugCamera_ = isDebugCamera; }
	
	// スケールの設定
	void SetScale(const Vector3& scale) { worldTransform_.scale_ = scale; }
	
	// 回転の設定
	void SetRotate(const Vector3& rotate) { worldTransform_.rotate_ = rotate; }
	
	// 移動の設定
	void SetTranslate(const Vector3& translate) { worldTransform_.translate_ = translate; }
	
	// カラーの設定
	void SetColor(const Vector4& color) { color_ = color; }
	
	// UV変換行列の設定
	void SetUVTransform(const Matrix4x4& uvTransform) { uvTransform_ = uvTransform; }

private: /// ---------- メンバ関数 ---------- ///

	// マテリアルデータの初期化処理
	void InitializeMaterial();

	// 頂点バッファデータの初期化
	void InitializeVertexBufferData();

	// インデックスデータの初期化
	void InitializeIndexData();

private: /// ---------- メンバ変数 ---------- ///

	DirectXCommon* dxCommon_ = nullptr;

	Camera* camera_ = nullptr;

	// ワールド行列の計算
	WorldTransform worldTransform_;

	// テクスチャ番号
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_;

	//スプライト用のマテリアルソースを作る
	ComPtr <ID3D12Resource> materialResource;
	Material* materialData_ = nullptr;

	ComPtr <ID3D12Resource> vertexResource;// 頂点リソースを作る
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};// 頂点バッファビューを作成する
	VertexData* vertexData_ = nullptr;// 頂点データを設定する
	ComPtr <ID3D12Resource> wvpResource;// TransformationMatrix用のリソース
	TransformationMatrix* wvpData = nullptr;//データを書き込む

	// インデックスバッファを作成および設定する
	ComPtr <ID3D12Resource> indexResource;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	uint32_t* indexData_ = nullptr;

	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 viewProjectionMatrix_;
	Matrix4x4 debugViewProjectionMatrix_;
	bool isDebugCamera_ = false;

	// カラー
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// UV変換行列
	Matrix4x4 uvTransform_ = Matrix4x4::MakeIdentity();
};

