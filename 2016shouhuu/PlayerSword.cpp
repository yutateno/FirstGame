#include "Dxlib.h"
#include <math.h>
#include "Scenemgr.h"
#include "PlayerShield.h"
#include "Enemy.h"
#include "SgameSecond.h"
#include "SgameLast.h"
#include "Sound.h"
#include "Media.h"

#define g 9.8067	//�d��
#define x_max 2.000	//�U���ړ����x
#define UP -2	//��ړ��l
#define DOWN 2	//���ړ��l

//���e(�E��)
int x;							//x���W
int y;							//y���W
int time1, time2, flag = 0;		//�U�����[�V�����Ɋւ���ϐ�
double t;						//�U���Ɋւ���J�E���g
bool AttackShieldHit = 0;		//�h��ɓ������Ă邩����
int AttackShieldHitcount = 0;	//���b�������Ă邩������
int PSwMoveUBFlag = 0;			//��ړ��{�^���������ꂽ��Ƃ������̃J�E���g
int PSwMoveDBFlag = 0;			//���ړ��{�^���������ꂽ��Ƃ������̃J�E���g
int DFlag;						//�ǂ��炩�̗E�҂��_���[�W���󂯂�
int DWeakcount = 0;				//��U���̏ꍇ�̐������Ԍ���
int DStrongcount = 0;			//�����U���̏ꍇ�̐������Ԍ���
int damageY;					//�_���[�W�̕\����������Ƃ������悭
int PSattack;					//�G�ɍU�����͂������ǂ���
int YWeakcount = 0;				//���(�΋�)�ɓ�������

//���e(��������)
int LimitTime;				//�������ԂɊւ��鎞�Ԃ��L�^
int ccount;					//�������Ԃ̊J�n�𐳊m�ɂ��邽��
int LimitStartTime;			//�������Ԃ̎n�܂�
int TwoBGMStop;				//�Q�[���I�[�o�[�ɂȂ�������BGM���~�߂�J�E���g
bool PSGOFlag;				//�Q�[���I�[�o�[�t���b�O

//�R���g���[���[
int PSwpad;		//�R���g���[���[


//�v���C���[�̕`��
void PlayerSword_Draw()
{
	//�����l�Ɛ^�񒆂�荶�̎��Ɛ^�񒆂��E�̎��ŕ�����
	if (x == 0)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x < 0)
	{
		DrawGraph(100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x > 0 && x <= 200)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	else
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
	//�G�̎�U�����E�҂ɒʂ���
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-10�b");
	}
	//�G�̋��U�����E�҂ɒʂ���
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcount++;
		SetDrawBright(75, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-20�b");
	}
	//����㩂Ɉ���������
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
	}
	//�����΋��U����������
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcount++;
		SetDrawBright(125, 125, 125);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 255, 0), "-10�b");
	}
	//��U���̐������Ԍ���
	if (DWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		DWeakcount = 0;
	}
	if (YWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		YWeakcount = 0;
	}
	//���U���̐������Ԍ���
	if (DStrongcount == 24)
	{
		damageY = 0;
		DFlag += 2;
		DStrongcount = 0;
	}
}

//���X�{�X�̕`��
void PlayerSword_LastDraw()
{
	//�����l�Ɛ^�񒆂�荶�̎��Ɛ^�񒆂��E�̎��ŕ�����
	if (x == 0)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x < 0)
	{
		DrawGraph(100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	else if (x > 0 && x <= 200)
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	else
	{
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
	//�G�̎�U�����E�҂ɒʂ���
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-10�b");
	}
	//�G�̋��U�����E�҂ɒʂ���
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcount++;
		SetDrawBright(75, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 0, 255), "-20�b");
	}
	//����㩂Ɉ���������A��ȍU���𕂂���
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcount++;
	}
	//�����΋��U����������
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcount++;
		SetDrawBright(125, 0, 0);
		DrawGraph(x + 100, (y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		damageY--;
		DrawFormatString(80, (y * 40) + 260 + damageY, GetColor(255, 255, 0), "-10�b");
	}
	//��U���̗̑͌���
	if (DWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		DWeakcount = 0;
	}
	if (YWeakcount == 19)
	{
		damageY = 0;
		DFlag++;
		YWeakcount = 0;
	}
	//���U���̗̑͌���
	if (DStrongcount == 24)
	{
		damageY = 0;
		DFlag += 2;
		DStrongcount = 0;
	}
}

//�X�V
void PlayerSword_Update()
{
	PSwpad = GetJoypadInputState(DX_INPUT_PAD1);
	ccount++;	//�����Ő������Ԃ����n�߂�
	if (ccount == 1)
	{
		LimitTime = GetNowCount();
	}
	// �J�[�\���ړ�
	if ((PSwpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_S) != 0)	//��(�R���g���[���[��,�L�[�{�[�hS)
	{
		PSwMoveUBFlag++;
	}
	else
	{
		PSwMoveUBFlag = 0;
	}
	if ((PSwpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_X) != 0)	//��(�R���g���[���[��,�L�[�{�[�hX)
	{
		PSwMoveDBFlag++;
	}
	else
	{
		PSwMoveDBFlag = 0;
	}
	//s�������ꂽ���ɓ���
	if (PSwMoveUBFlag == 1 && y != UP)
	{
		y -= 2;
	}
	//x�������ꂽ�牺�ɓ���
	if (PSwMoveDBFlag == 1 && y != DOWN)
	{
		y += 2;
	}
	//a�������ꂽ�Ƃ��ɖh�䂪���Ȃ��Ƃ��U������
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y != Getshieldpoint())	//�R���g���[���[B,�L�[�{�[�hA
	{
		PSattack = 0;
		//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
		time1 = GetNowCount();
		//�����Ă�t���O��ۑ�����
		flag = 1;
	}
	//�h�䂪���Ȃ��Ƃ��ɓ����Ă���Ƃ�
	if (flag == 1)
	{
		//�o�ߎ��Ԃ��J�E���g����
		time2 = GetNowCount();
		//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
		t = (double)(time2 - time1) / 1000.000;
		//x���W���v�Z���Ō�̐��l�ő������ς��
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*200.000 / x_max);
		//�G�̈ʒu�ł�߂�
		if (x <= 250)
		{
			PSattack++;
		}
		//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
		else
		{
			flag = 0;
			x = 0;
			PSattack = 0;
		}
	}
	//�h��ƈʒu������Ă����Ƃ�
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y == Getshieldpoint())	//�R���g���[���[B,�L�[�{�[�hA
	{
		//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
		time1 = GetNowCount();
		//�����Ă�t���O��ۑ�����
		flag = 2;
	}
	if (flag == 2)
	{
		//�o�ߎ��Ԃ��J�E���g����
		time2 = GetNowCount();
		//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
		t = (double)(time2 - time1) / 1000.000;
		//x���W���v�Z���Ō�̐��l�ő��x���ς��
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*150.000 / x_max);
		//�h��̈ʒu�ł�߂�
		if (x <= 90 && x >= 0)
		{
		}
		//�h��ɓ͂����瓮������߂ď����l�ɖ߂�
		else
		{
			DFlag++;	//�����ւ̍U���̃y�i���e�B
			AttackShieldHit = 1;
			flag = 0;
			x = 0;
		}
	}
	//�h��ɓ������Ă�Ƃ����b�������Ă锻����c��
	if (AttackShieldHit == 1)
	{
		AttackShieldHitcount++;
		if (AttackShieldHitcount == 20)
		{
			AttackShieldHit = 0;
			AttackShieldHitcount = 0;
		}
	}
}

//���X�{�X�ł̍X�V
void PlayerSword_LastUpdate()
{
	PSwpad = GetJoypadInputState(DX_INPUT_PAD1);
	// �J�[�\���ړ�
	if ((PSwpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_S) != 0)	//��(�R���g���[���[��,�L�[�{�[�hS)
	{
		PSwMoveUBFlag++;
	}
	else
	{
		PSwMoveUBFlag = 0;
	}
	if ((PSwpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_X) != 0)	//��(�R���g���[���[��,�L�[�{�[�hX)
	{
		PSwMoveDBFlag++;
	}
	else
	{
		PSwMoveDBFlag = 0;
	}
	//s�������ꂽ���ɓ���
	if (PSwMoveUBFlag == 1 && y != UP)
	{
		y -= 2;
	}
	//x�������ꂽ�牺�ɓ���
	if (PSwMoveDBFlag == 1 && y != DOWN)
	{
		y += 2;
	}
	//a�������ꂽ�Ƃ��ɖh�䂪���Ȃ��Ƃ��U������
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y != Getshieldpoint())	//�R���g���[���[B,�L�[�{�[�hA
	{
		PSattack = 0;
		//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
		time1 = GetNowCount();
		//�����Ă�t���O��ۑ�����
		flag = 1;
	}
	//�h�䂪���Ȃ��Ƃ��ɓ����Ă���Ƃ�
	if (flag == 1)
	{
		//�o�ߎ��Ԃ��J�E���g����
		time2 = GetNowCount();
		//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
		t = (double)(time2 - time1) / 1000.000;
		//x���W���v�Z���Ō�̐��l�ő������ς��
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*200.000 / x_max);
		//�G�̈ʒu�ł�߂�
		if (x <= 250)
		{
			PSattack++;
		}
		//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
		else
		{
			flag = 0;
			x = 0;
			PSattack = 0;
		}
	}
	//�h��ƈʒu������Ă����Ƃ�
	if (((PSwpad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0) && y == Getshieldpoint())	//�R���g���[���[B,�L�[�{�[�hA
	{
		//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
		time1 = GetNowCount();
		//�����Ă�t���O��ۑ�����
		flag = 2;
	}
	if (flag == 2)
	{
		//�o�ߎ��Ԃ��J�E���g����
		time2 = GetNowCount();
		//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
		t = (double)(time2 - time1) / 1000.000;
		//x���W���v�Z���Ō�̐��l�ő��x���ς��
		x = (int)((sqrt(2.000*g*x_max)*t - 0.150*g*t*t)*150.000 / x_max);
		//�h��̈ʒu�ł�߂�
		if (x <= 90 && x >= 0)
		{
		}
		//�h��ɓ͂����瓮������߂ď����l�ɖ߂�
		else
		{
			DFlag++;	//�����ւ̍U���̃y�i���e�B
			AttackShieldHit = 1;
			flag = 0;
			x = 0;
		}
	}
	//�h��ɓ������Ă�Ƃ����b�������Ă锻����c��
	if (AttackShieldHit == 1)
	{
		AttackShieldHitcount++;
		if (AttackShieldHitcount == 20)
		{
			AttackShieldHit = 0;
			AttackShieldHitcount = 0;
		}
	}
}

//��������(�v���C���[�̃_���[�W�Ɋւ��邱�Ƃ������Ȃ̂�)
void Limit()
{
	LimitStartTime = ((GetNowCount() + (DFlag * 10000) + (Getleftcount() * 10000)) - (SGSecondFlag() * 10000));
	TwoBGMStop = LimitStartTime - LimitTime;
	
	//�Q�[���I�[�o�[����
	if ((LimitStartTime - LimitTime) >= 360000)
	{
		if (GetSLastFlag() == 0)
		{
			PSGOFlag = 1;
			SceneMgr_ChangeScene(eScene_SGameOver);   //�V�[�����Q�[���I�[�o�[�Ɉڂ�
		}
		else
		{
			PSGOFlag = 1;
			SceneMgr_ChangeScene(eScene_SGameOverLast);
		}
	}
	else if ((LimitStartTime - LimitTime) > 300000)	//�Ō�
	{
		DrawFormatString(300, 0, GetColor(255, 255, 125), "%d�b", (360000 - (LimitStartTime - LimitTime)) / 1000);
	}
	else if ((LimitStartTime - LimitTime) <= 300000 && (LimitStartTime - LimitTime)>240000)	//5�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 125), "%d��%d�b", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 60);
	}
	else if ((LimitStartTime - LimitTime) <= 240000 && (LimitStartTime - LimitTime)>180000)	//4�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 120);
	}
	else if ((LimitStartTime - LimitTime) <= 180000 && (LimitStartTime - LimitTime)>120000)	//3�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 180);
	}
	else if ((LimitStartTime - LimitTime) <= 120000 && (LimitStartTime - LimitTime)>60000)	//2�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 240);
	}
	else	//�ŏ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (360000 - (LimitStartTime - LimitTime)) / 60000, (360000 - (LimitStartTime - LimitTime)) / 1000 - 300);
	}
}

//���Z�b�g
void PlayerSword_R()
{
	x = 0;
	y = 0;
	time1 = 0;
	time2 = 0;
	flag = 0;
	t = 0;
	AttackShieldHit = 0;
	AttackShieldHitcount = 0;
	PSwMoveUBFlag = 0;
	PSwMoveDBFlag = 0;
	LimitTime = 0;
	ccount = 0;
	DFlag = 0;
	LimitStartTime = 0;
	DWeakcount = 0;
	DStrongcount = 0;
	damageY = 0;
	PSattack = 0;
	YWeakcount = 0;
	TwoBGMStop = 0;
	PSGOFlag = 0;
}

//�ʒu�𓾂�֐�
int GetswordXpoint(void)
{
	return x;
}
int GetswordYpoint(void)
{
	return y;
}
//�h��ɓ������Ă邩�ǂ���
bool AttackShield()
{
	return AttackShieldHit;
}

int GetPSattack(void)
{
	return PSattack;
}

bool GetTwoBGMStop()
{
	return PSGOFlag;
}