#include "Player.h"
#include "Enemy.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0, 0, 0 };
}

void Player::Update(ViewProjection viewProjection_) {

	Move();

	//重力
	if (jumpPower == 0) {
		if (worldTransform_.translation_.y > 0) {
			worldTransform_.translation_.y -= 0.3f;
		}
		else if (worldTransform_.translation_.y <= 0) {
			worldTransform_.translation_.y = 0.0f;
			if (isJump == true) {
				isJump = false;
			}
		}
	}


	//ジャンプ
	if (isJump == false) {
		if (input_->TriggerKey(DIK_SPACE)) {
			jumpPower = 0.5f;
			isJump = true;
		}
		airPower = 1.0f;
	}
	else {
		airPower = 0.7f;
	}

	//ジャンプの力を徐々に強く
	jumpPower *= 1.1f;
	//一定までいったら0にする
	if (jumpPower > 1) {
		jumpPower = 0;
	}

	worldTransform_.translation_ += move;
	worldTransform_.translation_.y += jumpPower;

	//カメラとオブジェクトの距離を測る
	viewLength = worldTransform_.translation_ - viewProjection_.eye;
	viewLength.length();
	viewLength.normalize();
	//カメラに近づきすぎないように
	if (viewLength.z >= 0.067) {
		worldTransform_.translation_.z += kCharacterSpeed * airPower;
	}
	

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();


	const float kMoveLimitX = 35;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);



	//弾発射処理
	Attack();

	//弾更新
	//複数
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//単発
	/*if (bullet_) {
		bullet_->Update();
	}*/

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//弾を生成し初期化
		//複数
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//単発
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(model_, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), velocity);

		//弾の登録
	   //複数
		bullets_.push_back(std::move(newBullet));

		//単発
		/*bullet_.reset(newBullet);*/
	}

}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) {

	Vector3 result = { 0, 0, 0 };


	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
		velocity.y * worldTransform.matWorld_.m[1][0] +
		velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
		velocity.y * worldTransform.matWorld_.m[1][1] +
		velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
		velocity.y * worldTransform.matWorld_.m[1][2] +
		velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}

Vector3 Player::GetWorldPosition2() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::setparent(WorldTransform* worldTransform) {
	worldTransform_.parent_ = worldTransform;
}
void Player::Move() {
	//
	kCharacterSpeed = 0.5f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_UP)) {
		move = { 0, 0, kCharacterSpeed * airPower };
		if (input_->PushKey(DIK_LEFT)) {
			kCharacterSpeed = 0.3f;
			move = { -kCharacterSpeed * airPower, 0, kCharacterSpeed * airPower };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			kCharacterSpeed = 0.3f;
			move = { kCharacterSpeed * airPower, 0, kCharacterSpeed * airPower };
		}
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0, 0, -kCharacterSpeed * airPower };
		if (input_->PushKey(DIK_LEFT)) {
			kCharacterSpeed = 0.3f;
			move = { -kCharacterSpeed * airPower, 0, -kCharacterSpeed * airPower };
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			kCharacterSpeed = 0.3f;
			move = { kCharacterSpeed * airPower, 0, -kCharacterSpeed * airPower };
		}
	}
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed * airPower, 0, 0 };
		if (input_->PushKey(DIK_UP)) {
			kCharacterSpeed = 0.3f;
			move = { -kCharacterSpeed * airPower, 0, kCharacterSpeed * airPower };
		}
		else if (input_->PushKey(DIK_DOWN)) {
			kCharacterSpeed = 0.3f;
			move = { -kCharacterSpeed * airPower, 0, -kCharacterSpeed * airPower };
		}
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed * airPower, 0, 0 };
		if (input_->PushKey(DIK_UP)) {
			kCharacterSpeed = 0.3f;
			move = { kCharacterSpeed * airPower, 0, kCharacterSpeed * airPower };
		}
		else if (input_->PushKey(DIK_DOWN)) {
			kCharacterSpeed = 0.3f;
			move = { kCharacterSpeed * airPower, 0, -kCharacterSpeed * airPower };
		}
	}
}



