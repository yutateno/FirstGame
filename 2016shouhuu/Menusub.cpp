#include "SceneMgr.h"
#include "DxLib.h"
#include "Sound.h"
#include "Media.h"

//�R���g���[���[
int MSubpad;

//���e
int MenuSubUp;		//��ړ�
int MenuSubDown;	//���ړ�
bool SForceEnd;		//�����I��
int SSPoint;		//�I���J�[�\���̈ʒu

//�e�����̊֐�
void OneSub(void);		//��l�v���C������֐�
void TwoSub(void);		//��l�v���C������֐�
void EndSub(void);		//�I������֐�

//�����|�C���^�z��
void(*SubMethod[])() =
{
	OneSub,TwoSub,EndSub
};

//�`��
void MenuSub_Draw()
{
	SForceEnd = 0;
	// �I���|�C���^�̈ʒu������
	SSPoint = 0;

	// ���[�v
	while (true)
	{
		// ���ʂ̐ݒ�
		ChangeVolumeSoundMem(255, S_BGM.MenuSubS);
		PlaySoundMem(S_BGM.MenuSubS, DX_PLAYTYPE_LOOP, FALSE);
		DrawGraph(0, 0, M_BackGround.TitleSubM, FALSE);
		// �J�[�\���`��
		DrawBox(430, 320 + (SSPoint * 32), 446, 336 + (SSPoint * 32), GetColor(255, 255, 0), TRUE);
		DrawFormatString(462, 320, GetColor(0, 0, 0), "�P�ŗV��");
		DrawFormatString(462, 352, GetColor(0, 0, 0), "�߂�");
		DrawFormatString(462, 384, GetColor(0, 0, 0), "�I������");

		// �L�[���͂ɉ����ď���
		MSubpad = GetJoypadInputState(DX_INPUT_PAD1);
		//Esc�L�[��������Ă�����
		if (((MSubpad & PAD_INPUT_7) && (MSubpad & PAD_INPUT_8)) || CheckHitKey(KEY_INPUT_ESCAPE) != 0)
		{
			SForceEnd = 1;
			break;
		}
		//��{�^���������ꂽ��
		if (((MSubpad & PAD_INPUT_UP) || CheckHitKey(KEY_INPUT_UP) != 0))
		{
			MenuSubUp++;
		}
		else
		{
			MenuSubUp = 0;
		}
		//���{�^���������ꂽ��
		if (((MSubpad & PAD_INPUT_DOWN) || CheckHitKey(KEY_INPUT_DOWN) != 0))
		{
			MenuSubDown++;
		}
		else
		{
			MenuSubDown = 0;
		}
		//��ړ�����
		if (MenuSubUp == 1 && SSPoint != 0)
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			SSPoint--;
		}
		//���ړ�����
		if (MenuSubDown == 1 && SSPoint != 2)	//���ڂ��R�Ȃ̂�
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			SSPoint++;
		}
		//����L�[������
		if ((MSubpad & PAD_INPUT_X) || CheckHitKey(KEY_INPUT_X) != 0)	//����Y����
		{
			PlaySoundMem(S_SE.BotanS, DX_PLAYTYPE_BACK, TRUE);
			StopSoundMem(S_BGM.MenuSubS);
			//���ڂɉ������������s��
			SubMethod[SSPoint]();
			break;
		}
	}
	//�����I��
	if (SForceEnd == 1)
	{
		DxLib_End(); // DX���C�u�������I������
	}
}

void OneSub()
{
	SceneMgr_ChangeScene(eScene_UraGame);   //�V�[���𗠃Q�[����ʂɕύX
}

void TwoSub()
{
	SceneMgr_ChangeScene(eScene_Menu);		//�V�[�������j���[��
}

void EndSub()
{
	DxLib_End();							//DX���C�u�������I������
}