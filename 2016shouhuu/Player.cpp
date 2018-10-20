#include "Dxlib.h"
#include <math.h>
#include "Scenemgr.h"
#include "Enemy.h"
#include "GameSecond.h"
#include "GameLast.h"
#include "Sound.h"
#include "Media.h"

#define up -2			//��ړ��l
#define down 2			//���ړ��l
#define G 9.8067		//�d��
#define X_MAX 2.000		//�U���̈ړ����x

//���e�F�E��
int X;								//X���W
int Y;								//Y���W
int PMoveUFlag;						//�オ�����ꂽ��t���b�O�Ƃ������̃J�E���g
int PMoveDFlag;						//���������ꂽ��t���b�O�Ƃ������̃J�E���g
int TIME1;							//�U���Ɋւ���J�E���g
bool FLAG;							//�U���{�^���������ꂽ���ǂ���
int TIME2;							//�U���Ɋւ���J�E���g
double T;							//�U���Ɋւ���J�E���g
int DWeakcoun;						//��U������������
int DAmageY;						//�_���[�W�̕\����������ƗV��
int DStrongcoun;					//���U��(�΋�)������������
int DFLag;							//�_���[�W�~��
int Pattack;						//�G�ɍU�����͂������ǂ����Ɋւ���J�E���g
int YWeakcoun;						//�΋��U������������
double PlayAttackBar;				//�Q�[�W�̒���
bool RabitFlag;						//�U�����x�㏸�\�t���b�O
bool RabitUseFlag;					//�U�����x�㏸���g�����t���b�O
double LastPlayAttackBar = 120;		//�Q�[�W�̒���
bool LastRabitFlag;					//�U�����x�㏸�\�t���b�O
bool LastRabitUseFlag;				//�U�����x�㏸���g�����t���b�O

//���e�F��������
int CCOunt;				//�������Ԃ��n�߂�t���b�O�̂悤�Ȃ���
int LimitTimE;			//�������Ԃ̕⍲
int LimitStartTimE;		//�������Ԃ̑�{
int OneBGMStop;			//�Q�[���I�[�o�[�ւ̃V�[���̐؂�ւ�����BGM���~�߂邽�߂̂���
bool PGOFlag;			//�������Ԑ؂ꂽ�t���b�O

//�R���g���[���[
int Ppad;		//�R���g���[���[


//�G����̕`��
void Player_Draw()
{
	//�Q�[�W(�S�̖ڂ���g����V�����Z
	if (EnemyBreak() >= 3)
	{
		DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
		DrawRectGraph(80, 120, (int)PlayAttackBar, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);
	}
	//�V���Ȍ��̓���Z���g���ĂȂ��Ƃ�
	if (RabitUseFlag == 0)
	{
		//�����l
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//�^�񒆂�荶�̎�
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//�^�񒆂��E�̎�
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
		}
		//���X�{�X�ɓ������u�Ԃ��������Ȃ�̂�h���p
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
		}
	}
	//�V���Ȍ��̓���Z���g���Ă���Ƃ�
	else
	{
		//�����l
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//�^�񒆂�荶�̎�
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//�^�񒆂��E�̎�
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[1], TRUE);
		}
		//���X�{�X�ɓ������u�Ԃ��������Ȃ�̂�h���p
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[2], TRUE);
		}
	}
	//�V���Ȍ�����ɓ���ĂȂ��Ƃ�
	if (EnemyBreak() < 3)
	{
		//�G�̎�U�����E�҂ɒʂ���
		if (EAttackHit() == 1)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
		}
		//�G�̋��U�����E�҂ɒʂ���
		if (EAttackHit() == 2)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DStrongcoun++;
			SetDrawBright(75, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20�b");
		}
		//�����΋��U����������
		if (GetYAttackHit() == 1)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			YWeakcoun++;
			SetDrawBright(125, 125, 125);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20�b");
		}
		//�O�����
		if (EAttackHit() == 3)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
		}
	}
	//�V���Ȍ�����ɓ��ꂽ��
	else
	{
		//�G�̎�U�����E�҂ɒʂ���
		if (EAttackHit() == 1)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
			if (RabitUseFlag == 0)		//�Q�[�W���[���ɂ���
			{
				PlayAttackBar = 120;
			}
		}
		//�G�̋��U�����E�҂ɒʂ���
		if (EAttackHit() == 2)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DStrongcoun++;
			SetDrawBright(75, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20�b");
			if (RabitUseFlag == 0)		//�Q�[�W���[���ɂ���
			{
				PlayAttackBar = 120;
			}
		}
		//������ȍU����������
		if (GetYAttackHit() == 1)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			YWeakcoun++;
			SetDrawBright(125, 125, 125);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20�b");
			if (RabitUseFlag == 0)		//�Q�[�W���[���ɂ���
			{
				PlayAttackBar = 120;
			}
		}
		//�O�����
		if (EAttackHit() == 3)
		{
			PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
			DWeakcoun++;
			SetDrawBright(125, 0, 0);
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
			SetDrawBright(255, 255, 255);
			DAmageY--;
			DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
			if (RabitUseFlag == 0)		//�Q�[�W���[���ɂ���
			{
				PlayAttackBar = 120;
			}
		}
	}
	//��U���̐������Ԍ���
	if (DWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag++;
		DWeakcoun = 0;
	}
	//���U���̐������Ԍ���
	if (DStrongcoun == 24)
	{
		DAmageY = 0;
		DFLag += 2;
		DStrongcoun = 0;
	}
	//���@�U���̐������Ԍ���
	if (YWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag += 2;
		YWeakcoun = 0;
	}
}

//���X�{�X�̕`��
void Player_LastDraw()
{
	//�Q�[�W�iY���\�͂��̂܂܂���
	DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
	DrawRectGraph(80, 120, (int)LastPlayAttackBar, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);

	//�V���Ȍ��̓���Z���g���ĂȂ��Ƃ�
	if (LastRabitUseFlag == 0)
	{
		//�����l
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//�^�񒆂�荶�̎�
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		}
		//�^�񒆂��E�̎�
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
		}
		//���X�{�X�ɓ������Ƃ����Ȃ����Ȃ�̂�h���p
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
		}
	}
	//�V���Ȍ��̓���Z���g���Ă���Ƃ�
	else
	{
		//�����l
		if (X == 0)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//�^�񒆂�荶�̎�
		else if (X < 0)
		{
			DrawGraph(100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[0], TRUE);
		}
		//�^�񒆂��E�̎�
		else if (X > 0 && X <= 200)
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[1], TRUE);
		}
		//���X�{�X�ɓ������Ƃ����Ȃ����Ȃ�̂�h���p
		else
		{
			DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordYellowM[2], TRUE);
		}
	}
	//�G�̎�U�����E�҂ɒʂ���
	if (EAttackHit() == 1)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcoun++;
		SetDrawBright(125, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
		if (LastRabitUseFlag == 0)			//�Q�[�W���O�ɂ���
		{
			LastPlayAttackBar = 120;
		}
	}
	//�G�̋��U�����E�҂ɒʂ���
	if (EAttackHit() == 2)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DStrongcoun++;
		SetDrawBright(75, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-20�b");
		if (LastRabitUseFlag == 0)			//�Q�[�W���O�ɂ���
		{
			LastPlayAttackBar = 120;
		}
	}
	//������ȍU����������
	if (GetYAttackHit() == 1)
	{
		PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
		YWeakcoun++;
		SetDrawBright(125, 125, 125);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(80, (Y * 40) + 260 + DAmageY, GetColor(255, 255, 0), "-20�b");
		if (LastRabitUseFlag == 0)			//�Q�[�W���O�ɂ���
		{
			LastPlayAttackBar = 120;
		}
	}
	//㩂ɓ���������
	if (EAttackHit() == 3)
	{
		PlaySoundMem(S_SE.HitS, DX_PLAYTYPE_BACK, TRUE);
		DWeakcoun++;
		SetDrawBright(125, 0, 0);
		DrawGraph(X + 100, (Y * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		SetDrawBright(255, 255, 255);
		DAmageY--;
		DrawFormatString(180, (Y * 40) + 260 + DAmageY, GetColor(255, 0, 255), "-10�b");
		if (LastRabitUseFlag == 0)			//�Q�[�W���O�ɂ���
		{
			LastPlayAttackBar = 120;
		}
	}
	//��U���̐������Ԍ���
	if (DWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag++;
		DWeakcoun = 0;
	}
	//���U���̐������Ԍ���
	if (DStrongcoun == 24)
	{
		DAmageY = 0;
		DFLag += 2;
		DStrongcoun = 0;
	}
	//���@�U���̐������Ԍ���
	if (YWeakcoun == 19)
	{
		DAmageY = 0;
		DFLag += 2;
		YWeakcoun = 0;
	}
}

//�G����̍X�V
void Player_Update()
{
	Ppad = GetJoypadInputState(DX_INPUT_PAD1);
	CCOunt++;	//�����Ő������Ԃ��n�߂�
	if (CCOunt == 1)
	{
		LimitTimE = GetNowCount();
	}
	//�J�[�\���ړ�
	if ((Ppad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//��(�R���g���[���[��,�L�[�{�[�h��)
	{
		PMoveUFlag++;
	}
	else
	{
		PMoveUFlag = 0;
	}
	if ((Ppad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)	//��(�R���g���[���[��,�L�[�{�[�h��)
	{
		PMoveDFlag++;
	}
	else
	{
		PMoveDFlag = 0;
	}
	//���������ꂽ���ɓ���
	if (PMoveUFlag == 1 && Y != up)
	{
		Y -= 2;
		if (RabitUseFlag == 0)	//�ړ��ŃQ�[�W��������
		{
			PlayAttackBar -= 5;
		}
	}
	//���������ꂽ�牺�ɓ���
	if (PMoveDFlag == 1 && Y != down)
	{
		Y += 2;
		if (RabitUseFlag == 0)	//�ړ��ŃQ�[�W��������
		{
			PlayAttackBar -= 5;
		}
	}
	//�V���Ȍ��̓���Z���g���Ă����ԂōU�����Ă��Ȃ��Ƃ�
	if (RabitUseFlag == 0)
	{
		//a�������ꂽ�Ƃ��U������
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//�R���g���[���[B,�L�[�{�[�hZ
		{
			Pattack = 0;
			//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
			TIME1 = GetNowCount();
			//�����Ă�t���O��ۑ�����
			FLAG = 1;
		}
		//�����Ă���Ƃ�
		if (FLAG == 1)
		{
			//�o�ߎ��Ԃ��J�E���g����
			TIME2 = GetNowCount();
			//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x���W���v�Z���Ō�̐��l�ő������ς��
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*175.000 / X_MAX);	
			//�G�̈ʒu�ł�߂�
			if (X <= 250)
			{
				Pattack++;
			}
			//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//�V���Ȍ��̓���Z���g���Ă����ԂōU�������Ƃ�
	else
	{
		//a�������ꂽ�Ƃ��U������
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//�R���g���[���[B,�L�[�{�[�hZ
		{
			Pattack = 0;
			//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
			TIME1 = GetNowCount();
			//�����Ă�t���O��ۑ�����
			FLAG = 1;
		}
		//�����Ă���Ƃ�
		if (FLAG == 1)
		{
			//�o�ߎ��Ԃ��J�E���g����
			TIME2 = GetNowCount();
			//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x���W���v�Z���Ō�̐��l�ő������ς��
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*300.000 / X_MAX);
			//�G�̈ʒu�ł�߂�
			if (X <= 250)
			{
				Pattack++;
			}
			//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//�Q�[�W�����܂�����
	if (PlayAttackBar <= 0)
	{
		PlayAttackBar = 0;
		RabitFlag = 1;
	}
	//�Q�[�W���ێ�������
	if (RabitFlag == 1)
	{
		PlayAttackBar = 0;
	}
	//����Z�i�U�����x�㏸�j�������ꂽ��
	if (EnemyBreak() >= 3 && RabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X == 0)
	{
		RabitUseFlag = 1;
	}
	//�����l�ȊO�ŉ����ꂽ�甽�f���Ȃ��悤��
	else if (EnemyBreak() >= 3 && RabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X > 0)
	{
		RabitFlag = 1;
	}
	//�Q�[�W�����炵�Ă���
	if (RabitUseFlag == 1)
	{
		RabitFlag = 0;
		PlayAttackBar += 0.5;
		if (PlayAttackBar >= 120)
		{
			PlayAttackBar = 120;
			RabitUseFlag = 0;
		}
	}
}

//���X�{�X�ł̍X�V
void Player_LastUpdate()
{
	Ppad = GetJoypadInputState(DX_INPUT_PAD1);
	// �J�[�\���ړ�
	if ((Ppad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)	//��(�R���g���[���[��,�L�[�{�[�h��)
	{
		PMoveUFlag++;
	}
	else
	{
		PMoveUFlag = 0;
	}
	if ((Ppad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)	//��(�R���g���[���[��,�L�[�{�[�h��)
	{
		PMoveDFlag++;
	}
	else
	{
		PMoveDFlag = 0;
	}
	//���������ꂽ���ɓ���
	if (PMoveUFlag == 1 && Y != up)
	{
		Y -= 2;
		if (LastRabitUseFlag == 0)
		{
			LastPlayAttackBar -= 5;
		}
	}
	//���������ꂽ�牺�ɓ���
	if (PMoveDFlag == 1 && Y != down)
	{
		Y += 2;
		if (LastRabitUseFlag == 0)
		{
			LastPlayAttackBar -= 5;
		}
	}
	if (LastRabitUseFlag == 0)
	{
		//a�������ꂽ�Ƃ��ɍU������
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//�R���g���[���[B,�L�[�{�[�hA
		{
			Pattack = 0;
			//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
			TIME1 = GetNowCount();
			//�����Ă�t���O��ۑ�����
			FLAG = 1;
		}
		//�����Ă���Ƃ�
		if (FLAG == 1)
		{
			//�o�ߎ��Ԃ��J�E���g����
			TIME2 = GetNowCount();
			//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x���W���v�Z���Ō�̐��l�ő������ς��
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*175.000 / X_MAX);
			//�G�̈ʒu�ł�߂�
			if (X <= 250)
			{
				Pattack++;
			}
			//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	else
	{
		//a�������ꂽ�Ƃ��U������
		if ((Ppad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)	//�R���g���[���[B,�L�[�{�[�hZ
		{
			Pattack = 0;
			//�����ꂽ�Ƃ��̎��Ԃ��J�E���g����
			TIME1 = GetNowCount();
			//�����Ă�t���O��ۑ�����
			FLAG = 1;
		}
		//�����Ă���Ƃ�
		if (FLAG == 1)
		{
			//�o�ߎ��Ԃ��J�E���g����
			TIME2 = GetNowCount();
			//�b�ɕϊ����ĉ����ꂽ�Ƃ��̌o�ߎ��Ԃ��v�Z����
			T = (double)(TIME2 - TIME1) / 1000.000;
			//x���W���v�Z���Ō�̐��l�ő������ς��
			X = (int)((sqrt(2.000*G*X_MAX)*T - 0.150*G*T*T)*300.000 / X_MAX);
			//�G�̈ʒu�ł�߂�
			if (X <= 250)
			{
				Pattack++;
			}
			//�G�ɓ͂����瓮������߂ď����l�ɖ߂�
			else
			{
				FLAG = 0;
				X = 0;
				Pattack = 0;
			}
		}
	}
	//�Q�[�W�����܂�����
	if (LastPlayAttackBar <= 0)
	{
		LastPlayAttackBar = 0;
		LastRabitFlag = 1;
	}
	//�Q�[�W���ێ�������
	if (LastRabitFlag == 1)
	{
		LastPlayAttackBar = 0;
	}
	//���x�㏸�������ꂽ��
	if (LastRabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X == 0)
	{
		LastRabitUseFlag = 1;
	}
	//�����l�ȊO�ŉ����ꂽ��
	else if (LastRabitFlag == 1 && ((CheckHitKey(KEY_INPUT_W) != 0) || (Ppad & PAD_INPUT_A)) && X > 0)
	{
		LastRabitFlag = 1;
	}
	//�Q�[�W�����炵�Ă���
	if (LastRabitUseFlag == 1)
	{
		LastRabitFlag = 0;
		LastPlayAttackBar += 0.5;
		if (LastPlayAttackBar >= 120)
		{
			LastPlayAttackBar = 120;
			LastRabitUseFlag = 0;
		}
	}
}

//��������
void LIMIT()
{
	LimitStartTimE = ((GetNowCount() + (DFLag * 10000)) - (GSecondFlag() * 10000));
	OneBGMStop = LimitStartTimE - LimitTimE;
	
	//�Q�[���I�[�o�[����
	if ((LimitStartTimE - LimitTimE) >= 300000)
	{
		if (GetLastFlag() == 0)
		{
			PGOFlag = 1;
			SceneMgr_ChangeScene(eScene_GameOver);   //�V�[�����Q�[���I�[�o�[�Ɉڂ�
		}
		else
		{
			PGOFlag = 1;
			SceneMgr_ChangeScene(eScene_GameOverLast);   //�V�[�����Q�[���I�[�o�[�Ɉڂ�
		}
	}
	else if ((LimitStartTimE - LimitTimE) > 240000)	//�Ō�1
	{
		DrawFormatString(270, 0, GetColor(255, 255, 125), "%d�b", (300000 - (LimitStartTimE - LimitTimE)) / 1000);
	}
	else if ((LimitStartTimE - LimitTimE) <= 240000 && (LimitStartTimE - LimitTimE) > 180000)	//2�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 60);
	}
	else if ((LimitStartTimE - LimitTimE) <= 180000 && (LimitStartTimE - LimitTimE) > 120000)	//3�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 120);
	}
	else if ((LimitStartTimE - LimitTimE) <= 120000 && (LimitStartTimE - LimitTimE) > 60000)	//4�Ԗ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 180);
	}
	else//�ŏ�
	{
		DrawFormatString(270, 0, GetColor(255, 255, 255), "%d��%d�b", (300000 - (LimitStartTimE - LimitTimE)) / 60000, (300000 - (LimitStartTimE - LimitTimE)) / 1000 - 240);
	}
}

//���Z�b�g
void Player_R()
{
	X = 0;
	Y = 0;
	PMoveUFlag = 0;
	PMoveDFlag = 0;
	TIME1 = 0;
	FLAG = 0;
	TIME2 = 0;
	T = 0;
	DWeakcoun = 0;
	DAmageY = 0;
	DStrongcoun = 0;
	DFLag = 0;
	CCOunt = 0;
	LimitTimE = 0;
	LimitStartTimE = 0;
	Pattack = 0;
	YWeakcoun = 0;
	OneBGMStop = 0;
	PGOFlag = 0;
	PlayAttackBar = 0;
	RabitFlag = 0;
	RabitUseFlag = 0;
	LastPlayAttackBar = 120;
	LastRabitFlag = 0;
	LastRabitUseFlag = 0;
}

//�ʒu�擾
int GetPlayerXpoint()
{
	return X;
}

int GetPlayerYpoint()
{
	return Y;
}

int GetPattack()
{
	return Pattack;
}

bool GetOneBGMStop()
{
	return PGOFlag;
}

//�U�����x�㏸���g���Ă��邩�ǂ���
bool GetRabitUseFlag()
{
	return RabitUseFlag;
}

//�U�����x�㏸���g���Ă��邩�ǂ���
bool GetLastRabitUseFlag()
{
	return LastRabitUseFlag;
}