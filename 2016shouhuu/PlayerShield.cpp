#include "Dxlib.h"
#include "PlayerSword.h"
#include "Enemy.h"
#include "Sound.h"
#include "Media.h"

#define UP -2		//��ړ��l
#define DOWN 2		//���ړ��l

//���e
int shieldpoint;			//�ʒu
int PShMoveUBFlag = 0;		//��{�^���������ꂽ���ǂ�������
int PShMoveDBFlag = 0;		//���{�^���������ꂽ���ǂ�������
int leftcount = 0;			//���u���Ă�����
int LCD = 0;				//���u���Ԃ�600�ɂȂ�����_���[�W�󂯂�����
int LeftDcount = 0;			//���u���ă_���[�W�󂯂����̉摜�ω�
bool LDC = 0;				//���u���ă_���[�W�󂯂����̉摜�ω�
int DamageY;				//�_���[�W�󂯂��\����������Ƃ������悭
int resetDamageY;			//DamageY�������I������0�ɂȂ�悤��
bool ShieldMahouFlag;		//���@�V�[���h�����Ă�
int ShieldMahou;			//�N�[���_�E����������܂ł̃J�E���g
int CoolDown = 100;			//���@�V�[���h�����߂̃N�[���_�E��
bool CoolDownFlag;			//���@�V�[���h���g��ꂽ����

//�R���g���[���[
int PShpad;		//�R���g���[���[


//�v���C���[�̕`��
void PlayerShield_Draw()
{
	//�ʏ�
	if (ShieldMahouFlag == 0 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
	}
	else if (ShieldMahouFlag == 1 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	else
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldCoolDownM, TRUE);
	}
	//���̗E�҂̍U������������
	if (AttackShield() == 1)
	{
		PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	if (PShATTACKHIT() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	//���u���ă_���[�W���󂯂�
	if (LDC == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		SetDrawBright(125, 0, 125);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	if (resetDamageY == 19)
	{
		DamageY = 0;
		resetDamageY = 0;
	}
}

//���X�{�X�`��
void PlayerShield_LastDraw()
{
	//�ʏ�
	if (ShieldMahouFlag == 0 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
	}
	else if (ShieldMahouFlag == 1 && CoolDownFlag == 0)
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	else
	{
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldCoolDownM, TRUE);
	}

	//���̗E�҂̍U������������
	if (AttackShield() == 1)
	{
		PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	//㩂���������
	if (PShATTACKHIT() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		resetDamageY++;
		SetDrawBright(125, 0, 0);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	//���u���ă_���[�W���󂯂�
	if (LDC == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		SetDrawBright(125, 0, 125);
		DrawGraph(200, 250 + shieldpoint * 40, M_Character.PlayerShieldM, TRUE);
		SetDrawBright(255, 255, 255);
		DamageY--;
		DrawFormatString(180, 260 + (shieldpoint * 40) + DamageY, GetColor(255, 0, 255), "-10�b");
	}
	if (resetDamageY == 19)
	{
		DamageY = 0;
		resetDamageY = 0;
	}
}

//�X�V
void PlayerShield_Update()
{
	PShpad = GetJoypadInputState(DX_INPUT_PAD2);
	leftcount++;
	// �J�[�\���ړ�
	if ((PShpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//��{�^���������ꂽ(�R���g���[���[��,�L�[�{�[�h��)
	{
		PShMoveUBFlag++;
		leftcount = 0;
	}
	else  //��{�^����������Ă��Ȃ�
	{
		PShMoveUBFlag = 0;
	}
	if ((PShpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)  //���{�^���������ꂽ(�R���g���[���[��,�L�[�{�[�h��)
	{
		PShMoveDBFlag++;
		leftcount = 0;
	}
	else  //���{�^����������Ă��Ȃ�
	{
		PShMoveDBFlag = 0;
	}
	if (EnemyBreak() >= 3 && CoolDown == 100 && ((PShpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_D) != 0) && ShieldMahou == 0)	//���@�V�[���h�����Ă�
	{
		ShieldMahouFlag = 1;
		leftcount = 0;
	}
	//��Ɉړ�
	if (PShMoveUBFlag == 1 && shieldpoint != UP)
	{
		shieldpoint -= 2;
	}
	//���Ɉړ�
	if (PShMoveDBFlag == 1 && shieldpoint != DOWN)
	{
		shieldpoint += 2;
	}
	if (ShieldMahouFlag == 1)
	{
		ShieldMahou++;
		if (ShieldMahou >= 30)
		{
			ShieldMahouFlag = 0;
			ShieldMahou = 0;
			CoolDownFlag = 1;
		}
	}
	if (CoolDownFlag == 1)
	{
		CoolDown--;
		if (CoolDown <= 0)
		{
			CoolDown = 100;
			CoolDownFlag = 0;
		}
	}
	//���u�ɂ��_���[�W�ǉ�
	if (leftcount % 600 == 0 && leftcount != 0)
	{
		LCD++;
		LeftDcount++;
	}
	//�摜�ω�������
	if (LeftDcount >= 1)
	{
		LDC = 1;
		LeftDcount++;
		if (LeftDcount == 20)
		{
			DamageY = 0;
			LDC = 0;
			LeftDcount = 0;
		}
	}
}

//���X�{�X�ł̍X�V
void PlayerShield_LastUpdate()
{
	PShpad = GetJoypadInputState(DX_INPUT_PAD2);
	leftcount++;
	// �J�[�\���ړ�
	if ((PShpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//��{�^���������ꂽ(�R���g���[���[��,�L�[�{�[�h��)
	{
		PShMoveUBFlag++;
		leftcount = 0;
	}
	else  //��{�^����������Ă��Ȃ�
	{
		PShMoveUBFlag = 0;
	}
	if ((PShpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)  //���{�^���������ꂽ(�R���g���[���[��,�L�[�{�[�h��)
	{
		PShMoveDBFlag++;
		leftcount = 0;
	}
	else  //���{�^����������Ă��Ȃ�
	{
		PShMoveDBFlag = 0;
	}
	if (CoolDown == 100 && ((PShpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_D) != 0) && ShieldMahou == 0)	//���@�V�[���h�����Ă�
	{
		ShieldMahouFlag = 1;
		leftcount = 0;
	}
	//��Ɉړ�
	if (PShMoveUBFlag == 1 && shieldpoint != UP)
	{
		shieldpoint -= 2;
	}
	//���Ɉړ�
	if (PShMoveDBFlag == 1 && shieldpoint != DOWN)
	{
		shieldpoint += 2;
	}
	if (ShieldMahouFlag == 1)
	{
		ShieldMahou++;
		if (ShieldMahou >= 30)
		{
			ShieldMahouFlag = 0;
			ShieldMahou = 0;
			CoolDownFlag = 1;
		}
	}
	if (CoolDownFlag == 1)
	{
		CoolDown--;
		if (CoolDown <= 0)
		{
			CoolDown = 100;
			CoolDownFlag = 0;
		}
	}
	//���u�ɂ��_���[�W�ǉ�
	if (leftcount % 600 == 0 && leftcount != 0)
	{
		LCD++;
		LeftDcount++;
	}
	//�摜�ω�������
	if (LeftDcount >= 1)
	{
		LDC = 1;
		LeftDcount++;
		if (LeftDcount == 20)
		{
			DamageY = 0;
			LDC = 0;
			LeftDcount = 0;
		}
	}
}

//���Z�b�g
void PlayerShield_R()
{
	shieldpoint = 0;
	PShMoveUBFlag = 0;
	PShMoveDBFlag = 0;
	leftcount = 0;
	LCD = 0;
	LeftDcount = 0;
	LDC = 0;
	DamageY = 0;
	resetDamageY = 0;
	ShieldMahouFlag = 0;
	ShieldMahou = 0;
	CoolDown = 100;
	CoolDownFlag = 0;
}

//�ʒu�𓾂�֐�
int Getshieldpoint(void)
{
	return shieldpoint;
}

//���u���Ԃɂ��_���[�W�𓾂�֐�
int Getleftcount(void)
{
	return LCD;
}

//���@�V�[���h�������Ă��邩�ǂ����𓾂�֐�
bool GetShieldMahouFlag()
{
	return ShieldMahouFlag;
}