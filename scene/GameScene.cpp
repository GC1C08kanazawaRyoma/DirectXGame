#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	// よしだくん
	int num = 0;

	for (int i = 0; i < 100; i++) {
		// x、y、z 方向のスケーリングを設定
		worldTransform_[i].scale_ = {5.0f, 5.0f, 5.0f};
		if (i <= 50) {
			// x、y、z 軸周りの平行移動を設定
			worldTransform_[i].translation_ = {-40.0f + (i * 10), 20.0f, 10.0f};
		} else if (i > 50) {
			// x、y、z 軸周りの平行移動を設定
			worldTransform_[i].translation_ = {-40.0f + (num * 10), -20.0f, 10.0f};
			num++;
		}
		// ワールドトランスフォーム初期化
		worldTransform_[i].Initialize();

		// ビュープロジェクション初期化
		viewProjection_.Initialize();
	}
}

void GameScene::Update()
{
	/*debugText_->SetPos(50, 50);
	for (int i = 0; i < 100; i++) {
		debugText_->Printf(
		  "translation:(%f,%f,%f)",
			worldTransform_[i].translation_.x,
			worldTransform_[i].translation_.y,
			worldTransform_[i].translation_.z);
	}*/
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	
	/// // 3Dモデルの描画
	for (int i = 0; i < 100; i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
