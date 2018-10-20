#include "DxLib.h"
#include "Background.h"
#include "Player.h"
#include "scenemgr.h"
#include "GameLast.h"
#include "Enemy.h"
#include "GameFirst.h"
#include "GameSecond.h"
#include "SgameFirst.h"
#include "SgameSecond.h"
#include "Sound.h"
#include "Media.h"

//���g
double PEndMoveX, PEndMoveY;	//�E�҂̓���
double PriMoveX, PriMoveY;		//�P�̓���
int enemyfeed;					//���X�{�X�������̂ɗ��p
double enemyfeedout = 1;		//�����{�X�������̂ɗ��ppart2
int SEcount;					//����
int dd = 255;					//�t�F�[�h�A�E�g�p

//�R���g���[���[
int GEpad;


//���x�������Ă���
void DOWNn()
{
	if (dd >= 0)
	{
		dd -= 3;
	}
}
void DARKk()
{
	if (dd >= 0)
	{
		SetDrawBright(dd, dd, dd);
	}
}

//���Z�b�g
void GameEnd_R()
{
	PEndMoveX = 0;
	PEndMoveY = 0;
	PriMoveX = 0;
	PriMoveY = 0;
	enemyfeed = 0;
	enemyfeedout = 1;
	SEcount = 0;
	dd = 255;
}

//�X�V
void GameEnd_Update()
{
	GEpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GEpad & PAD_INPUT_7) && (GEpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	if (GEpad & PAD_INPUT_7)   //BACK��������Ă�����
	{
		StopSoundMem(S_BGM.MenuS);
		Background_R();
		Enemy_R();
		GameEnd_R();
		GameFirst_R();
		GameLast_R();
		GameSecond_R();
		Player_R();
		SGameFirst_R();
		SGameSecond_R();
		SceneMgr_ChangeScene(eScene_Menu);
	}
	SEcount++;
	if (SEcount >= 240 && SEcount <= 365)
	{
		// ���ʂ̐ݒ�
		ChangeVolumeSoundMem(150, S_BGM.MenuS);

		// ���̍Đ�
		PlaySoundMem(S_BGM.MenuS, DX_PLAYTYPE_LOOP,FALSE);
		//��
		if (GetPlayerYpoint() == -2)
		{
			PEndMoveX += 3.92;
			PEndMoveY += 0.48;
		}

		//�^��
		else if (GetPlayerYpoint() == 0)
		{
			PEndMoveX += 3.92;
			PEndMoveY -= 0.16;
		}

		//��
		else
		{
			PEndMoveX += 3.92;
			PEndMoveY -= 0.8;
		}

	}
	if (SEcount >= 370 && SEcount <= 395)
	{
		PEndMoveY += 1.3;
	}
	if (SEcount >= 397 && SEcount <= 407)
	{
		PriMoveY += 0.5;
		//PrinMoveY=3
	}
	if (SEcount >= 409)
	{
		PEndMoveX -= 5;
		PriMoveX -= 5;
	}
	if (SEcount >= 430 && SEcount < 515)
	{
		DOWNn();
	}
	if (SEcount >= 515)
	{
		dd = 255;
	}
}

//�`��
void GameEnd_Draw()
{
	// ��ʑS�̂�`��\�ɂ���
	SetDrawArea(0, 0, 640, 480);
	LastBackground_Draw();	//�w�i�̕`��
	//���̕`��
	if (SEcount < 370)
	{
		DrawGraph((int)PEndMoveX + 100, (int)PEndMoveY + (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SEcount >= 370)
	{
		DrawTurnGraph((int)PEndMoveX + 100, (int)PEndMoveY + (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//�P�̕`��
	DrawGraph((int)PriMoveX + 590, (int)PriMoveY + 300, M_Character.PrincessM, TRUE);
	//�G�̕`��
	if (SEcount < 20)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	//�G�������Ă���
	if (SEcount >= 20 && SEcount <= 220)
	{
		// �P���C���P�ʂŕ`������鏈��
		for (enemyfeed = 0; enemyfeed < 150; enemyfeed++)
		{
			if (enemyfeed % (int)enemyfeedout == 0)	//�\�����鐔�l(�����邲�Ƃɏ����Ă���)
			{
				// ���胉�C��������`��\�ɂ���
				SetDrawArea(365, 195 + enemyfeed, 515, 195 + enemyfeed + 1);

				// �摜��`��
				DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);	//x���W��0�ɂ���΂Ԃ�Ȃ�
				enemyfeedout += 0.005;
			}
		}
	}
	if (SEcount >= 430)
	{
		DARKk();
	}
	if (SEcount >= 515)
	{
		DrawGraph(0, 0, M_BackGround.BlackM, FALSE);
	}
	if (SEcount >= 525)
	{
		DrawFormatString(300, 220, GetColor(255, 255, 0), "Thank You For Playing!");
		DrawFormatString(300, 270, GetColor(255, 255, 0), "BACK�Ń^�C�g���֖߂�܂��B");
		DrawFormatString(300, 320, GetColor(255, 255, 0), "���j���[�� LB �������Ɖ���");
	}
}