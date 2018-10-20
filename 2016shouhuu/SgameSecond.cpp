#include "SceneMgr.h"
#include "Background.h"
#include "PlayerSword.h"
#include "PlayerShield.h"
#include "DxLib.h"
#include "Media.h"
#include "Sound.h"

//���e
int Dark = 255;			//�Â�����Ƃ��̌��x
int Bright;				//���邭����Ƃ��̌��x
int MOveX;				//�ŏ��̈ړ�
int SSCount;			//���̃V�[����S���J�E���g
double EnemyMoveX;		//�G�̈ړ�
bool SGSFlag;			//�G����|�����t���b�O(�������Ԃ𐳂������邽�߂̏��u

//�R���g���[���[
int SGSpad;		//�R���g���[���[

//���x�������Ă���
void Down()
{
	if (Dark >= 0)
	{
		Dark -= 3;
	}
}
void DARK()
{
	if (Dark >= 0)
	{
		SetDrawBright(Dark, Dark, Dark);
	}
}

//���x���グ�Ă���
void UP()
{
	if (Bright <= 255)
	{
		Bright += 5;
	}
}
void BRIGHT()
{
	if (Bright <= 255)
	{
		SetDrawBright(Bright, Bright, Bright);
	}
}

//�X�V
void SgameSecond_Update()
{
	SGSpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGSpad & PAD_INPUT_7) && (SGSpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	SSCount++;
	SGSFlag = 1;
	if (SSCount >= 60)
	{
		MOveX += 4;
	}
	if (SSCount >= 63)
	{
		PlaySoundMem(S_BGM.AsiotoS, DX_PLAYTYPE_LOOP, FALSE);
	}
	if (SSCount >= 100 && SSCount <= 200)
	{
		Down();
	}
	if (SSCount >= 100)
	{
		StopSoundMem(S_BGM.AsiotoS);
	}
	if (SSCount >= 220)
	{
		UP();
		LastBackground_Update();
	}
}

//�`��
void SgameSecond_Draw()
{
	if (SSCount <= 200)
	{
		Background_Draw();
		DrawGraph(MOveX + 100, (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(MOveX + 200, 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
		DARK();
	}
	if (SSCount >= 220)
	{
		BRIGHT();
		LastBackground_Draw();
	}
	if (SSCount >= 220 && SSCount <= 420)
	{
		DrawGraph(200, 220, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(200, 280, M_Character.PlayerShieldM, TRUE);
	}
	if (SSCount >= 360 && SSCount <= 480)
	{
		EnemyMoveX += 2.25;
		DrawGraph(640 - (int)EnemyMoveX, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCount >= 420)
	{
		DrawGraph(100, (GetswordYpoint() * 40) + 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(200, 250 + (Getshieldpoint() * 40), M_Character.PlayerShieldM, TRUE);
	}
	if (SSCount >= 480)
	{
		DrawGraph(365, 195, M_Enemy.LastEnemyM, TRUE);
	}
	if (SSCount == 490)
	{
		SceneMgr_ChangeScene(eScene_SgameLast);
	}
}

//���Z�b�g
void SGameSecond_R()
{
	Dark = 255;
	Bright = 0;
	MOveX = 0;
	SSCount = 0;
	EnemyMoveX = 0;
	SGSFlag = 0;
}

//�������Ԃ̌��蒲��
bool SGSecondFlag()
{
	return SGSFlag;
}