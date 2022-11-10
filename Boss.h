#pragma once
#include "Input.h"
#include <cassert>
#include "DebugText.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include "ViewProjection.h"
#include "Time.h"


class Player;

class GameScene;

class Boss {
public:
	Boss();
	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///初期化
	///</summary>
	void Update(Vector3 player);

	///< summary>
	///初期化
	///</summary>
	void Draw(ViewProjection viewProjection_);

	void Attack(Vector3 Player);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	///------------------------------------------///
	//メンバ変数
	bool isJump = false;
	//攻撃クールタイム
	int coolTime;
	//攻撃パターン
	int attackNum;
};
