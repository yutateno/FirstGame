#include "SceneMgr.h"
#include "Background.h"
#include "Player.h"
#include "SgameSecond.h"
#include "DxLib.h"
#include "Media.h"
#include "Sound.h"

//���e
int MOvEX;			//�ŏ��̈ړ�
int SSCounT;		//���̃V�[����S���J�E���g
double EnemyMovEX;	//�G�̈ړ�
bool GSFlag;		//���̃V�[���ɓ������t���b�O�i�������Ԃ������ł��A�N�e�B�u�Ȃ̂ł��̒����p

//�R���g���[���[
int GSpad;

//�X�V
void GameSecond_Update()
{
	GSpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GSpad & PAD_INPUT_7) && (GSpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	SSCounT++;
	GSFlag = 1;
	if (SSCounT >= 60)
	{
		MOvEX += 4;
	}
	if (SSCounT >= 63)
	{
		PlaySoundMem(S_BGM.AsiotoS, DX_PLAYTYPE_LOOP, FALSE);
	}
	if (SSCounT >= 100 && SSCounT <= 200)
	{
		Down();		//��l�p�Ɠ����Ȃ̂ŕ��p�i���g�̓t�F�[�h�A�E�g�̏���
	}
	if (SSCounT >= 100)
	{
		StopSoundMem(S_BGM.AsiotoS);
	}
	if (SSCounT >= 220)
	{
		UP();		//��l�p�̓����Ȃ̂ŕ��p�i���g�̓t�F�[�h�C���̏���
		LastBackground_Update();
	}
}

//�`��
void GameSecond_Draw()
{
	if (SSCounT <= 200)
	{
		Background_Draw();
		DrawGraph(MOvEX + 100, (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		DARK();		//��l�p�Ɠ����Ȃ̂ŕ��p�i���g�̓t�F�[�h�A�E�g�̏���
	}
	if (SSCounT >= 220)
	{
		BRIGHT();	//��l�p�Ɠ����Ȃ̂ŕ��p�i���g�̓t�F�[�h�C���̏���
		LastBackground_Draw();
	}
	if (SSCounT >= 220 && SSCounT <= 420)
	{
		DrawGraph(200, 220, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SSCounT >= 360 && SSCounT <= 480)
	{
		EnemyMovEX += 2.25;
		DrawGraph(640 - (int)EnemyMovEX, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCounT >= 420)
	{
		DrawGraph(100, (GetPlayerYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (SSCounT >= 480)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCounT == 490)
	{
		SceneMgr_ChangeScene(eScene_GameLast);
	}
}

//���Z�b�g
void GameSecond_R()
{
	MOvEX = 0;
	SSCounT = 0;
	EnemyMovEX = 0;
	GSFlag = 0;
}

//�������Ԃ̌��蒲��
bool GSecondFlag()
{
	return GSFlag;
}