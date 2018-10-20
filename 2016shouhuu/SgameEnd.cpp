#include "DxLib.h"
#include "Background.h"
#include "PlayerShield.h"
#include "PlayerSword.h"
#include "scenemgr.h"
#include "SgameLast.h"
#include "Enemy.h"
#include "SgameFirst.h"
#include "SgameSecond.h"
#include "Sound.h"
#include "Media.h"

//���e
double PSwEndMoveX, PSwEndMoveY;	//���̗E�҂̓���
double PShEndMoveX, PShEndMoveY;	//���̗E�҂̓���
double PrinMoveX, PrinMoveY;		//�P�̓���
int EnemyFeed;						//�G�������Ă����̂Ɋւ���
double EnemyFeedOut = 1;			//�G�������Ă����̂Ɋւ���
int SgEcount;						//�S�̗̂���
int D = 255;						//�t�F�[�h�A�E�g�֘A

//�R���g���[���[
int SGEpad;		//�R���g���[���[


//���x�������Ă���
void DowN()
{
	if (D >= 0)
	{
		D -= 3;
	}
}
void DarK()
{
	if (D >= 0)
	{
		SetDrawBright(D, D, D);
	}
}

//���Z�b�g
void SGameEnd_R()
{
	PSwEndMoveX = 0;
	PSwEndMoveY = 0;
	PShEndMoveX = 0;
	PShEndMoveY = 0;
	PrinMoveX = 0;
	PrinMoveY = 0;
	EnemyFeed = 0;
	EnemyFeedOut = 1;
	SgEcount = 0;
	D = 255;
}

//�X�V
void SgameEnd_Update()
{
	SGEpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGEpad & PAD_INPUT_7) && (SGEpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	if (SGEpad & PAD_INPUT_7)   //BACK �L�[��������Ă�����
	{
		StopSoundMem(S_BGM.MenuS);
		Background_R();
		Enemy_R();
		PlayerShield_R();
		PlayerSword_R();
		SGameEnd_R();
		SGameFirst_R();
		SGameLast_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
	SgEcount++;
	if (SgEcount >= 240 && SgEcount <= 365)
	{
		// ���ʂ̐ݒ�
		ChangeVolumeSoundMem(150, S_BGM.MenuS);

		// ���̍Đ�
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP, FALSE);
		//������ŏ�����
		if (GetswordYpoint() == -2 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*�ŏI�I�ɓ��������@����������//
			PSwEndMoveX = 490	X=590
			PSwEndMoveY = 60	Y=230
			PShEndMoveX = 390	X=590
			PShEndMoveY = 160	Y=330
			*/
		}
		//�����^�񒆂ŏ�����
		else if (GetswordYpoint() == 0 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 160
			*/
		}
		//�������ŏ�����
		else if (GetswordYpoint() == 2 && Getshieldpoint() == -2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			PShEndMoveY += 1.28;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 160
			*/
		}
		//������ŏ����^��
		else if (GetswordYpoint() == -2 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = 60
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//�����^�񒆂ŏ����^��
		else if (GetswordYpoint() == 0 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//�������ŏ����^��
		else if (GetswordYpoint() == 2 && Getshieldpoint() == 0)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			PShEndMoveY += 0.64;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 80
			*/
		}
		//������ŏ�����
		else if (GetswordYpoint() == -2 && Getshieldpoint() == 2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY += 0.48;
			PShEndMoveX += 3.12;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = 60
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
		//�����^�񒆂ŏ�����
		else if (GetswordYpoint() == 0 && Getshieldpoint() == 2)
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.16;
			PShEndMoveX += 3.12;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -20
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
		//�������ŏ�����
		else
		{
			PSwEndMoveX += 3.92;
			PSwEndMoveY -= 0.8;
			PShEndMoveX += 3.12;
			/*�ŏI�I��//
			PSwEndMoveX = 490
			PSwEndMoveY = -100
			PShEndMoveX = 390
			PShEndMoveY = 0
			*/
		}
	}
	if (SgEcount >= 370 && SgEcount <= 395)
	{
		PSwEndMoveX -= 2;
		PSwEndMoveY += 2;
		PShEndMoveX += 1.68;
		PShEndMoveY -= 2;
		/*�ŏI�I�ɓ��������@�����O ��������
		PSwEndMoveX = -50	X=590  X=540
		PSwEndMoveY = 50	Y=230  Y=280
		PShEndMoveX = 42	X=590  X=632
		PShEndMoveY = -50	Y=330  Y=280
		*/
	}
	if (SgEcount >= 397 && SgEcount <= 407)
	{
		PrinMoveY += 0.3;
		//PrinMoveY=3
	}
	if (SgEcount >= 409)
	{
		PSwEndMoveX -= 5;
		PShEndMoveX -= 5;
		PrinMoveX -= 5;
		/*�ŏI�I�ɓ��������@�����O  ��������
		PSwEndMoveX =682	X=540	X=-142	
		PShEndMoveX =682 	X=632	X=-50
		PrinMoveX	=682	X=590	X=-92
		*/
	}
	if (SgEcount >= 430 && SgEcount < 515)
	{
		DowN();
	}
	if (SgEcount >= 515)
	{
		D = 255;
	}
}

//�`��
void SgameEnd_Draw()
{
	// ��ʑS�̂�`��\�ɂ���
	SetDrawArea(0, 0, 640, 480);
	LastBackground_Draw();	//�w�i�̕`��
	//���̕`��
	if (SgEcount < 370)
	{
		DrawGraph((int)PSwEndMoveX + 100, (int)PSwEndMoveY + (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//�P�̕`��
	DrawGraph((int)PrinMoveX + 590, (int)PrinMoveY + 300, M_Character.PrincessM, TRUE);
	//���̕`��
	if (SgEcount >= 370)
	{
		DrawTurnGraph((int)PSwEndMoveX + 100, (int)PSwEndMoveY + (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//���̕`��
	if (SgEcount < 396)
	{
		DrawGraph((int)PShEndMoveX + 200, (int)PShEndMoveY + 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
	}
	if (SgEcount >= 396)
	{
		DrawTurnGraph((int)PShEndMoveX + 200, (int)PShEndMoveY + 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
	}
	//�G�̕`��
	if (SgEcount < 20)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	//�G�������Ă���
	if (SgEcount >= 20 && SgEcount <= 220)
	{
		// �P���C���P�ʂŕ`������鏈��
		for (EnemyFeed = 0; EnemyFeed < 150; EnemyFeed++)
		{
			if (EnemyFeed % (int)EnemyFeedOut == 0)	//�\�����鐔�l(�����邲�Ƃɏ����Ă���)
			{
				// ���胉�C��������`��\�ɂ���
				SetDrawArea(365, 195 + EnemyFeed, 515, 195 + EnemyFeed + 1);

				// �摜��`��
				DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);	//x���W��0�ɂ���΂Ԃ�Ȃ�
				EnemyFeedOut += 0.005;
			}
		}
	}
	if (SgEcount >= 430)
	{
		DarK();
	}
	if (SgEcount >= 515)
	{
		DrawGraph(0, 0, M_BackGround.BlackM, FALSE);
	}
	if (SgEcount >= 525)
	{
		DrawFormatString(300, 220, GetColor(255, 255, 0), "Thank You For Playing!");
		DrawFormatString(300, 270, GetColor(255, 255, 0), "BACK�Ń^�C�g���֖߂�܂��B");
		DrawFormatString(300, 320, GetColor(255, 255, 0), "���j���[�� LB �������Ɖ���");
	}
}