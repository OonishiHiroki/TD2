#include "Boss.h"

Boss::Boss() {
	srand(time(NULL));
}

void Boss::Initialize(Model* model, uint32_t textureHandle) {
	//NULL�|�C���^�`�F�b�N
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();
	worldTransform_.translation_ = { 0, 3, 15 };
	worldTransform_.scale_ = { 4,4,4 };
}

void Boss::Update(Vector3 player) {

	//�d��
	if (isJump == false) {
		if (worldTransform_.translation_.y > 3) {
			worldTransform_.translation_.y -= 0.9f;
		}
		else if (worldTransform_.translation_.y <= 3) {
			worldTransform_.translation_.y = 3.0f;
			if (isJump == true) {
				isJump = false;
			}
		}
	}
	Attack(player);

	//�s��X�V
	AffinTrans::affin(worldTransform_);
	worldTransform_.TransferMatrix();
	//�f�o�b�O�e�L�X�g
	debugText_->SetPos(50, 250);
	debugText_->Printf(
		"translation : %f,%f,%f", worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
	debugText_->SetPos(50, 300);
	debugText_->Printf(
		"%d",attackNum);
}

void Boss::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}

void Boss::Attack(Vector3 Player) {
	switch (attackNum) {
	//�U���p�^�[�����I
	case 0:

		attackNum = rand() % 3 + 1;
		break;

	case 1:
		coolTime++;
		if (coolTime == 100) {
			coolTime = 0;
			attackNum = 0;
		}
		break;

	case 2:
		coolTime++;
		if (coolTime == 100) {
			coolTime = 0;
			attackNum = 0;
		}
		break;

	case 3:
		coolTime++;
		if (coolTime == 100) {
			coolTime = 0;
			attackNum = 0;
		}
		break;
	}
}