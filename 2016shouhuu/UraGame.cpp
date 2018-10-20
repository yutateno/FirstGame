#include "DxLib.h"
#include "scenemgr.h"
#include "Media.h"
#include "Sound.h"

#define oneX 5			//�I���̈�Ԗڂ�X���W
#define twoX 165		//�I���̓�Ԗڂ�X���W
#define threeX 325		//�I���̎O�Ԗڂ�X���W
#define forX 485		//�I���̎l�Ԗڂ�X���W
#define botanY 412		//�I����Y���W

//�R���g���[���[
int Urapad;

//���e(�E�Ҋ֘A
int UraPswX, UraPswY;		//���̗E�҂̓���
int UraPshY;				//���̗E�҂̓���
int UraRand;				//�E�҂̍U�����闐��
bool UraPSFlag;				//�E�҂̍U���t���b�O
int UraRandF;				//����������ϐ�(��
bool UraPAttackFlag;		//�U������t���b�O
int UraRandFF;				//����������ϐ�(��

//���e(�G�֘A
int UraEnemyX, UraEnemyY;	//�G�̓���
bool UraEnemyAFlag;			//�G(����)���U������t���b�O

//���e(�I���֘A
int UraSentakunum;			//�I�����鏇��
int UraSentakuY;			//�I���{�^���̍��W
int UraSentakunumX;			//�c�őI�������Ƃ���
int UraSentakuUp;			//�I���ł̏�ړ������ɂ��邽��
int UraSentakuDown;			//�I���ł̉��ړ������ɂ��邽��
int UraSentakuK;			//�������񂾂��ɂ��邽��
int UraSentakuKFlag;		//�I�������܂����t���b�O
int UraSentakuB;			//�߂�
int UraGage = 240;			//�Q�[�W
int UraCount;				//�J�E���g

//���Z�b�g
void UraGame_R()
{
	UraPswX = 0;
	UraPswY = 0;
	UraPshY = 0;
	UraEnemyX = 0;
	UraEnemyY = 0;
	UraEnemyAFlag = 0;
	UraRand = 7;
	UraPSFlag = 0;
	UraRandF = 0;
	UraPAttackFlag = 0;
	UraRandFF = 0;
	UraSentakunum = 0;
	UraSentakuY = 0;
	UraSentakuUp = 0;
	UraSentakuDown = 0;
	UraSentakuK = 0;
	UraSentakuKFlag = 0;
	UraSentakuB = 0;
	UraGage = 240;
	UraCount = 0;
}

//���C���̍X�V
void UraGame_Update()
{
	UraRand = GetRand(8) + 1;	//1�`9�ɂ���
	Urapad = GetJoypadInputState(DX_INPUT_PAD1);
	PlaySoundMem(S_BGM.UraBGMM, DX_PLAYTYPE_LOOP, FALSE);
	if (((Urapad & PAD_INPUT_7) && (Urapad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	if (Urapad & PAD_INPUT_7)   //BACK�L�[��������Ă�����
	{
		StopSoundMem(S_BGM.UraBGMM);
		UraGame_R();
		SceneMgr_ChangeScene(eScene_MenuSub); //�����j���[�ɖ߂�
	}
}

//���̗E�Ҋ֘A
void UraGame_PlayerSword()
{
	//�U���p�x�̃J�E���g
	if (UraEnemyAFlag == 0)
	{
		UraGage --;
	}
	//277�Ŋ���؂ꂽ���ɍU��
	if (UraGage % 101 == 0 && UraGage != 0)
	{
		UraPSFlag = 1;
	}

	//�����Ɋւ���
	//�U������Ƃ��ɂȂ�����ǂ��ɍU�����邩��������
	if (UraPSFlag == 1)
	{
		UraRandF = UraRand;
		UraRandFF = UraRandF;
		UraPSFlag = 0;
	}
	//1.��U
	if (UraRandF == 1)
	{
		UraPAttackFlag = 1;
		UraPswY = -2;
	}
	//2.�^�U
	else if (UraRandF == 2)
	{
		UraPAttackFlag = 1;
		UraPswY = 0;
	}
	//3.���U
	else if (UraRandF == 3)
	{
		UraPAttackFlag = 1;
		UraPswY = 2;
	}
	//4.��h
	else if (UraRandF == 4)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//5.�^�h
	else if (UraRandF == 5)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//6.���h
	else if (UraRandF == 6)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//7.�㖂
	else if (UraRandF == 7)
	{
		UraPswX = 0;
		UraPswY = -2;
	}
	//8.�^��
	else if (UraRandF == 8)
	{
		UraPswX = 0;
		UraPswY = 0;
	}
	//9.����
	else if (UraRandF == 9)
	{
		UraPswX = 0;
		UraPswY = 2;
	}
	//�U������Ƃ�
	if (UraPAttackFlag == 1)
	{
		UraPswX += 10;
		if (UraPswX >= 250)
		{
			UraPswX = 0;
			UraRandF = 0;
			UraPAttackFlag = 0;
		}
	}

	//�G�Ɋւ���
	//�ʏ펞(�U�����Ă��Ȃ�
	if(UraPswX <= 0)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	//�U���ʒu�܂ňړ���
	else if (UraPswX > 0 && UraPswX <= 200)
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[1], TRUE);
	}
	//�U������
	else
	{
		DrawGraph(UraPswX + 100, (UraPswY * 40) + 250, M_Character.PlayerSwordM[2], TRUE);
	}
}

//���̗E�Ҋ֘A
void UraGame_PlayerShield()
{
	//�����Ɋւ���
	//1.��U
	if (UraRandFF == 1)
	{
		UraPshY = 0;
	}
	//2.�^�U
	else if (UraRandFF == 2)
	{
		UraPshY = 2;
	}
	//3.���U
	else if (UraRandFF == 3)
	{
		UraPshY = 0;
	}
	//4.��h
	else if (UraRandFF == 4)
	{
		UraPshY = -2;
	}
	//5.�^�h
	else if (UraRandFF == 5)
	{
		UraPshY = 0;
	}
	//6.���h
	else if (UraRandFF == 6)
	{
		UraPshY = 2;
	}
	//7.�㖂
	else if (UraRandFF == 7)
	{
		UraPshY = -2;
	}
	//8.�^��
	else if (UraRandFF == 8)
	{
		UraPshY = 0;
	}
	//9.����
	else if (UraRandFF == 9)
	{
		UraPshY = 2;
	}

	//�G�Ɋւ���
	//���@�h�䎞
	if (UraRandFF >= 7 && UraRandFF <= 9)
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldMahouM, TRUE);
	}
	//�ʏ펞
	else
	{
		DrawGraph(200, 250 + UraPshY * 40, M_Character.PlayerShieldM, TRUE);
	}
}

//�G�֘A(����)
void UraGame_Enemy()
{
	DrawBox(379, 119, 501, 141, GetColor(255, 255, 255), FALSE);
	DrawRectGraph(380, 120, UraGage / 2, 0, 120, 20, M_Character.PAttackBarM, FALSE, FALSE);
	if (UraEnemyAFlag == 0)
	{
		DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
	}
	//�U��������
	if (UraGage == 0)
	{
		UraEnemyAFlag = 1;
	}
	//�U���̃t���b�O��������
	if (UraEnemyAFlag == 1)
	{
		//�R���g���[���Ɋւ���
		//��ړ�
		if ((Urapad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0)
		{
			UraSentakuUp++;
		}
		else
		{
			UraSentakuUp = 0;
		}
		if (UraSentakuUp == 1 && UraSentakuY != 0)
		{
			UraSentakuY--;
		}
		//���ړ�
		if ((Urapad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0)
		{
			UraSentakuDown++;
		}
		else
		{
			UraSentakuDown = 0;
		}
		if (UraSentakuDown == 1 && ((UraSentakunum == 0 && UraSentakuY != 1) 
			|| (UraSentakunum == 1 && UraSentakuY != 1) 
			|| (UraSentakunum == 2 && UraSentakuY != 2) 
			|| (UraSentakunum == 3 && UraSentakuY != 2)))
		{
			UraSentakuY++;
		}
		//����L�[����������
		if ((Urapad & PAD_INPUT_B) || CheckHitKey(KEY_INPUT_A) != 0)
		{
			UraSentakuK++;
		}
		else
		{
			UraSentakuK = 0;
		}
		if (UraSentakuK == 1)
		{
			UraSentakuKFlag++;
		}
		if ((Urapad & PAD_INPUT_A) || CheckHitKey(KEY_INPUT_S) != 0)
		{
			UraSentakuB++;
		}
		else
		{
			UraSentakuB = 0;
		}
		if (UraSentakuB == 5 && UraSentakunum != 0)
		{
			UraSentakunumX = 1;
			UraSentakuB = 0;
			UraSentakuY = 0;
			UraSentakunum--;
		}
		DrawFormatString(oneX + 30, 412, GetColor(255, 255, 255), "�� ��");
		DrawFormatString(oneX + 30, 436, GetColor(255, 255, 255), "�� �u");
		//�ŏ��̑I��
		if (UraSentakunum == 0)
		{
			DrawGraph(oneX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
			//�P�i�ڂŉ����ꂽ��
			if (UraSentakuY == 0 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 1;
				UraSentakunum = 1;
			}
			//�Q�i�ڂŉ����ꂽ��
			if (UraSentakuY == 1 && UraSentakuKFlag == 1)
			{
				UraSentakuKFlag = 0;
				UraSentakuY = 0;
				UraSentakunumX = 2;
				UraSentakunum = 1;
			}
		}
		//��Ԗڂ̑I��
		if (UraSentakunum == 1)
		{
			DrawGraph(twoX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			//���߂��I�����ꂽ��
			if (UraSentakunumX == 1)
			{
				DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "�U ��");
				DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "�� ��");
				if (UraSentakuY == 0)
				{
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				if (UraSentakuY == 1)
				{
					SetDrawBright(125, 0, 0);
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					UraSentakuKFlag = 0;
					UraSentakuY = 0;
					UraSentakunumX = 1;
					UraSentakunum = 2;
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					UraSentakuKFlag = 0;
					UraSentakuY = 0;
					UraSentakunumX = 2;
					UraSentakunum = 2;
				}
			}
			//���u���I�����ꂽ��
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//�O�Ԗڂ̑I��
		if (UraSentakunum == 2)
		{
			DrawGraph(threeX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "�U ��");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "�� ��");
			//�U�����I�����ꂽ��
			if (UraSentakunumX == 1)
			{
				DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "���@�U��");
				DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " �� �U��");
				DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "�ʏ�U��");
				if (UraSentakuY == 0)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 1;
						UraSentakunum = 3;
					}
				}
				if (UraSentakuY == 1)
				{
					UraEnemyX = 60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 255, 0);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 2;
						UraSentakunum = 3;
					}
				}
				if (UraSentakuY == 2)
				{
					DrawRotaGraph(440, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					if (UraSentakuKFlag == 1)
					{
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 3;
						UraSentakunum = 3;
					}
				}
			}
			//�������I�����ꂽ��
			else if (UraSentakunumX == 2)
			{
				UraSentakunumX = 0;
				UraSentakunum = 0;
				UraEnemyAFlag = 0;
				UraGage = 240;
			}
		}
		//�Ō�̑I��
		if (UraSentakunum == 3)
		{
			DrawGraph(forX, botanY + UraSentakuY * 24, M_Enemy.UraSentakuM, TRUE);
			DrawFormatString(twoX + 30, 412, GetColor(255, 255, 255), "�U ��");
			DrawFormatString(twoX + 30, 436, GetColor(255, 255, 255), "�� ��");
			DrawFormatString(threeX + 30, 412, GetColor(255, 255, 255), "���@�U��");
			DrawFormatString(threeX + 30, 436, GetColor(255, 255, 255), " �� �U��");
			DrawFormatString(threeX + 30, 460, GetColor(255, 255, 255), "�ʏ�U��");
			//���@�U�����I�����ꂽ��
			if (UraSentakunumX == 1)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "�ォ��");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "�� �i");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "������");
				if (UraSentakuY == 0)
				{
					UraEnemyX = 30;
					UraEnemyY = -60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuY == 1)
				{
					UraEnemyX = 30;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuY == 2)
				{
					UraEnemyX = 30;
					UraEnemyY = 60;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "�I�H");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "���@��Y��Ă��܂����悤��///");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "wwww");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "���@��Y��Ă��܂����悤��.....");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 2)
				{
					SetDrawBright(255, 0, 255);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					SetDrawBright(255, 255, 255);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "�I�H//");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "�M�o���Ȃ��Ȃ����悤��///");
					}
					if (UraCount > 50)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraCount = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
			//�ʏ�U�����I�����ꂽ��
			if (UraSentakunumX == 3)
			{
				DrawFormatString(forX + 30, 412, GetColor(255, 255, 255), "�ォ��");
				DrawFormatString(forX + 30, 436, GetColor(255, 255, 255), "�� �i");
				DrawFormatString(forX + 30, 460, GetColor(255, 255, 255), "������");
				if (UraSentakuY == 0)
				{
					UraEnemyX = 30;
					UraEnemyY = -60;
					DrawGraph(300, 170, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				else if (UraSentakuY == 1)
				{
					UraEnemyX = 30;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				else if (UraSentakuY == 2)
				{
					UraEnemyX = 30;
					UraEnemyY = 60;
					DrawGraph(300, 250, M_Enemy.AttackArrowM, TRUE);
					DrawGraph(300, 330, M_Enemy.AttackArrowM, TRUE);
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
				}
				if (UraSentakuKFlag == 1 && UraSentakuY == 0)
				{
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "�ːi����܂����B");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "�����e�i���X���Ƃ������Ƃ�");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				else if (UraSentakuKFlag == 1 && UraSentakuY == 1)
				{
					DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "�ːi�ɂ��ł�");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "��J���ޒ��ł�");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				else if (UraSentakuKFlag == 1 && UraSentakuY == 2)
				{
					DrawRotaGraph(440 + UraEnemyX, 270 + UraEnemyY, 1, 0, M_Enemy.UraEnemyM, TRUE);
					UraCount++;
					if (UraCount <= 20)
					{
						DrawFormatString(320, 240, GetColor(0, 0, 0), "�ːi��ꂽ");
					}
					if (UraCount > 20 && UraCount <= 50)
					{
						DrawFormatString(240, 240, GetColor(0, 0, 0), "������Ƌx�e������");
					}
					if (UraCount > 50)
					{
						UraCount = 0;
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakuKFlag = 0;
						UraSentakuY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
			//���U�����I�����ꂽ��
			if (UraSentakunumX == 2)
			{
				SetDrawBright(255, 255, 0);
				DrawRotaGraph(440 + UraEnemyX, 270, 1, 0, M_Enemy.UraEnemyM, TRUE);
				SetDrawBright(255, 255, 255);
				//����ĂȂ�
				if (UraPshY != UraPswY)
				{
					UraEnemyX -= 30;
					if (UraEnemyX <= -240)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
				//����Ă�
				else if (UraPshY == UraPswY)
				{
					UraEnemyX -= 25;
					if (UraEnemyX <= -190)
					{
						UraEnemyX = 0;
						UraEnemyY = 0;
						UraSentakunumX = 0;
						UraSentakunum = 0;
						UraEnemyAFlag = 0;
						UraGage = 240;
					}
				}
			}
		}
	}
}

//���C���̕`��
void UraGame_Draw()
{
	DrawGraph(0, 0, M_BackGround.UraBackGroundM, FALSE);	//�w�i
	UraGame_PlayerSword();
	UraGame_PlayerShield();
	UraGame_Enemy();
}