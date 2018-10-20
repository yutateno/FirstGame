#include "Dxlib.h"
#include "Scenemgr.h"
#include "Media.h"
#include "Enemy.h"

#define SCROLL_SPEED 2

//�X�V
int BGx;			//�X�N���[���̗�
int BGX = -100;		//���X�{�X�̔w�i�ւ̃X�N���[��
bool StopFlag;		//�X�N���[������߂�����
int ChengeFlag;		//�O���㔼�w�i���X���[�Y�ɕς���

//�G����̕`��
void Background_Draw()
{
	if (EnemyBreak() < 2)
	{
		//�ꖇ�ڕ`��
		DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

		//�񖇖ڕ`��
		DrawGraph(BGx + 640, 0, M_BackGround.BackGroundM, TRUE);

		//��ԉE�܂ŃX�N���[�������珉���l�ɖ߂�
		if (BGx == -640)
		{
			BGx = 0;
		}
	}
	//�G����̓r������w�i�������ς���M�~�b�N
	else
	{
		if (ChengeFlag == 0)
		{
			//�ꖇ�ڕ`��
			DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

			//�񖇖ڕ`��
			DrawGraph(BGx + 640, 0, M_BackGround.BackGroundM, TRUE);

			//��ԉE�܂ŃX�N���[�������珉���l�ɖ߂�
			if (BGx == -640)
			{
				BGx = 0;
				ChengeFlag = 1;
			}
		}
		else if (ChengeFlag == 1)
		{
			//�ꖇ�ڕ`��
			DrawGraph(BGx, 0, M_BackGround.BackGroundM, TRUE);

			//�񖇖ڕ`��
			DrawGraph(BGx + 640, 0, M_BackGround.SecondBackGroundM, TRUE);

			//��ԉE�܂ŃX�N���[�������珉���l�ɖ߂�
			if (BGx == -640)
			{
				BGx = 0;
				ChengeFlag = 2;
			}
		}
		else
		{
			//�ꖇ�ڕ`��
			DrawGraph(BGx, 0, M_BackGround.SecondBackGroundM, TRUE);

			//�񖇖ڕ`��
			DrawGraph(BGx + 640, 0, M_BackGround.SecondBackGroundM, TRUE);

			//��ԉE�܂ŃX�N���[�������珉���l�ɖ߂�
			if (BGx == -640)
			{
				BGx = 0;
			}
		}
	}
}

//���X�{�X�̕`��
void LastBackground_Draw()
{
	//�ꖇ�ڕ`��
	DrawGraph(BGX, 0, M_BackGround.SecondBackGroundM, TRUE);

	//�񖇖ڕ`��
	DrawGraph(BGX + 640, 0, M_BackGround.LastBackGroundM, TRUE);

	//��ԉE�܂ŃX�N���[�������珉���l�ɖ߂�
	if (BGX == -640)
	{
		StopFlag = 1;
	}
}

//�G����̍X�V
void Background_Update()
{
	BGx -= SCROLL_SPEED;
}

//���X�{�X�̍X�V
void LastBackground_Update()
{
	if (StopFlag != 1)
	{
		BGX -= SCROLL_SPEED;
	}
}

//���Z�b�g
void Background_R()
{
	BGx = 0;
	BGX = -100;
	StopFlag = 0;
	ChengeFlag = 0;
}