#include "SceneMgr.h"
#include "Background.h"
#include "DxLib.h"
#include "SgameFirst.h"
#include "Sound.h"
#include "Media.h"

//���e
int GFvol;		//BGM�̃{�����[��
int McounT;		//�ŏ��̖����ړ�����I���܂�(�����ł̓������i���Ă���
int ey = 225;	//�G��Y���W
double EXMc;	//�G�̏o���J�E���g

//�R���g���[���[
int GFpad;

//�X�V
void GameFirst_Update()
{
	GFpad = GetJoypadInputState(DX_INPUT_PAD1);
	if (((GFpad & PAD_INPUT_7) && (GFpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)   //Esc�L�[,START&BACK
	{
		DxLib_End(); // DX���C�u�������I������
	}
	Update();	//��l�p�Ɠ����Ȃ̂ŕ��p�i���g�̓t�F�[�h�C���̏���
	Background_Update();
	McounT++;
}

//�`��
void GameFirst_Draw()
{
	//���ʂ̐ݒ�(���񂾂񉹂��o���Ă���
	GFvol++;
	ChangeVolumeSoundMem(GFvol*2/3, S_BGM.ZakoS);

	//���̍Đ�
	PlaySoundMem(S_BGM.ZakoS, DX_PLAYTYPE_LOOP, FALSE);
	Bright();		//��l�p�Ɠ����Ȃ̂ŕ��p�i���g�̓t�F�[�h�C���̏���
	Background_Draw();

	if (McounT < 100)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
	}
	if (McounT >= 100 && McounT < 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[1], TRUE);
	}
	if (McounT >= 200)
	{
		DrawGraph(100, 250, M_Character.PlayerSwordM[0], TRUE);
		DrawGraph(670 - (int)EXMc, ey, M_Enemy.EnemyoM[0], TRUE);
		EXMc += 1.5;
		if ((670 - (int)EXMc) <= 410)
		{
			StopSoundMem(S_BGM.ZakoS);
			SceneMgr_ChangeScene(eScene_Game);	//�퓬��ʂɉf��
		}
	}
}

//���Z�b�g
void GameFirst_R()
{
	McounT = 0;
	ey = 225;
	EXMc = 0;
	GFvol = 0;
}