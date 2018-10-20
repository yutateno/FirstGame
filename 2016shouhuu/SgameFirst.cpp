#include "SceneMgr.h"
#include "Background.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//���e
int bright = 0;		//�t�F�[�h�C��
int Mcount;			//�ŏ��̖����ړ�����I���܂�
int SGFvol;			//������艹�̑傫���グ�Ă���
int EY = 225;		//�G��Y���W
double EXM;			//�G�̏o���J�E���g

//�R���g���[���[
int SGFpad;		//�R���g���[���[


//���x���グ�Ă���
void Update()
{
	if (bright <= 255)
	{
		bright += 3;
	}
}
//���x���グ�Ă���
void Bright()
{
	if (bright <= 255)
	{
		SetDrawBright(bright, bright, bright);
	}
}

//�X�V
void SgameFirst_Update()
{
	SGFpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((SGFpad & PAD_INPUT_7) && (SGFpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[��������Ă�����
	{
		DxLib_End(); // DX���C�u�������I������
	}
	Update();
	Background_Update();
	Mcount++;
}

//�`��
void SgameFirst_Draw()
{
	//���ʂ̐ݒ�(���񂾂񉹂��o���Ă���
	SGFvol++;
	ChangeVolumeSoundMem(SGFvol * 2 / 3, S_BGM.ZakoS);

	//���̍Đ�
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);
	
	Bright();
	Background_Draw();
	
	DrawGraph(200, 250, M_Character.PlayerShieldM, TRUE);
	if (Mcount < 100)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (Mcount >= 100 && Mcount < 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[1], TRUE);
	}
	if (Mcount >= 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(670 - (int)EXM, EY, M_Enemy.EnemyoM[0], TRUE);
		EXM += 1.5;
		if ((670 - (int)EXM) <= 410)
		{
			StopSoundMem(S_BGM.ZakoS);
			SceneMgr_ChangeScene(eScene_SGame);	//�퓬��ʂɉf��
		}
	}
}

//���Z�b�g
void SGameFirst_R()
{
	Mcount = 0;
	EY = 225;
	EXM = 0;
	SGFvol = 0;
	bright = 0;
}