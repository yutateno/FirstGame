#include "Dxlib.h"
#include "PlayerSword.h"
#include "PlayerShield.h"
#include "Scenemgr.h"
#include "Player.h"
#include "Background.h"
#include "Sound.h"
#include "Media.h"

//�G�̍��W
#define EAXMove 1	//E(nemy)A(�G)X(���W)Move
#define EAYMove 2	//E(nemy)A(�G)Y(���W)Move
#define EASXMove 2	//E(nemy)A(�G)S(trong)X(���W)Move
#define EAXWait 30	//E(nemy)A(�G)X(���W)Wait
#define EAYWait 60	//�����ǈ꒼���ɍU������̂ōU������Y���W�͓����Ȃ̂ŕ��p
#define EASXWait 60	//E(nemy)A(�G)S(trong)X(���W)Wait
#define EAAC 200	//E(nemy)A(�G)A(ttack)C(an)
#define EAACN 250	//E(nemy)A(�G)A(ttack)C(an)N(ot)
#define EX 440		//E(nemy)X(���W)�����l
#define EY 270		//E(nemy)Y(���W)�����l
#define DCC 12		//DamegeColorCount
#define TrapNum 2	//���X�{�X�ł̃g���b�v�̌�

//�G�Ɋւ��邱��
int count;					//�v���C���[�̍U����
int Attackcount = 0;		//�U������J�E���g
int Deathcount = 250;		//���S��̊�
int Waitcount = 0;			//�U������܂ł̐��b�̊�
int EnemyX = 0;				//���[�V�����̈ړ��J�E���g(X���W
int EnemyY = 0;				//���[�V�����̈ړ��J�E���g(Y���W
int attackrand = 0;				//�U���̂��߂Ɉړ�������
int check = 7;				//�U���p�^�[���̗���������l
int AttackHit = 0;			//�G�̍U�����ʂ���
int AttackHitcount = 0;		//�G�̍U���̃J�E���g
double EnemyXM = 0;			//�G�̏o���J�E���g
int AttackMoveCount;		//�U���̓���
int EnemyArea = 0;			//0�^��,1��,2��
int EnemyLastArea = 0;		//0�^��,1��,2��
int YAttackHit = 0;			//�΋��̍U���_���[�W
int ATTACKHITcount = 0;		//���̃_���[�W�J�E���g
int YAttackHITcount = 0;	//�΋��̃_���[�W�J�E���g
int DamegeColorCount = 12;	//�_���[�W�󂯂����̐F�ς�
bool DamegeColor = 0;		//�_���[�W�󂯂�����
int ShieldMove;				//�����Ă�V�������̓���

//㩂Ɋւ��邱��
int TenmetsuCount;		//㩂̔����J�E���g
int TenmetsuDis;		//�_�łƂ��ĕ\��������
int TrapCount;			//㩂̏o������
int TenmetsuFlag;		//㩂̔����J�E���g���l�ɒB������
int TrapFlag;			//�_�ł̃J�E���g���l�ɒB������
int TrapRand;			//�ǂ���㩂��o����
int TrapX, TrapY;		//�o���g���b�v�̍��W
int WerningY = 140;		//�_�ł���J�E���g��(�g���ĂȂ�����
int ATTACKHIT;			//����㩂Ɉ�����������
int TenmetsuTime;		//�_�ł̕\�������邽�߂̎���
int TrapRandd[2];		//���X�{�X�ł̓�ڂ�㩂̗���

//�G�̊G�Ɋւ��邱��
struct Enemy_E{
	int Gr[4];	//�摜�f�[�^
	int AC;		//�U���J�E���g������J�E���g
};
int EneI = 0;	//�Y����
Enemy_E Ene[6];	//6�̂̓G�L����

//���X�{�X�G�̊֐��Ɋւ��邱��
enum AttackDraw {
	LongNormal, UpAttackCan, MiddleAttackCan, UpAttackCannot
	, MiddleAttackCannot, UnderAttackCan, UnderAttackCannot, ShortNormal
	, MiddleAttackBefore, UnderAttackBefore, UpAttackBefore, UpNormal
	, UnderNormal, UpAttackMoveUpDown, UpAttackMoveMiddle
	, UnderAttackMoveUpDown, UnderAttackMoveMiddle
};

//���̊G�̊֐��ɂ�����邱��
enum ArrowDraw {
	Strong, Up, Middle, Under
	, MagicUp, MagicMiddle, MagicUnder
};

//�ǂݍ���
void Enemy_Read()
{
	//�G�P�̖ڂ���U�̖ڂ܂�
	LoadDivGraph("media\\enemy2.png", 4, 4, 1, 60, 90, Ene[0].Gr);
	LoadDivGraph("media\\enemy3.png", 4, 4, 1, 80, 84, Ene[4].Gr);
	LoadDivGraph("media\\enemy4.png", 4, 4, 1, 80, 80, Ene[2].Gr);
	LoadDivGraph("media\\enemy5.png", 4, 4, 1, 120, 120, Ene[3].Gr);
	LoadDivGraph("media\\enemy6.png", 4, 4, 1, 80, 80, Ene[1].Gr);
	LoadDivGraph("media\\enemy6.png", 4, 4, 1, 80, 80, Ene[5].Gr);
}

//�U���J�E���g�}���֐��i�����������ōςނ悤��
void Enemy_ReadRead()
{
	//�U���J�E���g
	Ene[0].AC = 150;
	Ene[1].AC = 140;
	Ene[2].AC = 130;
	Ene[3].AC = 120;
	Ene[4].AC = 110;
	Ene[5].AC = 100;
}

//��l�p�̍X�V
void Enemy_Update()
{
	Enemy_ReadRead();

	//�G�̎�U����㩍U���Ƌ��U���Ɩ��@�U���Ɋւ��錕�Ə��̃v���C���[�̔���
	if (AttackHit == 1)	//��U����㩂ɂ���������
	{
		AttackHitcount++;
		if (AttackHitcount == 20)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (AttackHit == 2)	//���U������������
	{
		AttackHitcount++;
		if (AttackHitcount == 25)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (YAttackHit == 1)	//��ȍU���ɓ�������
	{
		YAttackHITcount++;
		if (YAttackHITcount == 20)
		{
			YAttackHit = 0;
			YAttackHITcount = 0;
		}
	}
	if (ATTACKHIT == 1)	//����㩂Ɉ�����������
	{
		AttackHit = 3;
		ATTACKHITcount++;
		if (ATTACKHITcount == 20)
		{
			ATTACKHIT = 0;
			AttackHit = 0;
			ATTACKHITcount = 0;
		}
	}
}

//��l�p�̍X�V
void ENEMY_Update()
{
	Enemy_ReadRead();

	//�G�̎�U����㩍U���Ƌ��U���Ɩ��@�U���Ɋւ���v���C���[�̔���
	if (AttackHit == 1 || AttackHit == 3)	//��U����㩂ɓ��������Ƃ�
	{
		AttackHitcount++;
		if (AttackHitcount == 20)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (AttackHit == 2)						//���U���ɓ��������Ƃ�
	{
		AttackHitcount++;
		if (AttackHitcount == 25)
		{
			AttackHit = 0;
			AttackHitcount = 0;
		}
	}
	if (YAttackHit == 1)	//��ȍU���ɓ�������
	{
		YAttackHITcount++;
		if (YAttackHITcount == 20)
		{
			YAttackHit = 0;
			YAttackHITcount = 0;
		}
	}
}

//㩂Ɋւ���i�O�Ȃ��l�v���C�A�P�Ȃ��l�v���C
void Enemy_Trap(bool gamemode, int random) 
{
	//㩂��o��������J�E���g���B������
	if (TenmetsuCount % 400 == 0 && TenmetsuCount != 0)
	{
		TrapRand = random;	//���������ďo�����W�����߂�
		TenmetsuFlag = 1;
	}
	//���̓_�ł�����
	if (TenmetsuFlag == 1)
	{
		TenmetsuDis++;
		TenmetsuTime = GetNowCount();
		if ((TenmetsuTime / 100) % 2 == 0)
		{
			
			if (TrapRand == 1 || TrapRand == 7)			//��
			{
				DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
			}
			else if (TrapRand == 2 || TrapRand == 8)	//����
			{
				DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
			}
			else if (TrapRand == 3 || TrapRand == 9)	//�E��
			{
				if (gamemode == 1)	//��l�v���C
				{
					DrawGraph(200, 170, M_Enemy.TenmetsuM, FALSE);
				}
				else				//��l�v���C
				{
					DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else if (TrapRand == 4)										//�E
			{
				if (gamemode == 1)	//��l�v���C
				{
					DrawGraph(200, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else				//��l�v���C
				{
					DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else if (TrapRand == 5)										//�E��
			{
				if (gamemode == 1)	//��l�v���C
				{
					DrawGraph(200, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else				//��l�v���C
				{
					DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
				}
			}
			else																			//���� 0,6
			{
				DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
			}
		}
	}
	//����Ȃ�̍��}������㩂��o�������������������
	if (TenmetsuDis == 100)
	{
		TenmetsuDis = 0;
		TenmetsuFlag = 0;
		TrapFlag = 1;
	}
	//㩂��o��������
	if (TrapFlag == 1)
	{
		TrapCount++;
		if (gamemode == 0)	//��l�v���C
		{
			if (TrapRand == 0 || TrapRand == 3 || TrapRand == 6 || TrapRand == 9)	//��
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 1 || TrapRand == 4 || TrapRand == 7)				//�^��
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else																	//��
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetPlayerYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
		}
		else //��l�v���C
		{
			if (TrapRand == 0 || TrapRand == 6)	//����
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 1 || TrapRand == 7)	//��
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 2 || TrapRand == 8)	//����
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRand == 3 || TrapRand == 9)	//�E��
			{
				DrawGraph(200, 170, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == -2)
				{
					ATTACKHIT = 1;
				}
			}
			else if (TrapRand == 4)										//�E
			{
				DrawGraph(200, 250, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 0)
				{
					ATTACKHIT = 1;
				}
			}
			else//�E��
			{
				DrawGraph(200, 330, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 2)
				{
					ATTACKHIT = 1;
				}
			}
		}
	}
	//㩂̏o�����I��点��
	if (TrapCount == 55)
	{
		TrapCount = 0;
		TrapFlag = 0;
	}
}

//��l�v���C�̃��X�{�X�ł�㩓�p
void Enemy_TrapSecond(int random)
{
	//㩂��o��������J�E���g���B������
	if (TenmetsuCount % 400 == 0 && TenmetsuCount != 0)
	{
		TrapRandd[0] = random;	//���������ďo�����W�����߂�
		//��ڂ��
		if (random <= 2)
		{
			TrapRandd[1] = (random + 2);
		}
		if (random > 2)
		{
			TrapRandd[1] = (random - 3);
		}
		TenmetsuFlag = 1;
	}
	//���̓_�ł�����
	if (TenmetsuFlag == 1)
	{
		TenmetsuDis++;
		TenmetsuTime = GetNowCount();
		if ((TenmetsuTime / 100) % 2 == 0)
		{
			for (int i = 0; i < TrapNum; i++)
			{
				if (TrapRandd[i] == 1 || TrapRandd[i] == 7)			//��
				{
					DrawGraph(100, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 2 || TrapRandd[i] == 8)	//����
				{
					DrawGraph(100, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 3 || TrapRandd[i] == 9)	//�E��
				{
					DrawGraph(200, 170, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 4)										//�E
				{
					DrawGraph(200, 250, M_Enemy.TenmetsuM, FALSE);
				}
				else if (TrapRandd[i] == 5)										//�E��
				{
					DrawGraph(200, 330, M_Enemy.TenmetsuM, FALSE);
				}
				else																			//���� 0,6
				{
					DrawGraph(100, 170, M_Enemy.TenmetsuM, FALSE);
				}
			}
		}
	}
	//����Ȃ�̍��}������㩂��o�������������������
	if (TenmetsuDis == 100)
	{
		TenmetsuDis = 0;
		TenmetsuFlag = 0;
		TrapFlag = 1;
	}
	//㩂��o��������
	if (TrapFlag == 1)
	{
		TrapCount++;
		for (int i = 0; i < TrapNum; i++)
		{
			if (TrapRandd[i] == 0 || TrapRandd[i] == 6)	//����
			{
				DrawGraph(100, 170, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == -2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 1 || TrapRandd[i] == 7)	//��
			{
				DrawGraph(100, 250, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 0)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 2 || TrapRandd[i] == 8)	//����
			{
				DrawGraph(100, 330, M_Enemy.TrapM, TRUE);
				if (GetswordYpoint() == 2)
				{
					AttackHit = 1;
				}
			}
			else if (TrapRandd[i] == 3 || TrapRandd[i] == 9)	//�E��
			{
				DrawGraph(200, 170, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == -2)
				{
					ATTACKHIT = 1;
				}
			}
			else if (TrapRandd[i] == 4)										//�E
			{
				DrawGraph(200, 250, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 0)
				{
					ATTACKHIT = 1;
				}
			}
			else//�E��
			{
				DrawGraph(200, 330, M_Enemy.TrapM, TRUE);
				if (Getshieldpoint() == 2)
				{
					ATTACKHIT = 1;
				}
			}
		}
	}
	//㩂̏o�����I��点��
	if (TrapCount == 55)
	{
		TrapCount = 0;
		TrapFlag = 0;
	}
}

//���X�{�X�G�̊G
void Enemy_AttackDraw(AttackDraw AttackD)
{
	switch (AttackD) {
	case LongNormal:	//�ʏ펞
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;
		
	case UpAttackCan:	//�E�ォ��v���C���[�ɍU���\��
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackCan:	//�^�񒆂���v���C���[�ɍU���\��
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackCannot:	//�E�ォ��v���C���[�ɍU���ł��Ȃ��Ƃ��i���̗E�҂ɂ���Č��̗E�҂��Ŏ���Ă���Ȃǂ̂Ƃ�
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackCannot:	//�^�񒆂���v���C���[�ɍU���ł��Ȃ��Ƃ�
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackCan:	//�E������v���C���[�ɍU���\��
		if (count < 30)
		{
			DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackCannot:	//�E������v���C���[�ɍU���ł��Ȃ��Ƃ�
		if (count < 30)
		{
			DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case ShortNormal:	//�ʏ펞
		if (count < 30)
		{
			DrawRotaGraph(EX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case MiddleAttackBefore:	//�^�񒆂���U�����O��
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackBefore:	//�E������U�����O��
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackBefore:	//�E�ォ��U�����O��
		if (count < 30)
		{
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpNormal:	//�E��ɂ���ʏ펞
		if (count < 30)
		{
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderNormal:	//�E���ɂ���ʏ펞
		if (count < 30)
		{
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackMoveUpDown:	//�E��ł̍U�������̃��[�u���i�㉺�ɓ���
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UpAttackMoveMiddle:	//�E��ł̍U�������̃��[�u���i���̂ݓ���
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackMoveUpDown:	//�E���ł̍U�������̃��[�u���i�㉺�ɓ���
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	case UnderAttackMoveMiddle:	//�E���ł̍U�������̃��[�u���i���̂ݓ���
		if (count < 30)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		}
		else
		{
			SetDrawBright(255, 0, 255);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		break;

	default:
		break;
	}
}

//���̊G
void Enemy_ArrowDraw(ArrowDraw ArrowD)
{
	switch (ArrowD) {
		
	case Strong:	//���U��
		DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
		break;

	case Up:	//�E��U��
		DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		break;

	case Middle:	//�^�񒆍U��
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		break;

	case Under:	//�E���U��
		DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
		DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
		break;

	case MagicUp:	//�E�㖂�@�U��
		DrawGraph(300, 170, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	case MagicMiddle:	//�^�񒆖��@�U��
		DrawGraph(300, 250, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	case MagicUnder:	//�E�����@�U��
		DrawGraph(300, 330, M_Enemy.AttackYellowArrowM, TRUE);
		break;

	default:
		break;
	}
}

//��l�v���C�̍U���p�^�[��
void Enemy_AttackRand()
{
	if (attackrand == 1)
	{
		Enemy_ArrowDraw(Strong);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
		Waitcount++;
		if (Waitcount >= 45)
		{
			AttackMoveCount++;
			EnemyX -= 50;
			if (AttackMoveCount == 5)
			{
				if (GetswordYpoint() == Getshieldpoint()) //����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //����Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				else //�����Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[2], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 0;
			}
		}
	}
	else if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)) //��Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if ((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2)) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else													//����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 1;
			}
		}
	}
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == Getshieldpoint())
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2)))	//����Ă���ꍇ
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//����Ă��Ȃ��ꍇ
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 0;
			}
		}
	}
	else if (attackrand == 4)
	{
		//�G�̍U���̃`���[�g���A���I��
		Enemy_ArrowDraw(Under);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == -2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else //����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 2;
			}
		}
	}
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& GetShieldMahouFlag() == 0 && ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ��Ŗ��@���ˉ\
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 11;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10
				&& GetShieldMahouFlag() == 1 && ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2))) //��ŏ������Ȃ��Ƃ��i���ƌ����S�������ł͂Ȃ��̂�'��'�ł܂Ƃ߂��Ȃ�
			{
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2))) //��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 1;
			}
		}
	}
	else if (attackrand == 11)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 170, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyArea = 1;
		}
	}
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 12;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)))	//�������Ȃ��Ƃ�
			{
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)))	//��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 12)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 250, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
		}
	}
	else if (attackrand == 7)
	{
		//�G�̍U���̃`���[�g���A���I��
		Enemy_ArrowDraw(MagicUnder);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(250, 330, M_Enemy.YellowAttackM, TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 13;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)))		//�������Ȃ��Ƃ�
			{
				DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);

			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyArea = 2;
			}
		}
	}
	else if (attackrand == 13)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 330, M_Enemy.YellowAttackM, TRUE);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[1], TRUE);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyArea = 2;
		}
	}
	//�K���Ȓl�ɓ����`�Ő��䂵�Ă�̂�else�͓���Ȃ�
}

//��l�v���C�̃��X�{�X�̍U���p�^�[��
void LastEnemy_AttackRand()
{
	if (attackrand == 1)
	{
		Enemy_ArrowDraw(Strong);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 45)
		{
			AttackMoveCount++;
			EnemyX -= 50;
			if (AttackMoveCount == 5)
			{
				if (GetswordYpoint() == Getshieldpoint()) //����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //����Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else //�����Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 2;
					Enemy_AttackDraw(UnderAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 0;
			}
		}
	}
	else if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2))) //��Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == -2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == 2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else //����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(UpAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 1;
			}
		}
	}
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == Getshieldpoint())
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == -2 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2)))	//����Ă���ꍇ
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else	//����Ă��Ȃ��ꍇ
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 0;
			}
		}
	}
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (((GetswordYpoint() == 2 && Getshieldpoint() == 0)
					|| (GetswordYpoint() == 2 && Getshieldpoint() == -2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(UnderAttackCan);
				}
				else if (((GetswordYpoint() == 0 && Getshieldpoint() == 2)
					|| (GetswordYpoint() == 0 && Getshieldpoint() == -2))) //���Ŏ���Ă��Ȃ��Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else  //����Ă���Ƃ�
				{
					PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
					Enemy_AttackDraw(UnderAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 2;
			}
		}
	}
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UpAttackBefore);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ�
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 11;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��ŏ�������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2))) //��ŏ������Ȃ��Ƃ�
			{
				DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(UpAttackBefore);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2))) //��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(UpNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(UpNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 1;
			}
		}
	}
	else if (attackrand == 11)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(UpAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 170, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyLastArea = 1;
		}
	}
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(MiddleAttackBefore);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				DrawGraph(250, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 12;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)))	//��������Ƃ�
			{
				ATTACKHIT = 1;
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)))	//�������Ȃ��Ƃ�
			{
				DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
				Enemy_AttackDraw(MiddleAttackBefore);
			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)))	//��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 12)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(MiddleAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 250, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
		}
	}
	else if (attackrand == 7)
	{
		Enemy_ArrowDraw(MagicUnder);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UnderAttackBefore);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount >= 3 && AttackMoveCount < 10 && GetShieldMahouFlag() == 0
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				ATTACKHIT = 1;
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount == 3 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, TRUE);
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(250, 330, M_Enemy.YellowAttackM, TRUE);
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 13;
			}
			else if (AttackMoveCount >= 4 && AttackMoveCount < 10 && GetShieldMahouFlag() == 1
				&& ((Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == -2))) //��������Ƃ�
			{
				ATTACKHIT = 1;
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			}
			else if (AttackMoveCount >= 3 && AttackMoveCount < 10
				&& (GetShieldMahouFlag() == 0 || GetShieldMahouFlag() == 1)
				&& ((Getshieldpoint() == -2 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == -2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 0)
					|| (Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //�������Ȃ��Ƃ�
			{
				Enemy_AttackDraw(UnderAttackBefore);
				DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);

			}
			if (AttackMoveCount == 10)
			{
				if (((Getshieldpoint() == 0 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == 2 && GetswordYpoint() == 2)
					|| (Getshieldpoint() == -2 && GetswordYpoint() == 2))) //��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(UnderNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(UnderNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
				EnemyLastArea = 2;
			}
		}
	}
	else if (attackrand == 13)
	{
		EnemyX += 10;
		DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
		Enemy_AttackDraw(UnderAttackBefore);
		if (EnemyX >= 40)
		{
			DrawTurnGraph(250 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
		}
		if (EnemyX == 80)
		{
			PlaySoundMem(S_SE.BonS, DX_PLAYTYPE_BACK, TRUE);
			DrawTurnGraph(330, 330, M_Enemy.YellowAttackM, TRUE);
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			count++;
			EnemyX = 0;
			attackrand = 0;
			EnemyLastArea = 2;
		}
	}
	//�����ɂȂ����������S��Ǝ��S�O�Ő��䂵�Ă���̂�else�͂Ȃ��ł��B
}

//��l�v���C�̃v���C���[�̕`��
void Enemy_Draw()
{
	// ���ʂ̐ݒ�
	ChangeVolumeSoundMem(255, S_BGM.ZakoS);
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);

	//�U���p�^�[���̗���
	int random = GetRand(9);

	Enemy_Trap(1, random);

	//��̖ڂɊւ��邱��
	//�ʏ펞
	if (count < 10)
	{
		if (EneI == 0)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "����@�U���F�a / �ړ��F���X�e�B�b�N�㉺");
		}
		if (EneI >= 2)	//3�̖ڂ���o��
		{
			TenmetsuCount++;	//㩂��o��������J�E���g
		}
		if (EneI == 3)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "����@���@���ˁF�a (�N�[���_�E���ɋC��t����)");
		}
		//�̗�
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPM, FALSE);
		DrawRectGraph(380, 120, (12 * count) - 120, 0, 120, 20, M_Enemy.DAMAGEM, FALSE, FALSE);
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			Deathcount = 250;	//���S��̊�
			check = random;		//�U�������̈ʒu�p�^�[��
			Attackcount++;
		}
	}
	if (EnemyArea == 0)	//�U���ł̓G�̈ʒu�^��---------------------------------------------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//�ʏ펞
		{
			DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 0 && EnemyArea == 0)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//��̖ڍU���p�^�[��(30�{�ɗ�����U��������)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//�����U���p�^�[��
			if (check == 6 || check == 9)
			{
				Enemy_ArrowDraw(Strong);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 3 || check == 7)
			{
				Enemy_ArrowDraw(Up);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else if (check == 1 || check == 4 || check == 8)
			{
				Enemy_ArrowDraw(Middle);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else
			{
				Enemy_ArrowDraw(Under);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//�����U���p�^�[��
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else if (check == 1 || check == 4)
			{
				Enemy_ArrowDraw(Middle);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else if (check == 2 || check == 5)
			{
				Enemy_ArrowDraw(Under);//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else if (check == 7)	//���@�U����
			{
				Enemy_ArrowDraw(MagicUp);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
			else if (check == 8)	//���@�U���^��
			{
				Enemy_ArrowDraw(MagicMiddle);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 6;
				}
			}
			else					//���@�U����
			{
				Enemy_ArrowDraw(MagicUnder);	//�G�̍U���̃`���[�g���A���I��
				DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		Enemy_AttackRand();	//�U���p�^�[��
	}
	else if (EnemyArea == 1)	//�U���Ɋւ��邱�ƁA��ɂ���-------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//�ʏ펞
		{
			DrawRotaGraph(EX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == -2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//��̖ڍU���p�^�[��(30�{�ɗ�����U��������)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//�����U���p�^�[��
			if (check == 6 || check == 7)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 2 || check == 3 || check == 8)
			{
				Enemy_ArrowDraw(Up);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//�����U���p�^�[��
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 2 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else if (check == 1 || check == 4 || check == 5)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//��ȏ�
			else
			{
				Enemy_ArrowDraw(MagicUp);
				DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
		}
		Enemy_AttackRand();	//�U���p�^�[��
	}
	else if (EnemyArea == 2)	//�U���Ɋւ��邱�ƁA���ɂ���-----------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 10)//�ʏ펞
		{
			DrawRotaGraph(EX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 10 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
		}
		if (DamegeColor == 1)
		{
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//��̖ڍU���p�^�[��(30�{�ɗ�����U��������)
		if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
		{
			//�����U���p�^�[��
			if (check == 6 || check == 7)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E��
			else if (check == 0 || check == 1 || check == 4 || check == 8)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else
			{
				Enemy_ArrowDraw(Under);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
		}
		else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
		{
			//�����U���p�^�[��
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E��
			else if (check == 0 || check == 1 || check == 4)
			{
				Enemy_ArrowDraw(Middle);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else if (check == 2 || check == 3 || check == 5)
			{
				Enemy_ArrowDraw(Under);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else	//��ȉ�
			{
				Enemy_ArrowDraw(MagicUnder);
				DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[0], TRUE);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		Enemy_AttackRand();	//�U���p�^�[��
	}
	//���̖ڂɊւ��邱��
	//���̖ڂ̏o���ɏ����Ԃ����
	if (count == 10 && EneI != 6)
	{
		PlaySoundMem(S_SE.NextenemyS, DX_PLAYTYPE_BACK, FALSE);
		Deathcount--;
		Attackcount = 0;
		EnemyArea = 3;	//����
		TenmetsuCount = 0;
		Background_Update();	//�w�i�𓮂���
		if (EneI == 1)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "㩂̗\���B�_�ł��ڈ󂾁I");
		}
		//����ɓ����O
		if (EneI == 2 && Deathcount > 125)
		{
			ShieldMove -= 2;
			DrawRotaGraph(EX + ShieldMove, EY, 1, 0, M_Enemy.ShieldM, TRUE);
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "���F�ɂ͋C��t���āI���ʂ̏��ł͖h���Ȃ��I");
		}
		//����ɓ��ꂽ��
		if (EneI == 2 && Deathcount <= 125)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "���@�𔽎˂���d��������ɓ��ꂽ�I�����ˁH");
		}
		//�J�[������񂾂���
		if (EneI == 2 && Deathcount <= 125 && Deathcount > 85)
		{
			PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, FALSE);
		}
		if (Deathcount <= 100)
		{
			DrawRotaGraph(640 - (int)EnemyXM, EY, 1, 0, Ene[EneI + 1].Gr[0], TRUE);
			EnemyXM += 2;
		}
	}
	//���̖ځ`
	if (Deathcount <= 0 && count >= 10 && count < 20)
	{
		Deathcount = 0;
		count = 0;
		EnemyXM = 0;
		DamegeColor = 0;
		DamegeColorCount = DCC;
		EneI++;
		EnemyArea = 0;
	}
	if (GetTwoBGMStop() == 1)
	{
		StopSoundMem(S_BGM.ZakoS);
	}
	if (EneI == 5 && count == 10)
	{
		DamegeColor = 0;
		DamegeColorCount = DCC;
		TenmetsuCount = 0;
		StopSoundMem(S_BGM.ZakoS);
		SceneMgr_ChangeScene(eScene_SgameSecond);	//�{�X�O�V�[��
	}
}

//��l�p�̕`��
void ENEMY_Draw()
{
	// ���ʂ̐ݒ�
	ChangeVolumeSoundMem(255, S_BGM.ZakoS);
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);

	//�U���p�^�[���̗���
	int random = GetRand(9);

	Enemy_Trap(0, random);

	//��̖ڂɊւ��邱��
	//�ʏ펞
	if (count < 10)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			Deathcount = 250;	//���S��̊�
			check = random;
			Attackcount++;
			DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
		}
		if (EneI == 0)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "����@�U���F�a / �ړ��F���X�e�B�b�N�㉺");
		}
		if (EneI >= 2)
		{
			TenmetsuCount++;	//㩂��o��������J�E���g
		}
		if (EneI == 3)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(0, 0, 0), "����@�U�����x�㏸�F�`�i�ŏ��������߂Ƃ�����j");
		}
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPM, FALSE);
		DrawRectGraph(380, 120, (12 * count) - 120, 0, 120, 20, M_Enemy.DAMAGEM, FALSE, FALSE);
	}
	//�U�����͂�����_���[�W�̊G�ɕς���
	if (count < 10 && attackrand == 0 && EnemyX == 0 && GetPlayerYpoint() == 0)
	{
		if (GetRabitUseFlag() == 0 && GetPattack() == 23)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		if (GetRabitUseFlag() == 1 && GetPattack() == 12)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
	}
	if (DamegeColor == 1 && count != 10)
	{
		DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[1], TRUE);
		DamegeColorCount--;
		if (DamegeColorCount <= 0)
		{
			DamegeColorCount = DCC;
			DamegeColor = 0;
		}
	}
	//��̖ڍU���p�^�[��(30�{�ɗ�����U��������)
	if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && EneI != 3 && EneI != 4)
	{
		//�ʏ�̍U��
		//�E���
		if (check == 0 || check == 3 || check == 6 || check == 9)
		{
			Enemy_ArrowDraw(Up);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//�E��
		else if (check == 1 || check == 4 || check == 7)
		{
			Enemy_ArrowDraw(Middle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//�E����
		else
		{
			Enemy_ArrowDraw(Under);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
	}
	else if ((Attackcount % Ene[EneI].AC) == 0 && count < 10 && (EneI == 3 || EneI == 4))
	{
		//�ʏ�̍U��
		//�E���
		if (check == 0 || check == 3 || check == 6)
		{
			Enemy_ArrowDraw(Up);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//�E��
		else if (check == 1 || check == 4)
		{
			Enemy_ArrowDraw(Middle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//�E����
		else if (check == 2 || check == 5)
		{
			Enemy_ArrowDraw(Under);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
		else if (check == 7)	//��ȍU����
		{
			Enemy_ArrowDraw(MagicUp);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 5;
			}
		}
		else if (check == 8)	//��ȍU���^��
		{
			Enemy_ArrowDraw(MagicMiddle);
			DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 6;
			}
		}
		else	//��ȍU����
		{
			Enemy_ArrowDraw(MagicUnder);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, Ene[EneI].Gr[0], TRUE);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 7;
			}
		}
	}
	//�ʏ�̍U���̏������I������
	//����ƍ�
	if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //��ɂ����Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (GetPlayerYpoint() == 0) //���ɂ����Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//���ɂ����Ƃ�
				{
					DrawRotaGraph(EAACN, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//��
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetPlayerYpoint() == -2 || GetPlayerYpoint() == 2))	//���ȊO�ɂ����Ƃ�
				{
					DrawRotaGraph(EAACN, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else	//���ɂ����Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				attackrand = 0;
			}
		}
	}
	//���ƍ���
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //���ɂ����Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else if (GetPlayerYpoint() == 0) //���ɂ����Ƃ�
				{
					AttackHit = 1;
					DrawRotaGraph(EAAC, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				else //��ɂ����Ƃ�
				{
					DrawRotaGraph(EAACN, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 5)	//��ȏ�
	{
		Enemy_ArrowDraw(MagicUp);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 6)//��Ȑ^��
	{
		Enemy_ArrowDraw(MagicMiddle);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 0)	//��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	else if (attackrand == 7)	//��ȉ�
	{
		Enemy_ArrowDraw(MagicUnder);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, Ene[EneI].Gr[3], TRUE);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //��������Ƃ�
				{
					YAttackHit = 1;
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					DrawRotaGraph(EX, EY, 1, 0, Ene[EneI].Gr[3], TRUE);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//���̖ڂɊւ��邱��
	//���̖ڂ̏o���ɏ����Ԃ����
	if (count == 10 && EneI != 6)
	{
		PlaySoundMem(S_SE.NextenemyS, DX_PLAYTYPE_BACK, FALSE);
		Deathcount--;
		TenmetsuCount = 0;
		Attackcount = 0;
		Background_Update();	//�w�i�𓮂���
		if (EneI == 1)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "㩂̗\���B�_�ł��ڈ󂾁I");
		}
		//����ɓ����O
		if (EneI == 2 && Deathcount > 125)
		{
			ShieldMove -= 2;
			DrawRotaGraph(EX + ShieldMove, EY, 1, 0, M_Character.SwordM, TRUE);	//ShieldMove�͓�l�p�̏��𗬗p��������
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "���F�����͖{�C�Ŕ�����I");
		}
		//����ɓ��ꂽ��
		if (EneI == 2 && Deathcount <= 125)
		{
			DrawGraph(0, 410, M_Enemy.HelpM, FALSE);
			DrawFormatString(10, 430, GetColor(127, 0, 127), "�V���Ȍ�����ɓ��ꂽ�I�Ȃɂ��Q�[�W�����Ă���");
			DrawBox(79, 119, 201, 141, GetColor(255, 255, 255), FALSE);
			DrawGraph(80, 120, M_Character.PAttackBarM, FALSE);
		}
		//�J�[������񂾂���
		if (EneI == 2 && Deathcount <= 125 && Deathcount > 85)
		{
			PlaySoundMem(S_SE.ShieldS, DX_PLAYTYPE_BACK, FALSE);
		}
		if (Deathcount <= 100)
		{
			DrawRotaGraph(640 - (int)EnemyXM, EY, 1, 0, Ene[EneI + 1].Gr[0], TRUE);
			EnemyXM += 2;
		}
	}
	//���̖ځ`
	if (Deathcount <= 0 && count >= 10 && count < 20)
	{
		Deathcount = 0;
		count = 0;
		EnemyXM = 0;
		DamegeColorCount = DCC;
		DamegeColor = 0;
		TenmetsuCount = 0;
		EneI++;
	}
	if (GetOneBGMStop() == 1)
	{
		StopSoundMem(S_BGM.ZakoS);
	}
	if (EneI == 5 && count == 10)
	{
		DamegeColorCount = DCC;
		DamegeColor = 0;
		TenmetsuCount = 0;
		StopSoundMem(S_BGM.ZakoS);
		SceneMgr_ChangeScene(eScene_GameSecond);	//�{�X�O�V�[��
	}
}

//��l�p�̃��X�{�X�̕`��
void LastEnemy_Draw()
{
	PlaySoundMem(S_BGM.BossS, DX_PLAYTYPE_LOOP, FALSE);

	//�U���p�^�[���̗���
	int random = GetRand(9);

	Enemy_TrapSecond(random);
	
	//�ʏ펞
	if (count < 50)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			check = random;
			Attackcount++;
		}
		TenmetsuCount++;	//㩂��o��������J�E���g
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
		//�̗̓o�[
		if (count <= 30)
		{
			DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
			DrawRectGraph(380, 120, (6 * (count-10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
		else
		{
			DrawGraph(380, 120, M_Enemy.DAMAGEM, FALSE);
			DrawRectGraph(380, 120, (6 * (count-10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
	}
	if (EnemyLastArea == 0)	//�^��---------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(ShortNormal);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 0)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//�U���p�^�[��
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//�����U���p�^�[��
			if (check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EASXMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else if (check == 1)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else if (check == 2 || check == 5)
			{
				Enemy_ArrowDraw(Under);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			else if (check == 7)	//��ȏ�
			{
				Enemy_ArrowDraw(MagicUp);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
			else if (check == 4 || check == 8)	//��Ȑ^��
			{
				Enemy_ArrowDraw(MagicMiddle);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 6;
				}
			}
			else		//��ȉ�
			{
				Enemy_ArrowDraw(MagicUnder);
				Enemy_AttackDraw(LongNormal);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		LastEnemy_AttackRand();	//�U���p�^�[��
	}
	else if (EnemyLastArea == 1)	//��-------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(UpNormal);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == -2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY - EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//�U���p�^�[��
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//�����U���p�^�[��
			if (check == 2 || check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(UpAttackMoveUpDown);
				EnemyX += EASXMove;
				EnemyY += EAYMove;
				if (EnemyX == EASXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E���
			else if (check == 0 || check == 3)
			{
				Enemy_ArrowDraw(Up);
				Enemy_AttackDraw(UpAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 2;
				}
			}
			//�E��
			else if (check == 1 || check == 5)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(UpAttackMoveUpDown);
				EnemyX += EAXMove;
				EnemyY += EAYMove;
				if (EnemyX == EAXWait || EnemyY == EAYWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//��ȏ�
			else
			{
				Enemy_ArrowDraw(MagicUp);
				Enemy_AttackDraw(UpAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 5;
				}
			}
		}
		LastEnemy_AttackRand();	//�U���p�^�[��
	}
	if (EnemyLastArea == 2)	//��-------------------------------------------
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0 && count < 50)
		{
			Enemy_AttackDraw(UnderNormal);
		}
		//�U�����͂�����_���[�W�̊G�ɕς���
		if (count < 50 && attackrand == 0 && EnemyX == 0 && GetswordYpoint() == 2)
		{
			if (GetPSattack() == 20)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
			}
			else if (GetPSattack() == 20 && count == 29)
			{
				PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
				DamegeColor = 1;
				count++;
				attackrand = 15;
			}
		}
		if (DamegeColor == 1)
		{
			SetDrawBright(125, 0, 0);
			DrawRotaGraph(EX + EnemyX, EY + EAYWait + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
			SetDrawBright(255, 255, 255);
			DamegeColorCount--;
			if (DamegeColorCount <= 0)
			{
				DamegeColorCount = DCC;
				DamegeColor = 0;
			}
		}
		//�U���p�^�[��
		if ((Attackcount % 90) == 0 && count < 50)
		{
			//�����U���p�^�[��
			if (check == 1 || check == 6)
			{
				Enemy_ArrowDraw(Strong);
				Enemy_AttackDraw(UnderAttackMoveUpDown);
				EnemyX += EASXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EASXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 1;
				}
			}
			//�ʏ�̍U��
			//�E��
			else if (check == 0 || check == 4)
			{
				Enemy_ArrowDraw(Middle);
				Enemy_AttackDraw(UnderAttackMoveUpDown);
				EnemyX += EAXMove;
				EnemyY -= EAYMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 3;
				}
			}
			//�E����
			else if (check == 2  || check == 5)
			{
				Enemy_ArrowDraw(Under);
				Enemy_AttackDraw(UnderAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 4;
				}
			}
			//��ȉ�
			else
			{
				Enemy_ArrowDraw(MagicUnder);
				Enemy_AttackDraw(UnderAttackMoveMiddle);
				EnemyX += EAXMove;
				if (EnemyX == EAXWait)
				{
					Attackcount = Attackcount + 1;
					check = 10;
					attackrand = 7;
				}
			}
		}
		LastEnemy_AttackRand();	//�U���p�^�[��
	}
	if (GetTwoBGMStop() == 1)
	{
		StopSoundMem(S_BGM.BossS);
	}
	if (count >= 50)
	{
		StopSoundMem(S_BGM.BossS);
		SceneMgr_ChangeScene(eScene_SgameEnd);	//end
	}
}

//��l�p�̃��X�{�X�̕`��
void LastENEMY_Draw()
{
	PlaySoundMem(S_BGM.BossS, DX_PLAYTYPE_LOOP, FALSE);

	//�U���p�^�[���̗���
	int random = GetRand(9);

	Enemy_Trap(0, random);

	//�ʏ펞
	if (count < 50)
	{
		if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
		{
			check = random;
			Attackcount++;
		}
		TenmetsuCount++;	//㩂��o��������J�E���g
		DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
		DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
		//�̗̓o�[
		if (count <= 30)
		{
			DrawGraph(380, 120, M_Enemy.HPsecondM, FALSE);
			DrawRectGraph(380, 120, (6 * (count - 10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
		else
		{
			DrawGraph(380, 120, M_Enemy.DAMAGEM, FALSE);
			DrawRectGraph(380, 120, (6 * (count - 10)) - 120, 0, 120, 20, M_Enemy.HPM, FALSE, FALSE);
		}
	}
	if (attackrand == 0 && Waitcount == 0 && EnemyX == 0 && EnemyY == 0)
	{
		Enemy_AttackDraw(LongNormal);
	}
	//�U�����͂�����_���[�W�̊G�ɕς���
	if (count < 50 && attackrand == 0 && EnemyX == 0 && GetPlayerYpoint() == 0)
	{
		if (GetLastRabitUseFlag() == 0 && GetPattack() == 23)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		else if (GetLastRabitUseFlag() == 0 && GetPattack() == 23 && count == 29)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
			attackrand = 15;
		}
		if (GetLastRabitUseFlag() == 1 && GetPattack() == 12)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
		}
		else if (GetLastRabitUseFlag() == 1 && GetPattack() == 12 && count == 29)
		{
			PlaySoundMem(S_SE.AttackS, DX_PLAYTYPE_BACK, TRUE);
			DamegeColor = 1;
			count++;
			attackrand = 15;
		}
	}
	if (DamegeColor == 1)
	{
		SetDrawBright(125, 0, 0);
		DrawRotaGraph(EX + EnemyX, EY + EnemyY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		DamegeColorCount--;
		if (DamegeColorCount <= 0)
		{
			DamegeColorCount = DCC;
			DamegeColor = 0;
		}
	}
		//��̖ڍU���p�^�[��(30�{�ɗ�����U��������)
	if ((Attackcount % 95) == 0 && count < 50)
	{
		//�ʏ�̍U��
		//�E���
		if (check == 0 || check == 6 || check == 8)
		{
			Enemy_ArrowDraw(Up);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 2;
			}
		}
		//�E��
		else if (check == 1 || check == 4)
		{
			Enemy_ArrowDraw(Middle);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 3;
			}
		}
		//�E����
		else if (check == 2 || check == 5)
		{
			Enemy_ArrowDraw(Under);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 4;
			}
		}
		//��ȏ�
		else if (check == 7)
		{
			Enemy_ArrowDraw(MagicUp);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY -= EAYMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 5;
			}
		}
		//��Ȑ^��
		else if (check == 3)
		{
			Enemy_ArrowDraw(MagicMiddle);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			if (EnemyX == EAXWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 6;
			}
		}
		//��ȉ�
		else
		{
			Enemy_ArrowDraw(MagicUnder);
			Enemy_AttackDraw(LongNormal);
			EnemyX += EAXMove;
			EnemyY += EAYMove;
			if (EnemyX == EAXWait || EnemyY == EAYWait)
			{
				Attackcount = Attackcount + 1;
				check = 10;
				attackrand = 7;
			}
		}
	}
		//�ʏ�̍U���̏������I������
		//����ƍ�
	if (attackrand == 2)
	{
		Enemy_ArrowDraw(Up);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //��ɂ���Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(UpAttackCan);
				}
				else if (GetPlayerYpoint() == 0) //���ɂ���Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else	//���ɂ���Ƃ�
				{
					Enemy_AttackDraw(UpAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
		//��
	else if (attackrand == 3)
	{
		Enemy_ArrowDraw(Middle);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if ((GetPlayerYpoint() == -2 || GetPlayerYpoint() == 2))	//���ȊO�ɂ���
				{
					Enemy_AttackDraw(MiddleAttackCannot);
				}
				else	//���ɂ���Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				attackrand = 0;
			}
		}
	}
		//���ƍ���
	else if (attackrand == 4)
	{
		Enemy_ArrowDraw(Under);
		SetDrawBright(0, 255, 255);
		DrawRotaGraph(EX + EnemyX, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			EnemyX -= 22;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //���ɂ���Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(UnderAttackCan);
				}
				else if (GetPlayerYpoint() == 0) //���ɂ���Ƃ�
				{
					AttackHit = 1;
					Enemy_AttackDraw(MiddleAttackCan);
				}
				else  //��ɂ���Ƃ�
				{
					Enemy_AttackDraw(UnderAttackCannot);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//��ȏ�
	else if (attackrand == 5)
	{
		Enemy_ArrowDraw(MagicUp);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY - EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UpAttackBefore);
			DrawGraph(330 + EnemyX, 170, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == -2) //��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 170, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//��Ȑ^��
	else if (attackrand == 6)
	{
		Enemy_ArrowDraw(MagicMiddle);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(MiddleAttackBefore);
			DrawGraph(330 + EnemyX, 250, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 0)	//��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 250, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	//��ȉ�
	else if (attackrand == 7)
	{
		Enemy_ArrowDraw(MagicUnder);
		SetDrawBright(255, 255, 0);
		DrawRotaGraph(EX + EAXWait, EY + EAYWait, 1, 0, M_Enemy.LastEnemyM, TRUE);
		SetDrawBright(255, 255, 255);
		Waitcount++;
		if (Waitcount >= 35)
		{
			AttackMoveCount++;
			Enemy_AttackDraw(UnderAttackBefore);
			DrawGraph(330 + EnemyX, 330, M_Enemy.YellowAttackM, TRUE);
			EnemyX -= 26;
			if (AttackMoveCount == 10)
			{
				if (GetPlayerYpoint() == 2) //��������Ƃ�
				{
					YAttackHit = 1;
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				else	//���Ȃ��Ƃ�
				{
					Enemy_AttackDraw(ShortNormal);
					DrawGraph(100, 330, M_Enemy.YellowAttackM, TRUE);
				}
				Waitcount = 0;
				AttackMoveCount = 0;
				EnemyX = 0;
				EnemyY = 0;
				attackrand = 0;
			}
		}
	}
	if (GetOneBGMStop() == 1)
	{
		StopSoundMem(S_BGM.BossS);
	}
	if (count >= 50)
	{
		StopSoundMem(S_BGM.BossS);
		SceneMgr_ChangeScene(eScene_GameEnd);	//end
	}
}

//���Z�b�g
void Enemy_R()
{
	ShieldMove = 0;
	count = 0;
	Attackcount = 0;
	Deathcount = 250;
	Waitcount = 0;
	EnemyX = 0;
	EnemyY = 0;
	attackrand = 0;
	check = 7;
	AttackHit = 0;
	AttackHitcount = 0;
	EnemyXM = 0;
	AttackMoveCount = 0;
	EnemyArea = 0;
	EnemyLastArea = 0;
	YAttackHit = 0;
	ATTACKHITcount = 0;
	YAttackHITcount = 0;
	TenmetsuCount = 0;
	TenmetsuDis = 0;
	TrapCount = 0;
	TenmetsuFlag = 0;
	TrapFlag = 0;
	TrapRand = 0;
	TrapX = 0;
	TrapY = 0;
	WerningY = 140;
	ATTACKHIT = 0;
	TenmetsuTime = 0;
	EneI = 0;
	DamegeColorCount = 12;
	DamegeColor = 0;
	for (int i = 0; i < TrapNum; i++)
	{
		TrapRandd[i] = 0;
	}
}

//�U�����ʂ���
int EAttackHit(void)
{
	return AttackHit;
}

//���̖ڂ�
int EnemyBreak(void)
{
	return EneI;
}

//㩂ŏ��֍U��
int PShATTACKHIT(void)
{
	return ATTACKHIT;
}

int GetYAttackHit(void)
{
	return YAttackHit;
}