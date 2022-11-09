#pragma once
#include "Input.h"
#include <cassert>
#include "DebugText.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include "ViewProjection.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>


class Player {
  public:
	  ///<summary>
	  ///初期化
	  ///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///初期化
	///</summary>
	void Update(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	void Draw(ViewProjection viewProjection_);

	///< summary>
	///初期化
	///</summary>
	void Attack();
	void Move();

	//キャラの向きに応じた方向に球をだす
	Vector3 bVelocity(Vector3& velocity, WorldTransform& worldTransform);

	//ワールド座標を取得
	Vector3 GetWorldPosition2();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void setparent(WorldTransform* worldTransform);

	void ResetPlayer();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	

  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	
	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;
	//キャラクターの移動ベクトル
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	float kCharacterSpeed = 0.5f;
	//ジャンプ
	float jumpPower = 0.0f;
	bool isJump = 0;
	float airPower = 1.0f;
	Vector3 viewLength;

	//弾
	//複数 
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	

};
