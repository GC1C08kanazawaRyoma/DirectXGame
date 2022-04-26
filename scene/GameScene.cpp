#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

using namespace DirectX;
using namespace std;

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize()
{
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	// 乱数シード生成器
	random_device seed_gen;
	// メルセンヌ・ツイスター
	mt19937_64 engine(seed_gen());
	// 乱数範囲（回転角用）
	uniform_real_distribution<float> rotDist(0.0f, XM_2PI);
	// 乱数範囲（座標用）
	uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	// カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = XMConvertToRadians(10.0f);

	// アスペクト比を設定
	viewProjection_.aspectRatio = 1.0f;
	// ニアクリップ距離を設定
	viewProjection_.nearZ = 52.0f;
	// ファークリップ距離を設定
	viewProjection_.farZ = 53.0f;

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルの生成
	model_ = Model::Create();

	// 親(0番)
	worldTransform_[0].Initialize();
	// 子(1番)
	worldTransform_[1].translation_ = {0, 4.5f, 0};
	worldTransform_[1].parent_ = &worldTransform_[0];
	worldTransform_[1].Initialize();

	// ビュープロジェクション初期化
	viewProjection_.Initialize();
}

void GameScene::Update()
{	
	// キャラクター移動処理
	{
		// キャラクターの移動ベクトル
		XMFLOAT3 move = {0, 0, 0};
		// キャラクターの移動速度
		const float kCharactoerSpeed = 0.2f;
		// 押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move = {-kCharactoerSpeed, 0, 0};
		} else if (input_->PushKey(DIK_RIGHT)) {
			move = {kCharactoerSpeed, 0, 0};
		}
		// 注視点移動（ベクトルの加算）
		worldTransform_[PartId::Root]]
	}
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
	for (size_t i = 0; i < _countof(worldTransform_); i++)
	{
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
