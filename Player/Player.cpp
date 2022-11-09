#include "Player.h"
#include "Enemy.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0, 0, 0 };
}

void Player::Update(ViewProjection viewProjection_) {

	Move();

	//�d��
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


	//�W�����v
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

	//�W�����v�̗͂����X�ɋ���
	jumpPower *= 1.1f;
	//���܂ł�������0�ɂ���
	if (jumpPower > 1) {
		jumpPower = 0;
	}

	worldTransform_.translation_ += move;
	worldTransform_.translation_.y += jumpPower;

	//�J�����ƃI�u�W�F�N�g�̋����𑪂�
	viewLength = worldTransform_.translation_ - viewProjection_.eye;
	viewLength.length();
	viewLength.normalize();
	//�J�����ɋ߂Â������Ȃ��悤��
	if (viewLength.z >= 0.067) {
		worldTransform_.translation_.z += kCharacterSpeed * airPower;
	}
	

	//�s��X�V
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();


	const float kMoveLimitX = 35;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);



	//�e���ˏ���
	Attack();

	//�e�X�V
	//����
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�P��
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
		//�e�̑��x
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = bVelocity(velocity, worldTransform_);

		//�e�𐶐���������
		//����
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//�P��
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(model_, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), velocity);

		//�e�̓o�^
	   //����
		bullets_.push_back(std::move(newBullet));

		//�P��
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
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ�����
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

	//�����������ňړ��x�N�g����ύX
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



